#pragma once

#include "wzmain.hpp"
#include "..\Memory\MemoryPoolMan.hpp"
#include "..\Constants\ConfigLoader.hpp"
#include "..\Constants\ServerConstants.hpp"

#define MAPPING_WZ_NAME(name) mWzNode[(int)Wz::name] = WZ::Base[#name]

/*
Wz�ɮפ�Tag

By Wanger.
*/
enum class Wz
{
	Base,
	Character,
	Effect,
	Etc,
	Item,
	Map,
	Map2,
	Mob,
	Mob2,
	Morph,
	Npc,
	Quest,
	Reactor,
	Skill,
	String,
	TamingMob,
	UI,
};

/*
Wz�귽�s������

By Wanger.
*/
class WzResMan
{
private:
	WZ::Node mWzNode[(int)Wz::UI + 1];

	WzResMan() 
	{
		ConfigLoader::GetInstance()->LoadConfig("GlobalSetting.txt");
		WZ::AddPath(ConfigLoader::GetInstance()->StrValue("WzDir"));
		Init();
	}

	void Init()
	{
		WZ::Init(true);
		MAPPING_WZ_NAME(Base);
		MAPPING_WZ_NAME(Character);
		MAPPING_WZ_NAME(Effect);
		MAPPING_WZ_NAME(Etc);
		MAPPING_WZ_NAME(Item);
		MAPPING_WZ_NAME(Map);
		MAPPING_WZ_NAME(Map2);
		MAPPING_WZ_NAME(Mob);
		MAPPING_WZ_NAME(Mob2);
		MAPPING_WZ_NAME(Morph);
		MAPPING_WZ_NAME(Npc);
		MAPPING_WZ_NAME(Quest);
		MAPPING_WZ_NAME(Reactor);
		MAPPING_WZ_NAME(Skill);
		MAPPING_WZ_NAME(String);
		MAPPING_WZ_NAME(TamingMob);
		MAPPING_WZ_NAME(UI);
	}

public:

	static WzResMan* GetInstance()
	{
		static WzResMan *sWzResMan = new WzResMan();
		return sWzResMan;
		//return nullptr;
	}

	WZ::Node& GetWz(Wz wzTag)
	{
		return mWzNode[(int)wzTag];
	}

	void ReleaseMemory()
	{
		WzMemoryPoolMan::GetInstance()->Release();
		ConfigLoader::GetInstance()->LoadConfig("GlobalSetting.txt");
		WZ::AddPath(ConfigLoader::GetInstance()->StrValue("WzDir"));
		Init();
	}
};

extern WzResMan *stWzResMan;