#pragma once

#include <string>
#include <vector>
#include "Poco\Data\Session.h"
#include "Poco\Data\RecordSet.h"

#define GET_DB_SESSION WvsUnified::GetInstance()->GetDBSession()
#define DB_DATE_19000101_23 WvsUnified::GetInstance()->ftDB_DATE_19000101_23()
#define DB_DATE_20790101_23 WvsUnified::GetInstance()->ftDB_DATE_20790101_23()
#define CURRENT_SYSTEM_TIME WvsUnified::GetInstance()->ftCurrentSystemTime()

class WvsUnified
{
private:
	Poco::Data::Session mDBSession;

public:
	typedef Poco::Data::RecordSet ResultType;

	WvsUnified();
	~WvsUnified();

	void InitDB();

	static WvsUnified* GetInstance()
	{
		static WvsUnified* pInstance = new WvsUnified();
		return pInstance;
	}

	Poco::Data::Session& GetDBSession()
	{
		return mDBSession;
	}

	static FILETIME ftDB_DATE_19000101_23()
	{
		FILETIME ft;
		ft.dwLowDateTime = 0x14F373B;
		ft.dwHighDateTime = 0x0FDE04000;
		return ft;
	}

	static FILETIME ftDB_DATE_20790101_23()
	{
		FILETIME ft;
		ft.dwLowDateTime = 0x217E646;
		ft.dwHighDateTime = 0x0BB058000;
		return ft;
	}

	static FILETIME ftCurrentSystemTime()
	{
		FILETIME ft;
		SYSTEMTIME st;
		GetSystemTime(&st); // gets current time
		SystemTimeToFileTime(&st, &ft);
		return ft;
	}

	ResultType GetCharacterIdList(int nAccountID, int nWorldID);
	ResultType LoadAvatar(int nCharacterID);
};