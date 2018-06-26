#pragma once
#include <map>
#include <functional>
#include "User.h"
#include "Utility\Task\AsnycScheduler.h"

class LifePool;
class Mob;
class Portal;
class PortalMap;
class TownPortalPool;
class DropPool;

class Field
{
	std::mutex fieldUserMutex;
	std::map<int, User*> m_mUser; //m_lUser in WvsGame.idb
	int m_nFieldID = 0;
	LifePool *m_pLifePool;
	PortalMap *m_pPortalMap;
	DropPool *m_pDropPool;
	TownPortalPool *m_pTownPortalPool;

	std::string m_sStreetName, 
				m_sMapName, 
				m_sWeatherMsg; //商城心情道具訊息?

	int m_nWeatherItemID, //商城心情道具物品ID?
		m_nJukeBoxItemID; //Juke Box?

	double m_dIncRate_EXP = 1.0, 
		   m_dIncRate_Drop = 1.0; //加倍?

	bool m_bCloud, 
		 m_bTown, 
		 m_bSwim, 
		 m_bFly;

	int m_nReturnMap, 
		m_nForcedReturn, 
		m_nMobRate, 
		m_nFieldType, 
		m_nFieldLimit, 
		m_nCreateMobInterval, 
		m_nFixedMobCapacity, 
		m_nMapSizeX, 
		m_nMapSizeY;

	std::string m_strFirstUserEnter, 
				m_strUserEnter;

	static void UpdateTrigger(Field* pField)
	{
		pField->Update();
	}

	//std::_Binder<std::_Unforced, void(*)(Field* pField), Field* const> m_updateBinder;
	std::function<void(Field*)> m_updateBinder;
	AsnycScheduler::AsnycScheduler<decltype(m_updateBinder)>* m_asyncUpdateTimer;

public:
	Field();
	~Field();

	// 正服取得 SP_FieldEffect
	enum FieldEffectNumbers
	{
		FieldEffect_Summon = 0x0,
		FieldEffect_Tremble = 0x1,
		FieldEffect_Object = 0x2,
		FieldEffect_Object_Disable = 0x3,
		FieldEffect_Screen = 0x4,
		FieldEffect_Sound = 0x5,
		FieldEffect_MobHPTag = 0x6,
		FieldEffect_ChangeBGM = 0x7,
		FieldEffect_BGMVolumeOnly = 0x8,
		FieldEffect_BGMVolume = 0x9,
		FieldEffect_RewordRullet = 0xA,
		FieldEffect_TopScreen = 0xB,
		FieldEffect_Screen_Delayed = 0xC,
		FieldEffect_TopScreen_Delayed = 0xD,
		FieldEffect_Screen_AutoLetterBox = 0xE,
		FieldEffect_FloatingUI = 0xF,
		FieldEffect_Blind = 0x10,
		FieldEffect_GrayScale = 0x11,
		FieldEffect_OnOffLayer = 0x12,
		FieldEffect_Overlap = 0x13,
		FieldEffect_Overlap_Detail = 0x14,
		FieldEffect_Remove_Overlap_Detail = 0x15,
		FieldEffect_ColorChange = 0x16,
		FieldEffect_StageClear = 0x17,
		FieldEffect_TopScreen_WithOrigin = 0x18,
		FieldEffect_SpineScreen = 0x19,
		FieldEffect_OffSpineScreen = 0x1A,
	};

	void BroadcastPacket(OutPacket* oPacket);

	void SetCould(bool cloud);
	bool IsCloud() const;

	void SetTown(bool town);
	bool IsTown() const;

	void SetSwim(bool swim);
	bool IsSwim() const;

	void SetFly(bool fly);
	bool IsFly() const;

	void SetFieldID(int nFieldID);
	int GetFieldID() const;

	void SetReturnMap(int returnMap);
	int GetReturnMap() const;

	void SetForcedReturn(int forcedReturn);
	int GetForcedReturn() const;

	void SetMobRate(int mobRate);
	int GetMobRate() const;

	void SetFieldType(int fieldType);
	int GetFieldType() const;

	void SetFieldLimit(int fieldLimit);
	int GetFieldLimit() const;

	void SetCreateMobInterval(int interval);
	int GetCreateMobInterval() const;

	void SetFiexdMobCapacity(int capacity);
	int GetFixedMobCapacity() const;

	void SetFirstUserEnter(const std::string& script);
	const std::string& GetFirstUserEnter() const;

	void SetUserEnter(const std::string& script);
	const std::string& GetUserEnter() const;

	void SetMapSizeX(int x);
	int GetMapSizeX();

	void SetMapSizeY(int y);
	int GetMapSizeY();

	void InitLifePool();

	LifePool *GetLifePool();
	DropPool *GetDropPool();

	void OnEnter(User *pUser);
	void OnLeave(User *pUser);

	//發送oPacket給該地圖的其他User，其中pExcept是例外對象
	void SplitSendPacket(OutPacket* oPacket, User* pExcept);

	void OnMobMove(User* pCtrl, Mob* pMob, InPacket* iPacket);
	//不同Field 自行處理
	virtual void OnPacket(User* pUser, InPacket* iPacket);
	void OnUserMove(User* pUser, InPacket *iPacket);

	PortalMap* GetPortalMap();
	TownPortalPool* GetTownPortalPool();

	void Update();
};

