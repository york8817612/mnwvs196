#pragma once
#include "..\WvsLib\Net\SocketBase.h"

//Server �ݤ� Session
class LocalServer : public SocketBase
{
private:
	void OnClosed();

public:
	LocalServer(asio::io_service& serverService);
	~LocalServer();

	void OnPacket(InPacket *iPacket);


	void OnRegisterCenterRequest(InPacket *iPacket);
	void OnRequestCharacterList(InPacket *iPacket);
	void OnRequestCreateNewCharacter(InPacket *iPacket);
	void OnRequestGameServerInfo(InPacket *iPacket);
	void OnRequestMigrateIn(InPacket *iPacket);
	void OnRequestMigrateOut(InPacket *iPacket);
	void OnRequestTransferChannel(InPacket *iPacket);
	void OnRequestMigrateCashShop(InPacket *iPacket);
};

