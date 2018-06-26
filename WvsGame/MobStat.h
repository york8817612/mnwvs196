#pragma once

class MobStat
{
private:

public:
	MobStat();
	~MobStat();

	class TS_Flag {
	private:
		static const int FLAG_COUNT = 3;
		int m_aData[FLAG_COUNT], m_nFlagPos;

	public:
		TS_Flag(int dwFlagValue);
		void Encode(OutPacket *oPacket);
		TS_Flag& operator |= (const TS_Flag& rhs);
		bool operator & (const TS_Flag& rhs);

		static TS_Flag GetDefault();
	};

	// ¥¿ªAªº
	enum MobStatNumbers
	{
		MOB_STAT_PAD = 0x0,
		MOB_STAT_PDR = 0x1,
		MOB_STAT_MAD = 0x2,
		MOB_STAT_MDR = 0x3,
		MOB_STAT_ACC = 0x4,
		MOB_STAT_EVA = 0x5,
		MOB_STAT_Speed = 0x6,
		MOB_STAT_Stun = 0x7,
		MOB_STAT_Freeze = 0x8,
		MOB_STAT_Poison = 0x9,
		MOB_STAT_Seal = 0xA,
		MOB_STAT_Darkness = 0xB,
		MOB_STAT_PowerUp = 0xC,
		MOB_STAT_MagicUp = 0xD,
		MOB_STAT_PGuardUp = 0xE,
		MOB_STAT_MGuardUp = 0xF,
		MOB_STAT_PImmune = 0x10,
		MOB_STAT_MImmune = 0x11,
		MOB_STAT_Web = 0x12,
		MOB_STAT_HardSkin = 0x13,
		MOB_STAT_Ambush = 0x14,
		MOB_STAT_Venom = 0x15,
		MOB_STAT_Blind = 0x16,
		MOB_STAT_SealSkill = 0x17,
		MOB_STAT_Dazzle = 0x18,
		MOB_STAT_PCounter = 0x19,
		MOB_STAT_MCounter = 0x1A,
		MOB_STAT_RiseByToss = 0x1B,
		MOB_STAT_BodyPressure = 0x1C,
		MOB_STAT_Weakness = 0x1D,
		MOB_STAT_Showdown = 0x1E,
		MOB_STAT_MagicCrash = 0x1F,
		MOB_STAT_DamagedElemAttr = 0x20,
		MOB_STAT_Dark = 0x21,
		MOB_STAT_Mystery = 0x22,
		MOB_STAT_AddDamParty = 0x23,
		MOB_STAT_HitCriDamR = 0x24,
		MOB_STAT_Fatality = 0x25,
		MOB_STAT_Lifting = 0x26,
		MOB_STAT_DeadlyCharge = 0x27,
		MOB_STAT_Smite = 0x28,
		MOB_STAT_AddDamSkill = 0x29,
		MOB_STAT_Incizing = 0x2A,
		MOB_STAT_DodgeBodyAttack = 0x2B,
		MOB_STAT_DebuffHealing = 0x2C,
		MOB_STAT_AddDamSkill2 = 0x2D,
		MOB_STAT_BodyAttack = 0x2E,
		MOB_STAT_TempMoveAbility = 0x2F,
		MOB_STAT_FixDamRBuff = 0x30,
		MOB_STAT_ElementDarkness = 0x31,
		MOB_STAT_AreaInstallByHit = 0x32,
		MOB_STAT_BMageDebuff = 0x33,
		MOB_STAT_JaguarProvoke = 0x34,
		MOB_STAT_JaguarBleeding = 0x35,
		MOB_STAT_DarkLightning = 0x36,
		MOB_STAT_PinkbeanFlowerPot = 0x37,
		MOB_STAT_BattlePvP_Helena_Mark = 0x38,
		MOB_STAT_PsychicLock = 0x39,
		MOB_STAT_PsychicLockCoolTime = 0x3A,
		MOB_STAT_PsychicGroundMark = 0x3B,
		MOB_STAT_PowerImmune = 0x3C,
		MOB_STAT_PsychicForce = 0x3D,
		MOB_STAT_MultiPMDR = 0x3E,
		MOB_STAT_ElementResetBySummon = 0x3F,
		MOB_STAT_BahamutLightElemAddDam = 0x40,
		MOB_STAT_BossPropPlus = 0x41,
		MOB_STAT_MultiDamSkill = 0x42,
		MOB_STAT_RWLiftPress = 0x43,
		MOB_STAT_RWChoppingHammer = 0x44,
		MOB_STAT_TimeBomb = 0x45,
		MOB_STAT_Treasure = 0x46,
		MOB_STAT_AddEffect = 0x47,
		MOB_STAT_Invincible = 0x48,
		MOB_STAT_Explosion = 0x49,
		MOB_STAT_HangOver = 0x4A,
		MOB_STAT_Burned = 0x4B,
		MOB_STAT_BalogDisable = 0x4C,
		MOB_STAT_ExchangeAttack = 0x4D,
		MOB_STAT_AddBuffStat = 0x4E,
		MOB_STAT_LinkTeam = 0x4F,
		MOB_STAT_SoulExplosion = 0x50,
		MOB_STAT_SeperateSoulP = 0x51,
		MOB_STAT_SeperateSoulC = 0x52,
		MOB_STAT_Ember = 0x53,
		MOB_STAT_TrueSight = 0x54,
		MOB_STAT_Unk = 0x55,
		MOB_STAT_Laser = 0x56,
		MOB_STAT_StatResetSkill = 0x57,
		MOB_STAT_COUNT = 0x58,
		MOB_STAT_NONE = 0xFFFFFFFF,
	};

	bool CHECK_MS_NORML(MobStat::TS_Flag & flag, MobStat::MobStatNumbers eMSN);
	void EncodeTemporary(OutPacket *oPacket, MobStat::TS_Flag & dwFlag, int tCur);

};
