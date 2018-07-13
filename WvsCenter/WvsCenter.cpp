#include "WvsCenter.h"

#include "..\WvsLib\Net\PacketFlags\CenterPacketFlags.hpp"
#include "..\WvsLib\Net\OutPacket.h"
#include "..\WvsLib\Net\InPacket.h"

#include "..\WvsLib\Constants\ServerConstants.hpp"

WvsCenter::WvsCenter()
{
}

WvsCenter::~WvsCenter()
{
}

void WvsCenter::OnNotifySocketDisconnected(SocketBase *pSocket)
{
	printf("[WvsCenter][WvsCenter::OnNotifySocketDisconnected]�W�D���A��[WvsGame]���_�s�u�A�i��WvsLogin�ܧ�C\n");
	if (pSocket->GetServerType() == ServerConstants::SRV_GAME)
	{
		auto iter = m_mChannel.begin();
		for (; iter != m_mChannel.end(); ++iter)
			if (iter->second->GetLocalSocket().get() == pSocket)
				break;
		if (iter != m_mChannel.end())
		{
			delete iter->second;
			m_mChannel.erase(iter);
		}
		//--nConnectedChannel;
		NotifyWorldChanged();
	}
	else if (pSocket->GetServerType() == ServerConstants::SRV_SHOP) 
	{
		delete m_pShopEntry;
		m_pShopEntry = nullptr;
	}
}

LocalServerEntry * WvsCenter::GetChannel(int idx)
{
	auto findIter = m_mChannel.find(idx);
	return findIter == m_mChannel.end() ? nullptr : findIter->second;
}

int WvsCenter::GetChannelCount()
{
	return (int)m_mChannel.size();
}

void WvsCenter::Init()
{
}

void WvsCenter::NotifyWorldChanged()
{
	auto& socketList = WvsBase::GetInstance<WvsCenter>()->GetSocketList();
	for (const auto& socket : socketList)
	{
		if (socket.second->GetServerType() == ServerConstants::SRV_LOGIN)
		{
			//printf("On Notify World Changed\n");
			OutPacket oPacket;
			oPacket.Encode2(CenterSendPacketFlag::CenterStatChanged);
			oPacket.Encode2(WvsBase::GetInstance<WvsCenter>()->GetChannelCount());
			for (auto& pEntry : m_mChannel)
				oPacket.Encode1(pEntry.first);
			socket.second->SendPacket(&oPacket);
		}
	}
}

LocalServerEntry * WvsCenter::GetShop()
{
	return m_pShopEntry;
}

void WvsCenter::SetShop(LocalServerEntry * pEntry)
{
	m_pShopEntry = pEntry;
}

void WvsCenter::RegisterChannel(std::shared_ptr<SocketBase> &pServer, InPacket *iPacket)
{
	int nChannelID = iPacket->Decode1();
	LocalServerEntry *pEntry = new LocalServerEntry;

	pEntry->SetLocalSocket(pServer);
	pEntry->SetExternalIP(iPacket->Decode4());
	pEntry->SetExternalPort(iPacket->Decode2());
	printf("[WvsCenter][WvsCenter::RegisterChannel]�s���W�D���A��[Channel ID = %d][WvsGame]���U���\�AIP : ", nChannelID);
	auto ip = pEntry->GetExternalIP();
	for (int i = 0; i < 4; ++i)
		printf("%d ", (int)((char*)&ip)[i]);
	printf("\n Port = %d\n", pEntry->GetExternalPort());
	m_mChannel.insert({ nChannelID, pEntry });
}

void WvsCenter::RegisterCashShop(std::shared_ptr<SocketBase>& pServer, InPacket * iPacket)
{
	LocalServerEntry *pEntry = new LocalServerEntry;

	pEntry->SetLocalSocket(pServer);
	pEntry->SetExternalIP(iPacket->Decode4());
	pEntry->SetExternalPort(iPacket->Decode2());
	printf("[WvsCenter][WvsCenter::RegisterCashShop]�s���ӫ����A��[WvsShop]���U���\�C\n");

	SetShop(pEntry);
}
