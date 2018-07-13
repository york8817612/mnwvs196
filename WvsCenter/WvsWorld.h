#pragma once
#include <vector>
#include <map>
#include <memory>
#include "..\WvsLib\Net\WorldInfo.h"
#include "..\WvsLib\Constants\ConfigLoader.hpp"

class UserTransferStatus;

class WvsWorld
{
	WorldInfo m_WorldInfo;
	std::map<int, std::shared_ptr<UserTransferStatus>> m_mUserTransferStatus;
public:
	WvsWorld();
	~WvsWorld();
	void InitializeWorld();
	const WorldInfo& GetWorldInfo() const;

	static WvsWorld* GetInstance()
	{
		static WvsWorld* pInstance = new WvsWorld;
		return pInstance;
	}


	void SetUserTransferStatus(int nUserID, UserTransferStatus* pStatus);
	const UserTransferStatus* GetUserTransferStatus(int nUserID) const;
	void ClearUserTransferStatus(int nUserID);
};

