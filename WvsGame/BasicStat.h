#pragma once

struct GA_Character;
struct GW_ItemSlotBase;
class SecondaryStat;

class BasicStat
{
public:
	enum BasicStatFlag : long long int
	{
		BS_Skin = 0x01,
		BS_Face = 0x02,
		BS_Hair = 0x04,
		BS_Level = 0x10,
		BS_Job = 0x20,
		BS_STR = 0x40,
		BS_DEX = 0x80,
		BS_INT = 0x100,
		BS_LUK = 0x200,
		BS_HP = 0x400,
		BS_MaxHP = 0x800,
		BS_MP = 0x1000,
		BS_MaxMP = 0x2000,
		BS_AP = 0x4000,
		BS_SP = 0x8000,
		BS_EXP = 0x10000,
		BS_POP = 0x20000,
		BS_Meso = 0x40000,
		BS_Pet = 0x180008, //Unk
		BS_GachaponEXP = 0x200000,
		BS_Fatigue = 0x80000,
		BS_Charisma = 0x100000,
		BS_Insight = 0x200000,
		BS_Will = 0x400000,
		BS_Craft = 0x800000,
		BS_Sense = 0x1000000,
		BS_Charm = 0x2000000,
		BS_TrainLimit = 0x4000000
	};

	// ©x¤è¨ú±o
	enum CharacterStatFlag
	{
		CS_SKIN = 0x1,
		CS_FACE = 0x2,
		CS_HAIR = 0x4,
		CS_PETSN_UNUSED = 0x8,
		CS_LEV = 0x10,
		CS_JOB = 0x20,
		CS_STR = 0x40,
		CS_DEX = 0x80,
		CS_INT = 0x100,
		CS_LUK = 0x200,
		CS_HP = 0x400,
		CS_MHP = 0x800,
		CS_MP = 0x1000,
		CS_MMP = 0x2000,
		CS_AP = 0x4000,
		CS_SP = 0x8000,
		CS_EXP = 0x10000,
		CS_POP = 0x20000,
		CS_MONEY = 0x40000,
		CS_FATIGUE = 0x80000,
		CS_CHARISMAEXP = 0x100000,
		CS_INSIGHTEXP = 0x200000,
		CS_WILLEXP = 0x400000,
		CS_CRAFTEXP = 0x800000,
		CS_SENSEEXP = 0x1000000,
		CS_CHARMEXP = 0x2000000,
		CS_NCSLIMIT = 0x4000000,
		CS_ALBA = 0x8000000,
		CS_CHARCARD = 0x10000000,
		CS_PVP = 0x20000000,
		CS_PVPMODELEVEL = 0x40000000,
		CS_EVENT_POINT = 0x80000000,
	};

	class BasicStatRateOption
	{
	public:
		int nMMPr = 0,
			nMHPr = 0,
			nLUKr = 0,
			nINTr = 0,
			nDEXr = 0,
			nSTRr = 0;
	};

	BasicStatRateOption *m_bOption;

	int nGender = 0,
		nLevel = 0,
		nJob = 0,
		nSTR = 0,
		nINT = 0,
		nDEX = 0,
		nLUK = 0,
		nPOP = 0,
		nMHP = 0,
		nMMP = 0,
		nBasicStatInc = 0,
		nCharismaEXP = 0,
		nInsightEXP = 0,
		nWillEXP = 0,
		nSenseEXP = 0,
		nCharmEXP = 0,
		nIncMaxHPr = 0,
		nIncMaxMPr = 0,
		nPsdEVAX = 0,
		nIncDEXr = 0,
		nIncSTRr = 0,
		nIncINTr = 0,
		nIncLUKr = 0,
		nSpiritLinkIn = 0,
		nIncMaxHPr_Forced = 0,
		nIncRMAF = 0,
		nIncRMAI = 0,
		nIncRMAL = 0,
		nElemDefault = 0,
		nPDD,
		nMDD,
		nMAD,
		nPAD,
		nACC,
		nEVA,
		nSpeed,
		nJump,
		nCraft
		;

public:
	BasicStat();
	~BasicStat();

	virtual void SetFrom(int nFieldType, GA_Character* pChar, BasicStat* pSS, void *pFs, void* pNonBodyEquip, int nMHPForPvP, void* pPSD);
};

