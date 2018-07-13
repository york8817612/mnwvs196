#include "WvsShop.h"
#include <thread>
#include <iostream>
#include <functional>
#include "User.h"
#include "ClientSocket.h"
#include "..\WvsLib\Net\OutPacket.h"
#include "..\WvsLib\Task\AsnycScheduler.h"
#include "..\WvsLib\Logger\WvsLogger.h"
#include "..\WvsLib\Constants\ConfigLoader.hpp"
#include "..\WvsLib\Constants\ServerConstants.hpp"

WvsShop::WvsShop()
{
}


WvsShop::~WvsShop()
{
}

std::shared_ptr<Center>& WvsShop::GetCenter()
{
	return m_pCenterInstance;
}

void WvsShop::ConnectToCenter()
{
	int nCenterIdx = 0;
	m_pCenterInstance->SetSocketDisconnectedCallBack(std::bind(&Center::OnNotifyCenterDisconnected, m_pCenterInstance.get()));
	m_pCenterInstance->SetCenterIndex(nCenterIdx);
	m_pCenterInstance->Connect(
		ConfigLoader::GetInstance()->StrValue("Center" + std::to_string(nCenterIdx) + "_IP"),
		ConfigLoader::GetInstance()->IntValue("Center" + std::to_string(nCenterIdx) + "_Port")
	);

}

void WvsShop::CenterAliveMonitor()
{
	if (m_pCenterInstance->CheckSocketStatus(SocketBase::SocketStatus::eConnecting))
		return;
	WvsLogger::LogRaw(WvsLogger::LEVEL_WARNING, "=================�w���ˬdCenter Server�s�u�{��=================\n");
	int centerSize = ConfigLoader::GetInstance()->IntValue("CenterCount");
	if (m_pCenterInstance && m_pCenterInstance->CheckSocketStatus(SocketBase::SocketStatus::eClosed))
	{
		WvsLogger::LogFormat("Center Server %d �s�u���ѡA���խ��s�s�u�C\n", 0);
		m_pCenterWorkThread->detach();
		*m_pCenterWorkThread = (std::thread(&WvsShop::ConnectToCenter, this));
	}
}

void WvsShop::InitializeCenter()
{
	m_pCenterServerService = (new asio::io_service());
	m_pCenterInstance = std::make_shared<Center>(*m_pCenterServerService);
	m_pCenterWorkThread = (new std::thread(&WvsShop::ConnectToCenter, this));
	auto fHolderFunc = std::bind(&WvsShop::CenterAliveMonitor, this);
	auto pAliveHolder = AsnycScheduler::CreateTask(fHolderFunc, 10 * 100, true);
	pAliveHolder->Start();


	std::thread tCenterWorkThread([&] {
		asio::io_service::work work(*m_pCenterServerService);
		std::error_code ec;
		m_pCenterServerService->run(ec);
	});
	tCenterWorkThread.detach();
}

void WvsShop::OnUserConnected(std::shared_ptr<User>& pUser)
{
	std::lock_guard<std::mutex> lockGuard(m_mUserLock);
	m_mUserMap[pUser->GetUserID()] = pUser;
}

void WvsShop::OnNotifySocketDisconnected(SocketBase * pSocket)
{
	std::lock_guard<std::mutex> lockGuard(m_mUserLock);
	auto pClient = (ClientSocket*)pSocket;
	if (pClient->GetUser())
	{
		m_mUserMap.erase(pClient->GetUser()->GetUserID());
		pClient->SetUser(nullptr);
	}
}