#pragma once
#include "WvsUnified.h"

class SocketBase;

class CharacterDBAccessor
{
private:
	static WvsUnified mDBUnified;

public:
	enum NewCharacterFlag
	{
		EQP_POS_Hat = -1,
		EQP_POS_Top = -5,
		EQP_POS_Bottom = -6,
		EQP_POS_Shoes = -7,
		EQP_POS_Weapon = -11,
		EQP_POS_Cape = -9,
		EQP_POS_Shield = -10,

		EQP_ID_HatEquip = 0,
		EQP_ID_TopEquip = 1,
		EQP_ID_ButtomEquip = 2,
		EQP_ID_ShoesEquip = 3,
		EQP_ID_WeaponEquip = 4,
		EQP_ID_CapeEquip = 5,
		EQP_ID_ShieldEquip = 6,
		EQP_ID_FLAG_END = 7,

		STAT_Str = 0,
		STAT_Dex = 1,
		STAT_Int = 2,
		STAT_Luk = 3,
		STAT_HP = 4,
		STAT_MP = 5,
		STAT_MaxHP = 6,
		STAT_MaxMP = 7,
		STAT_SP = 8,
		STAT_AP = 9,
		STAT_Job = 10,
		STAT_SubJob = 11,
		STAT_Level = 12,
		STAT_FLAG_END = 13,
	};

	CharacterDBAccessor();
	~CharacterDBAccessor();

	static CharacterDBAccessor* GetInstance()
	{
		static CharacterDBAccessor* sPtrAccessor = new CharacterDBAccessor();
		return sPtrAccessor;
	}

	static void GetDefaultCharacterStat(int *aStat);

	void PostLoadCharacterListRequest(SocketBase *pSrv, int uLocalSocketSN, int nAccountID, int nWorldID);
	void PostCreateNewCharacterRequest(SocketBase *pSrv, int uLocalSocketSN, int nAccountID, int nWorldID, const std::string& strName, int nGender, int nFace, int nHair, int nSkin, const int* aBody, const int* aStat);
	void PostCharacterDataRequest(SocketBase *pSrv, int uClientSocketSN, int nCharacterID, void *oPacket);
	void OnCharacterSaveRequest(void *iPacket);
};

