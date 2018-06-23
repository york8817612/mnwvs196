#pragma once
#include "Net\SocketBase.h"
#include "WvsLoginConstants.hpp"
#include "Net\WvsBase.h"

class Center :
	public SocketBase
{
private:

	int nCenterIndex;
	bool bIsConnected = false, bConnectionFailed = false;
	asio::ip::tcp::resolver mResolver;

	WorldInfo mWorldInfo;

	void OnResolve(const std::error_code& err, asio::ip::tcp::resolver::iterator endpoint_iterator);
	void OnConnect(const std::error_code& err, asio::ip::tcp::resolver::iterator endpoint_iterator);

public:
	Center(asio::io_service& serverService);
	~Center();

	void SetCenterIndex(int idx);

	void OnConnectToCenter(const std::string& strAddr, short nPort);
	void OnPacket(InPacket *iPacket);
	void OnClosed();

	const WorldInfo& GetWorldInfo()
	{
		return mWorldInfo;
	}

	bool IsConnectionFailed() const
	{
		return bConnectionFailed;
	}

	bool IsConnected() const
	{
		return bIsConnected;
	}

	void OnUpdateWorldInfo(InPacket *iPacket);
	void OnCharacterListResponse(InPacket *iPacket);
	void Center::OnCharacterCreateResponse(InPacket *iPacket);
	void OnGameServerInfoResponse(InPacket *iPacket);

	static void OnNotifyCenterDisconnected(SocketBase *pSocket)
	{
		printf("[WvsLogin][Center]�PCenter Server���_�s�u�C\n");
		((Center*)pSocket)->bIsConnected = false;
		((Center*)pSocket)->bConnectionFailed = true;
	}
};

