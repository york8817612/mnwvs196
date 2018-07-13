#include "Center.h"
#include <functional>
#include <thread>

#include "..\WvsLib\Net\InPacket.h"
#include "..\WvsLib\Net\OutPacket.h"

#include "..\WvsLib\Net\PacketFlags\LoginPacketFlags.hpp"
#include "..\WvsLib\Net\PacketFlags\CenterPacketFlags.hpp"

#include "..\WvsLib\Constants\ServerConstants.hpp"

#include "WvsLogin.h"
#include "..\WvsLib\Logger\WvsLogger.h"

Center::Center(asio::io_service& serverService)
	: SocketBase(serverService, true)
{
}

Center::~Center()
{
}

void Center::SetCenterIndex(int idx)
{
	nCenterIndex = idx;
}

void Center::OnConnected()
{
	WvsLogger::LogRaw(WvsLogger::LEVEL_INFO, "[WvsLogin][Center::OnConnect]���\�s�u��Center Server�I\n");

	//�VCenter Server�o�eHand Shake�ʥ]
	OutPacket oPacket;
	oPacket.Encode2(LoginSendPacketFlag::Center_RegisterCenterRequest);

	//WvsLogin��ServerType��SVR_LOGIN
	oPacket.Encode1(ServerConstants::ServerType::SRV_LOGIN);

	SendPacket(&oPacket); 
	OnWaitingPacket();
}

void Center::OnPacket(InPacket *iPacket)
{
	WvsLogger::LogRaw("[WvsLogin][Center::OnPacket]�ʥ]�����G");
	iPacket->Print();
	int nType = (unsigned short)iPacket->Decode2();
	switch (nType)
	{
	case CenterSendPacketFlag::RegisterCenterAck:
	{
		auto result = iPacket->Decode1();
		if (!result)
		{
			WvsLogger::LogRaw(WvsLogger::LEVEL_ERROR, "[WvsLogin][RegisterCenterAck][���~]Center Server�ڵ���eLocalServer�s���A�{���Y�N�פ�C\n");
			exit(0);
		}
		WvsLogger::LogRaw(WvsLogger::LEVEL_INFO, "[WvsLogin][RegisterCenterAck]Center Server �{�ҧ����A�P�@�ɦ��A���s�u���\�إߡC\n");
		OnUpdateWorldInfo(iPacket);
		break;
	}
	case CenterSendPacketFlag::CenterStatChanged:
		OnUpdateChannelInfo(iPacket);
		break;
	case CenterSendPacketFlag::CharacterListResponse:
		OnCharacterListResponse(iPacket);
		break;
	case CenterSendPacketFlag::CharacterCreateResponse:
		OnCharacterCreateResponse(iPacket);
		break;
	case CenterSendPacketFlag::GameServerInfoResponse:
		OnGameServerInfoResponse(iPacket);

		break;
	}
}

void Center::OnClosed()
{
}

void Center::OnUpdateChannelInfo(InPacket * iPacket)
{
	m_WorldInfo.nGameCount = iPacket->Decode2();
	memset(m_WorldInfo.m_aChannelStatus, 0, sizeof(int) * ServerConstants::kMaxChannelCount);
	for (int i = 0; i < m_WorldInfo.nGameCount; ++i)
		m_WorldInfo.m_aChannelStatus[(iPacket->Decode1())] = 1;
}

void Center::OnUpdateWorldInfo(InPacket *iPacket)
{
	m_WorldInfo.nWorldID = iPacket->Decode1();
	m_WorldInfo.nEventType = iPacket->Decode1();
	m_WorldInfo.strWorldDesc = iPacket->DecodeStr();
	m_WorldInfo.strEventDesc = iPacket->DecodeStr();
	WvsLogger::LogRaw(WvsLogger::LEVEL_INFO, "[WvsLogin][Center::OnUpdateWorld]�C�����A���@�ɸ�T��s�C\n");
}

