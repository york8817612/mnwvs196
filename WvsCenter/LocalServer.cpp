
#include "../Database/CharacterDBAccessor.h"
#include "LocalServer.h"
#include "..\WvsLib\Net\InPacket.h"
#include "..\WvsLib\Net\OutPacket.h"

#include "..\WvsLib\Net\PacketFlags\LoginPacketFlags.hpp"
#include "..\WvsLib\Net\PacketFlags\CenterPacketFlags.hpp"
#include "..\WvsLib\Net\PacketFlags\ShopPacketFlags.hpp"
#include "..\WvsLib\Net\PacketFlags\GamePacketFlags.hpp"

#include "..\WvsLib\Constants\ServerConstants.hpp"
#include "WvsCenter.h"
#include "WvsWorld.h"
#include "UserTransferStatus.h"

LocalServer::LocalServer(asio::io_service& serverService)
	: SocketBase(serverService, true)
{
}

LocalServer::~LocalServer()
{
}

void LocalServer::OnClosed()
{

}

void LocalServer::OnPacket(InPacket *iPacket)
{
	WvsLogger::LogRaw("[WvsCenter][LocalServer::OnPacket]�ʥ]�����G");
	iPacket->Print();
	int nType = (unsigned short)iPacket->Decode2();
	switch (nType)
	{
		case LoginSendPacketFlag::Center_RegisterCenterRequest:
			OnRegisterCenterRequest(iPacket);
			break;
		case LoginSendPacketFlag::Center_RequestCharacterList:
			OnRequestCharacterList(iPacket);
			break;
		case LoginSendPacketFlag::Center_RequestCreateNewCharacter:
			OnRequestCreateNewCharacter(iPacket);
			break;
		case LoginSendPacketFlag::Center_RequestGameServerInfo:
			OnRequestGameServerInfo(iPacket);
			break;
		case GameSendPacketFlag::RequestMigrateIn:
			OnRequestMigrateIn(iPacket);
			break;
		case ShopSendPacketFlag::RequestMigrateOut:
		case GameSendPacketFlag::RequestMigrateOut:
			OnRequestMigrateOut(iPacket);
			break;
		case ShopSendPacketFlag::RequestTransferToGame:
		case GameSendPacketFlag::RequestTransferChannel:
			OnRequestTransferChannel(iPacket);
			break;
		case GameSendPacketFlag::RequestTransferShop:
			OnRequestMigrateCashShop(iPacket);
			break;
	}
}

void LocalServer::OnRegisterCenterRequest(InPacket *iPacket)
{
	auto serverType = iPacket->Decode1();
	SetServerType(serverType);
	const char* pInstanceName = (serverType == ServerConstants::SRV_LOGIN ? "WvsLogin" : (serverType == ServerConstants::SRV_GAME ? "WvsGame" : "WvsShop"));
	WvsLogger::LogFormat("[WvsCenter][LocalServer::OnRegisterCenterRequest]����s��[%s][%d]�s�u�ШD�C\n", pInstanceName, serverType);

	if (serverType == ServerConstants::SRV_GAME)
	{
		WvsBase::GetInstance<WvsCenter>()->RegisterChannel(shared_from_this(), iPacket);
		WvsBase::GetInstance<WvsCenter>()->NotifyWorldChanged();
	}

	OutPacket oPacket;
	oPacket.Encode2(CenterSendPacketFlag::RegisterCenterAck);
	oPacket.Encode1(1); //Success;
	if (serverType == ServerConstants::SRV_LOGIN)
	{
		auto pWorld = WvsWorld::GetInstance();
		oPacket.Encode1(pWorld->GetWorldInfo().nWorldID);
		oPacket.Encode1(pWorld->GetWorldInfo().nEventType);
		oPacket.EncodeStr(pWorld->GetWorldInfo().strWorldDesc);
		oPacket.EncodeStr(pWorld->GetWorldInfo().strEventDesc);
		WvsBase::GetInstance<WvsCenter>()->NotifyWorldChanged();
		//printf("[LocalServer::OnRegisterCenterRequest]Encoding World Information.\n");
	}
	
	if (serverType == ServerConstants::SRV_SHOP)
		WvsBase::GetInstance<WvsCenter>()->RegisterCashShop(shared_from_this(), iPacket);

	SendPacket(&oPacket);
}

