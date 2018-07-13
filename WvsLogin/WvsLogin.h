#pragma once
#include "..\WvsLib\Net\WvsBase.h"
#include "..\WvsLib\Constants\WvsLoginConstants.hpp"
#include "Center.h"
#include <thread>
#include <mutex>
#include <condition_variable>

class WvsLogin : public WvsBase
{
private:
	void ConnectToCenter(int nCenterIdx);

	int m_nCenterCount = 0;
	std::shared_ptr<Center> m_apCenterInstance[ServerConstants::kMaxNumberOfCenters];
	std::shared_ptr<asio::io_service> aCenterServerService[ServerConstants::kMaxNumberOfCenters];
	std::shared_ptr<std::thread> m_apCenterWorkThread[ServerConstants::kMaxNumberOfCenters];

	//����Center instance�O�_���b�s�u�A�Ω��קK���s�����`
	bool aIsConnecting[ServerConstants::kMaxNumberOfCenters];
	void CenterAliveMonitor(int idx);

public:
	WvsLogin();
	~WvsLogin();

	int GetCenterCount() const;
	std::shared_ptr<Center>& GetCenter(int idx);
	void InitializeCenter();
	void OnNotifySocketDisconnected(SocketBase *pSocket);
};