void Center::OnConnectFailed()
{
	WvsLogger::LogRaw(WvsLogger::LEVEL_ERROR, "[WvsShop][Center::OnConnect]�L�k�s�u��Center Server�A�i��O�A�ȩ|���ҰʩΪ̽T�{�s�u��T�C\n");
	OnDisconnect();
}

void Center::OnCharacterListResponse(InPacket *iPacket)
{
	unsigned int nLoginSocketID = iPacket->Decode4();
	auto pSocket = WvsBase::GetInstance<WvsLogin>()->GetSocket(nLoginSocketID);
	OutPacket oPacket;
	oPacket.Encode2(LoginSendPacketFlag::Client_ClientSelectWorldResult);
	oPacket.Encode1(0);
	oPacket.EncodeStr("normal");
	oPacket.Encode4(0);
	oPacket.Encode1(0);
	oPacket.Encode4(0);
	oPacket.Encode8(0);
	oPacket.Encode1(0);

	WvsLogger::LogRaw("[WvsLogin][Center::OnCharacterListResponse]���a�֦�����M��ʥ] : ");
	iPacket->Print();
	WvsLogger::LogRaw("\n");

	oPacket.EncodeBuffer(iPacket->GetPacket() + 6, iPacket->GetPacketSize() - 6);

	/*int chrSize = iPacket->Decode4();
	oPacket.Encode4(chrSize); //char size
	for (int i = 0; i < chrSize; ++i)
		oPacket.Encode4(iPacket->Decode4());

	chrSize = iPacket->Decode1();
	oPacket.Encode1(chrSize); //char size
	for (int i = 0; i < chrSize; ++i)
	{
		OnEncodingCharacterStat(&oPacket, iPacket);
		OnEncodingCharacterAvatar(&oPacket, iPacket);
		oPacket.Encode1(iPacket->Decode1());
		OnEncodingRank(&oPacket, iPacket);
	}*/

	oPacket.Encode1(0x03);
	oPacket.Encode1(0);
	oPacket.Encode4(8); //char slots

	oPacket.Encode4(0);
	oPacket.Encode4(-1);
	oPacket.Encode1(0);
	oPacket.EncodeTime(-1);
	oPacket.Encode1(0);
	oPacket.Encode1(0);
	oPacket.Encode4(0);
	for (int i = 0; i < 25; ++i)
		oPacket.Encode2(0);
	oPacket.Encode4(0);
	oPacket.Encode8(0);
	pSocket->SendPacket(&oPacket);
}

void Center::OnCharacterCreateResponse(InPacket *iPacket)
{
	int nLoginSocketID = iPacket->Decode4();
	auto pSocket = WvsBase::GetInstance<WvsLogin>()->GetSocket(nLoginSocketID);
	OutPacket oPacket;
	oPacket.Encode2(LoginSendPacketFlag::ClientCreateNewCharacterResult);
	oPacket.Encode1(0);

	printf("[WvsLogin][Center::OnCharacterCreateResponse]���a�إߨ���ʥ] : ");
	iPacket->Print();
	printf("\n");

	oPacket.EncodeBuffer(iPacket->GetPacket() + 6, iPacket->GetPacketSize() - 6);

	pSocket->SendPacket(&oPacket);
}

void Center::OnGameServerInfoResponse(InPacket *iPacket)
{
	unsigned int nLoginSocketID = iPacket->Decode4();
	auto pSocket = WvsBase::GetInstance<WvsLogin>()->GetSocket(nLoginSocketID); 
	OutPacket oPacket;
	oPacket.Encode2(LoginSendPacketFlag::Client_ClientSelectCharacterResult);
	oPacket.EncodeBuffer(iPacket->GetPacket() + 6, iPacket->GetPacketSize() - 6);
	pSocket->SendPacket(&oPacket);
}

void Center::OnNotifyCenterDisconnected(SocketBase * pSocket)
{
	WvsLogger::LogRaw("[WvsLogin][Center]�PCenter Server���_�s�u�C\n");
}