void LocalServer::OnRequestCharacterList(InPacket *iPacket)
{
	int nLoginSocketID = iPacket->Decode4();
	int nAccountID = iPacket->Decode4();
	int nChannelID = iPacket->Decode1();
	if(WvsBase::GetInstance<WvsCenter>()->GetChannel(nChannelID) != nullptr)
		CharacterDBAccessor::GetInstance()->PostLoadCharacterListRequest(this, nLoginSocketID, nAccountID, WvsWorld::GetInstance()->GetWorldInfo().nWorldID);
}

void LocalServer::OnRequestCreateNewCharacter(InPacket *iPacket)
{
	int nLoginSocketID = iPacket->Decode4();
	int nAccountID = iPacket->Decode4();

	std::string strName = iPacket->DecodeStr();
	iPacket->Decode4();
	iPacket->Decode4();

	int nJobType = iPacket->Decode4();
	int nSubJob = iPacket->Decode2();
	unsigned char nGender = iPacket->Decode1();
	unsigned char nSkin = iPacket->Decode1();
	iPacket->Decode1();
	int nFace = iPacket->Decode4();
	int nHair = iPacket->Decode4();

	int nTopID = iPacket->Decode4();
	int nShoesID = iPacket->Decode4();
	int nWeaponID = iPacket->Decode4();

	int aEquips[CharacterDBAccessor::EQP_ID_FLAG_END] = { 0 };
	int aStats[CharacterDBAccessor::STAT_FLAG_END] = { 0 };

	CharacterDBAccessor::GetDefaultCharacterStat(aStats);

	aEquips[CharacterDBAccessor::EQP_ID_TopEquip] = nTopID;
	aEquips[CharacterDBAccessor::EQP_ID_ShoesEquip] = nShoesID;
	aEquips[CharacterDBAccessor::EQP_ID_WeaponEquip] = nWeaponID;

	CharacterDBAccessor::GetInstance()->PostCreateNewCharacterRequest(
		this, 
		nLoginSocketID, 
		nAccountID, 
		WvsWorld::GetInstance()->GetWorldInfo().nWorldID, 
		strName, 
		nGender, 
		nFace, 
		nHair, 
		nSkin, 
		(const int*)aEquips, 
		(const int*)aStats);
}

void LocalServer::OnRequestGameServerInfo(InPacket *iPacket)
{
	int nLoginSocketID = iPacket->Decode4();
	int nWorldID = iPacket->Decode4();
	if (nWorldID != WvsWorld::GetInstance()->GetWorldInfo().nWorldID)
	{
		WvsLogger::LogRaw(WvsLogger::LEVEL_ERROR, "[WvsCenter][LocalServer::OnRequstGameServerInfo]���`�G�Ȥ�ݹ��ճs�u�ܤ��s�b���W�D���A��[WvsGame]�C\n");
		return;
	}
	int nChannelID = iPacket->Decode4();
	int nCharacterID = iPacket->Decode4();
	
	OutPacket oPacket;
	oPacket.Encode2(CenterSendPacketFlag::GameServerInfoResponse);
	oPacket.Encode4(nLoginSocketID);
	oPacket.Encode2(0);
	oPacket.Encode4(WvsBase::GetInstance<WvsCenter>()->GetChannel(nChannelID)->GetExternalIP());
	oPacket.Encode2(WvsBase::GetInstance<WvsCenter>()->GetChannel(nChannelID)->GetExternalPort());
	oPacket.Encode4(0);
	oPacket.Encode2(0);
	oPacket.Encode4(0);
	oPacket.Encode2(0);
	oPacket.Encode4(nCharacterID);
	oPacket.Encode1(0);
	oPacket.Encode4(0);

	WvsWorld::GetInstance()->ClearUserTransferStatus(nCharacterID);
	SendPacket(&oPacket);
}

