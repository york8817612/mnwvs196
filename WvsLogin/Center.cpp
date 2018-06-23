#include "Center.h"
#include <functional>
#include <thread>

#include "Net\InPacket.h"
#include "Net\OutPacket.h"

#include "Net\PacketFlags\LoginPacketFlags.hpp"
#include "Net\PacketFlags\CenterPacketFlags.hpp"

#include "Constants\ServerConstants.hpp"

#include "WvsLogin.h"

Center::Center(asio::io_service& serverService)
	: SocketBase(serverService, true),
	  mResolver(serverService)
{
}

Center::~Center()
{
}

void Center::SetCenterIndex(int idx)
{
	nCenterIndex = idx;
}

void Center::OnConnectToCenter(const std::string& strAddr, short nPort)
{
	asio::ip::tcp::resolver::query centerSrvQuery(strAddr, std::to_string(nPort)); 
	
	mResolver.async_resolve(centerSrvQuery,
		std::bind(&Center::OnResolve, std::dynamic_pointer_cast<Center>(shared_from_this()),
			std::placeholders::_1,
			std::placeholders::_2));
}

void Center::OnResolve(const std::error_code& err, asio::ip::tcp::resolver::iterator endpoint_iterator)
{
	if (!err)
	{
		asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
		GetSocket().async_connect(endpoint,
			std::bind(&Center::OnConnect, std::dynamic_pointer_cast<Center>(shared_from_this()),
				std::placeholders::_1, ++endpoint_iterator));
	}
	else
	{
	}
}

void Center::OnConnect(const std::error_code& err, asio::ip::tcp::resolver::iterator endpoint_iterator)
{
	if (err)
	{
		printf("[WvsLogin][Center::OnConnect]�L�k�s�u��Center Server�A�i��O�A�ȩ|���ҰʩΪ̽T�{�s�u��T�C\n");
		bConnectionFailed = true;
		OnDisconnect();
		return;
	}
	printf("[WvsLogin][Center::OnConnect]���\�s�u��Center Server�I\n");
	bIsConnected = true;

	//�VCenter Server�o�eHand Shake�ʥ]
	OutPacket oPacket;
	oPacket.Encode2(LoginPacketFlag::RegisterCenterRequest);

	//WvsLogin��ServerType��SVR_LOGIN
	oPacket.Encode1(ServerConstants::ServerType::SVR_LOGIN);

	SendPacket(&oPacket); 
	OnWaitingPacket();
}

void Center::OnPacket(InPacket *iPacket)
{
	printf("[WvsLogin][Center::OnPacket]�ʥ]�����G");
	iPacket->Print();
	int nType = (unsigned short)iPacket->Decode2();
	switch (nType)
	{
	case CenterPacketFlag::RegisterCenterAck:
	{
		auto result = iPacket->Decode1();
		if (!result)
		{
			printf("[WvsLogin][RegisterCenterAck][���~]Center Server�ڵ���eLocalServer�s���A�{���Y�N�פ�C\n");
			exit(0);
		}
		printf("[WvsLogin][RegisterCenterAck]Center Server �{�ҧ����A�P�@�ɦ��A���s�u���\�إߡC\n");
		OnUpdateWorldInfo(iPacket);
		break;
	}
	case CenterPacketFlag::CenterStatChanged:
		mWorldInfo.nGameCount = iPacket->Decode2();
		break;
	case CenterPacketFlag::CharacterListResponse:
		OnCharacterListResponse(iPacket);
		break;
	case CenterPacketFlag::CharacterCreateResponse:
		OnCharacterCreateResponse(iPacket);
		break;
	case CenterPacketFlag::GameServerInfoResponse:
		OnGameServerInfoResponse(iPacket);

		break;
	}
}

void Center::OnClosed()
{

}

void Center::OnUpdateWorldInfo(InPacket *iPacket)
{
	mWorldInfo.nWorldID = iPacket->Decode1();
	mWorldInfo.nEventType = iPacket->Decode1();
	mWorldInfo.strWorldDesc = iPacket->DecodeStr();
	mWorldInfo.strEventDesc = iPacket->DecodeStr();
	printf("[WvsLogin][Center::OnUpdateWorld]�C�����A���@�ɸ�T��s�C\n");
}

void Center::OnCharacterListResponse(InPacket *iPacket)
{
	int nLoginSocketID = iPacket->Decode4();
	auto pSocket = WvsBase::GetInstance<WvsLogin>()->GetSocketList()[nLoginSocketID];
	OutPacket oPacket;
	oPacket.Encode2(LoginPacketFlag::ClientSelectWorldResult);
	oPacket.Encode1(0);
	oPacket.EncodeStr("normal");
	oPacket.Encode4(0);
	oPacket.Encode1(0);
	oPacket.Encode4(0);
	oPacket.Encode8(0);
	oPacket.Encode1(0);

	printf("[WvsLogin][Center::OnCharacterListResponse]���a�֦�����M��ʥ] : ");
	iPacket->Print();
	printf("\n");

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
	oPacket.Encode8(-1);
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
	auto pSocket = WvsBase::GetInstance<WvsLogin>()->GetSocketList()[nLoginSocketID];
	OutPacket oPacket;
	oPacket.Encode2(LoginPacketFlag::ClientCreateNewCharacterResult);
	oPacket.Encode1(0);

	printf("[WvsLogin][Center::OnCharacterCreateResponse]���a�إߨ���ʥ] : ");
	iPacket->Print();
	printf("\n");

	oPacket.EncodeBuffer(iPacket->GetPacket() + 6, iPacket->GetPacketSize() - 6);

	pSocket->SendPacket(&oPacket);
}

void Center::OnGameServerInfoResponse(InPacket *iPacket)
{
	int nLoginSocketID = iPacket->Decode4();
	auto pSocket = WvsBase::GetInstance<WvsLogin>()->GetSocketList()[nLoginSocketID]; 
	OutPacket oPacket;
	oPacket.Encode2(LoginPacketFlag::ClientSelectCharacterResult);
	oPacket.EncodeBuffer(iPacket->GetPacket() + 6, iPacket->GetPacketSize() - 6);
	pSocket->SendPacket(&oPacket);
}