void LocalServer::OnRequestMigrateIn(InPacket *iPacket)
{
	int nClientSocketID = iPacket->Decode4();
	int nCharacterID = iPacket->Decode4();
	OutPacket oPacket;
	oPacket.Encode2(CenterSendPacketFlag::CenterMigrateInResult);
	oPacket.Encode4(nClientSocketID);
	CharacterDBAccessor::GetInstance()->PostCharacterDataRequest(this, nClientSocketID, nCharacterID, &oPacket); // for WvsGame
	auto pUserTransferStatus = WvsWorld::GetInstance()->GetUserTransferStatus(nCharacterID);

	WvsLogger::LogFormat("OnRequestMigrateIn, has transfer status ? %d\n", (pUserTransferStatus != nullptr));
	if (pUserTransferStatus == nullptr)
		oPacket.Encode1(0);
	else
	{
		oPacket.Encode1(1);
		pUserTransferStatus->Encode(&oPacket);
		//WvsWorld::GetInstance()->ClearUserTransferStatus(nCharacterID);
	}
	//CharacterDBAccessor::GetInstance()->PostCharacterDataRequest(this, nClientSocketID, nCharacterID, &oPacket); // for Client
	this->SendPacket(&oPacket);
}

void LocalServer::OnRequestMigrateOut(InPacket * iPacket)
{
	int nClientSocketID = iPacket->Decode4();
	int nCharacterID = iPacket->Decode4();
	CharacterDBAccessor::GetInstance()->OnCharacterSaveRequest(iPacket);
	bool bGameEnd = iPacket->Decode1() == 1 ? true : false;
	WvsLogger::LogFormat("OnRequestMigrateOut is game end = %d\n", bGameEnd == true ? 1 : 0);
	if (!bGameEnd)
	{
		UserTransferStatus* pStatus = new UserTransferStatus;
		pStatus->Decode(iPacket);
		WvsWorld::GetInstance()->SetUserTransferStatus(nCharacterID, pStatus);
	}
	else
		WvsWorld::GetInstance()->ClearUserTransferStatus(nCharacterID);
}

void LocalServer::OnRequestTransferChannel(InPacket * iPacket)
{
	WvsLogger::LogRaw("OnRequestTransferChannel\n");
	int nClientSocketID = iPacket->Decode4();
	int nCharacterID = iPacket->Decode4();
	int nChannelID = iPacket->Decode1();
	auto pEntry = WvsBase::GetInstance<WvsCenter>()->GetChannel(nChannelID);
	OutPacket oPacket;
	oPacket.Encode2(CenterSendPacketFlag::TransferChannelResult);
	oPacket.Encode4(nClientSocketID);
	oPacket.Encode1((pEntry != nullptr ? 1 : 0)); //bSuccess
	if (pEntry != nullptr)
	{
		oPacket.Encode1(1);
		oPacket.Encode4(pEntry->GetExternalIP());
		oPacket.Encode2(pEntry->GetExternalPort());
		oPacket.Encode4(0);
	}
	this->SendPacket(&oPacket);
}

void LocalServer::OnRequestMigrateCashShop(InPacket * iPacket)
{
	int nClientSocketID = iPacket->Decode4();
	int nCharacterID = iPacket->Decode4();
	OutPacket oPacket;
	oPacket.Encode2(CenterSendPacketFlag::MigrateCashShopResult);
	oPacket.Encode4(nClientSocketID);
	auto pEntry = WvsBase::GetInstance<WvsCenter>()->GetShop();
	if (WvsBase::GetInstance<WvsCenter>()->GetShop() == nullptr)
		oPacket.Encode1(0); //bSuccess
	else
	{
		oPacket.Encode1(1);
		oPacket.Encode1(1);
		oPacket.Encode4(pEntry->GetExternalIP());
		oPacket.Encode2(pEntry->GetExternalPort());
		oPacket.Encode4(0);
	}
	this->SendPacket(&oPacket);
}
