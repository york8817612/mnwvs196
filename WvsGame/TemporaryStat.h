#pragma once

class OutPacket;

#define ADD_TS_FLAG(flag, value) static const int TS_##flag = value;
#define GET_TS_FLAG(flagName) TemporaryStat::TS_Flag(TemporaryStat::TS_##flagName)

struct TemporaryStat
{
	class TS_Flag {
	private:
		static const int FLAG_COUNT = 17;
		int m_aData[FLAG_COUNT], m_nFlagPos;

	public:
		TS_Flag(int dwFlagValue);
		void Encode(OutPacket *oPacket);
		TS_Flag& operator |= (const TS_Flag& rhs);
		bool operator & (const TS_Flag& rhs);

		static TS_Flag GetDefault();
	};

	// 正服取得SecondaryStatNumbers
	/*enum SecondaryStatNumbers
	{
		SECONDARY_STAT_IndiePAD = 0x0,
		SECONDARY_STAT_IndieMAD = 0x1,
		SECONDARY_STAT_IndiePDD = 0x2,
		SECONDARY_STAT_IndieMDD = 0x3,
		SECONDARY_STAT_IndieMHP = 0x4,
		SECONDARY_STAT_IndieMHPR = 0x5,
		SECONDARY_STAT_IndieMMP = 0x6,
		SECONDARY_STAT_IndieMMPR = 0x7,
		SECONDARY_STAT_IndieACC = 0x8,
		SECONDARY_STAT_IndieEVA = 0x9,
		SECONDARY_STAT_IndieJump = 0xA,
		SECONDARY_STAT_IndieSpeed = 0xB,
		SECONDARY_STAT_IndieAllStat = 0xC,
		SECONDARY_STAT_IndieDodgeCriticalTime = 0xD,
		SECONDARY_STAT_IndieEXP = 0xE,
		SECONDARY_STAT_IndieBooster = 0xF,
		SECONDARY_STAT_IndieFixedDamageR = 0x10,
		SECONDARY_STAT_PyramidStunBuff = 0x11,
		SECONDARY_STAT_PyramidFrozenBuff = 0x12,
		SECONDARY_STAT_PyramidFireBuff = 0x13,
		SECONDARY_STAT_PyramidBonusDamageBuff = 0x14,
		SECONDARY_STAT_IndieRelaxEXP = 0x15,
		SECONDARY_STAT_IndieSTR = 0x16,
		SECONDARY_STAT_IndieDEX = 0x17,
		SECONDARY_STAT_IndieINT = 0x18,
		SECONDARY_STAT_IndieLUK = 0x19,
		SECONDARY_STAT_IndieDamR = 0x1A,
		SECONDARY_STAT_IndieScriptBuff = 0x1B,
		SECONDARY_STAT_IndieMDF = 0x1C,
		SECONDARY_STAT_IndieMaxDamageOver = 0x1D,
		SECONDARY_STAT_IndieAsrR = 0x1E,
		SECONDARY_STAT_IndieTerR = 0x1F,
		SECONDARY_STAT_IndieCr = 0x20,
		SECONDARY_STAT_IndiePDDR = 0x21,
		SECONDARY_STAT_IndieCrMax = 0x22,
		SECONDARY_STAT_IndieBDR = 0x23,
		SECONDARY_STAT_IndieStatR = 0x24,
		SECONDARY_STAT_IndieStance = 0x25,
		SECONDARY_STAT_IndieIgnoreMobpdpR = 0x26,
		SECONDARY_STAT_IndieEmpty = 0x27,
		SECONDARY_STAT_IndiePADR = 0x28,
		SECONDARY_STAT_IndieMADR = 0x29,
		SECONDARY_STAT_IndieCrMaxR = 0x2A,
		SECONDARY_STAT_IndieEVAR = 0x2B,
		SECONDARY_STAT_IndieMDDR = 0x2C,
		SECONDARY_STAT_IndieDrainHP = 0x2D,
		SECONDARY_STAT_IndiePMdR = 0x2E,
		SECONDARY_STAT_IndieMaxDamageOverR = 0x2F,
		SECONDARY_STAT_IndieForceJump = 0x30,
		SECONDARY_STAT_IndieForceSpeed = 0x31,
		SECONDARY_STAT_IndieQrPointTerm = 0x32,
		INDIE_STAT_COUNT = 0x33,
		SECONDARY_STAT_PAD = 0x34,
		SECONDARY_STAT_PDD = 0x35,
		SECONDARY_STAT_MAD = 0x36,
		SECONDARY_STAT_MDD = 0x37,
		SECONDARY_STAT_ACC = 0x38,
		SECONDARY_STAT_EVA = 0x39,
		SECONDARY_STAT_Craft = 0x3A,
		SECONDARY_STAT_Speed = 0x3B,
		SECONDARY_STAT_Jump = 0x3C,
		SECONDARY_STAT_MagicGuard = 0x3D,
		SECONDARY_STAT_DarkSight = 0x3E,
		SECONDARY_STAT_Booster = 0x3F,
		SECONDARY_STAT_PowerGuard = 0x40,
		SECONDARY_STAT_MaxHP = 0x41,
		SECONDARY_STAT_MaxMP = 0x42,
		SECONDARY_STAT_Invincible = 0x43,
		SECONDARY_STAT_SoulArrow = 0x44,
		SECONDARY_STAT_Stun = 0x45,
		SECONDARY_STAT_Poison = 0x46,
		SECONDARY_STAT_Seal = 0x47,
		SECONDARY_STAT_Darkness = 0x48,
		SECONDARY_STAT_ComboCounter = 0x49,
		SECONDARY_STAT_WeaponCharge = 0x4A,
		SECONDARY_STAT_HolySymbol = 0x4B,
		SECONDARY_STAT_MesoUp = 0x4C,
		SECONDARY_STAT_ShadowPartner = 0x4D,
		SECONDARY_STAT_PickPocket = 0x4E,
		SECONDARY_STAT_MesoGuard = 0x4F,
		SECONDARY_STAT_Thaw = 0x50,
		SECONDARY_STAT_Weakness = 0x51,
		SECONDARY_STAT_Curse = 0x52,
		SECONDARY_STAT_Slow = 0x53,
		SECONDARY_STAT_Morph = 0x54,
		SECONDARY_STAT_Regen = 0x55,
		SECONDARY_STAT_BasicStatUp = 0x56,
		SECONDARY_STAT_Stance = 0x57,
		SECONDARY_STAT_SharpEyes = 0x58,
		SECONDARY_STAT_ManaReflection = 0x59,
		SECONDARY_STAT_Attract = 0x5A,
		SECONDARY_STAT_NoBulletConsume = 0x5B,
		SECONDARY_STAT_Infinity = 0x5C,
		SECONDARY_STAT_AdvancedBless = 0x5D,
		SECONDARY_STAT_IllusionStep = 0x5E,
		SECONDARY_STAT_Blind = 0x5F,
		SECONDARY_STAT_Concentration = 0x60,
		SECONDARY_STAT_BanMap = 0x61,
		SECONDARY_STAT_MaxLevelBuff = 0x62,
		SECONDARY_STAT_MesoUpByItem = 0x63,
		SECONDARY_STAT_Ghost = 0x64,
		SECONDARY_STAT_Barrier = 0x65,
		SECONDARY_STAT_ReverseInput = 0x66,
		SECONDARY_STAT_ItemUpByItem = 0x67,
		SECONDARY_STAT_RespectPImmune = 0x68,
		SECONDARY_STAT_RespectMImmune = 0x69,
		SECONDARY_STAT_DefenseAtt = 0x6A,
		SECONDARY_STAT_DefenseState = 0x6B,
		SECONDARY_STAT_DojangBerserk = 0x6C,
		SECONDARY_STAT_DojangInvincible = 0x6D,
		SECONDARY_STAT_DojangShield = 0x6E,
		SECONDARY_STAT_SoulMasterFinal = 0x6F,
		SECONDARY_STAT_WindBreakerFinal = 0x70,
		SECONDARY_STAT_ElementalReset = 0x71,
		SECONDARY_STAT_HideAttack = 0x72,
		SECONDARY_STAT_EventRate = 0x73,
		SECONDARY_STAT_ComboAbilityBuff = 0x74,
		SECONDARY_STAT_ComboDrain = 0x75,
		SECONDARY_STAT_ComboBarrier = 0x76,
		SECONDARY_STAT_BodyPressure = 0x77,
		SECONDARY_STAT_RepeatEffect = 0x78,
		SECONDARY_STAT_ExpBuffRate = 0x79,
		SECONDARY_STAT_StopPortion = 0x7A,
		SECONDARY_STAT_StopMotion = 0x7B,
		SECONDARY_STAT_Fear = 0x7C,
		SECONDARY_STAT_HiddenPieceOn = 0x7D,
		SECONDARY_STAT_MagicShield = 0x7E,
		SECONDARY_STAT_MagicResistance = 0x7F,
		SECONDARY_STAT_SoulStone = 0x80,
		SECONDARY_STAT_Flying = 0x81,
		SECONDARY_STAT_Frozen = 0x82,
		SECONDARY_STAT_AssistCharge = 0x83,
		SECONDARY_STAT_Enrage = 0x84,
		SECONDARY_STAT_DrawBack = 0x85,
		SECONDARY_STAT_NotDamaged = 0x86,
		SECONDARY_STAT_FinalCut = 0x87,
		SECONDARY_STAT_HowlingAttackDamage = 0x88,
		SECONDARY_STAT_BeastFormDamageUp = 0x89,
		SECONDARY_STAT_Dance = 0x8A,
		SECONDARY_STAT_EMHP = 0x8B,
		SECONDARY_STAT_EMMP = 0x8C,
		SECONDARY_STAT_EPAD = 0x8D,
		SECONDARY_STAT_EMAD = 0x8E,
		SECONDARY_STAT_EPDD = 0x8F,
		SECONDARY_STAT_EMDD = 0x90,
		SECONDARY_STAT_Guard = 0x91,
		SECONDARY_STAT_Cyclone = 0x92,
		SECONDARY_STAT_HowlingCritical = 0x93,
		SECONDARY_STAT_HowlingMaxMP = 0x94,
		SECONDARY_STAT_HowlingDefence = 0x95,
		SECONDARY_STAT_HowlingEvasion = 0x96,
		SECONDARY_STAT_Conversion = 0x97,
		SECONDARY_STAT_Revive = 0x98,
		SECONDARY_STAT_PinkbeanMinibeenMove = 0x99,
		SECONDARY_STAT_Sneak = 0x9A,
		SECONDARY_STAT_Mechanic = 0x9B,
		SECONDARY_STAT_BeastFormMaxHP = 0x9C,
		SECONDARY_STAT_Dice = 0x9D,
		SECONDARY_STAT_BlessingArmor = 0x9E,
		SECONDARY_STAT_DamR = 0x9F,
		SECONDARY_STAT_TeleportMasteryOn = 0xA0,
		SECONDARY_STAT_CombatOrders = 0xA1,
		SECONDARY_STAT_Beholder = 0xA2,
		SECONDARY_STAT_DispelItemOption = 0xA3,
		SECONDARY_STAT_Inflation = 0xA4,
		SECONDARY_STAT_OnixDivineProtection = 0xA5,
		SECONDARY_STAT_Web = 0xA6,
		SECONDARY_STAT_Bless = 0xA7,
		SECONDARY_STAT_TimeBomb = 0xA8,
		SECONDARY_STAT_DisOrder = 0xA9,
		SECONDARY_STAT_Thread = 0xAA,
		SECONDARY_STAT_Team = 0xAB,
		SECONDARY_STAT_Explosion = 0xAC,
		SECONDARY_STAT_BuffLimit = 0xAD,
		SECONDARY_STAT_STR = 0xAE,
		SECONDARY_STAT_INT = 0xAF,
		SECONDARY_STAT_DEX = 0xB0,
		SECONDARY_STAT_LUK = 0xB1,
		SECONDARY_STAT_DispelItemOptionByField = 0xB2,
		SECONDARY_STAT_DarkTornado = 0xB3,
		SECONDARY_STAT_PVPDamage = 0xB4,
		SECONDARY_STAT_PvPScoreBonus = 0xB5,
		SECONDARY_STAT_PvPInvincible = 0xB6,
		SECONDARY_STAT_PvPRaceEffect = 0xB7,
		SECONDARY_STAT_WeaknessMdamage = 0xB8,
		SECONDARY_STAT_Frozen2 = 0xB9,
		SECONDARY_STAT_PVPDamageSkill = 0xBA,
		SECONDARY_STAT_AmplifyDamage = 0xBB,
		SECONDARY_STAT_IceKnight = 0xBC,
		SECONDARY_STAT_Shock = 0xBD,
		SECONDARY_STAT_InfinityForce = 0xBE,
		SECONDARY_STAT_IncMaxHP = 0xBF,
		SECONDARY_STAT_IncMaxMP = 0xC0,
		SECONDARY_STAT_HolyMagicShell = 0xC1,
		SECONDARY_STAT_KeyDownTimeIgnore = 0xC2,
		SECONDARY_STAT_ArcaneAim = 0xC3,
		SECONDARY_STAT_MasterMagicOn = 0xC4,
		SECONDARY_STAT_AsrR = 0xC5,
		SECONDARY_STAT_TerR = 0xC6,
		SECONDARY_STAT_DamAbsorbShield = 0xC7,
		SECONDARY_STAT_DevilishPower = 0xC8,
		SECONDARY_STAT_Roulette = 0xC9,
		SECONDARY_STAT_SpiritLink = 0xCA,
		SECONDARY_STAT_AsrRByItem = 0xCB,
		SECONDARY_STAT_Event = 0xCC,
		SECONDARY_STAT_CriticalBuff = 0xCD,
		SECONDARY_STAT_DropRate = 0xCE,
		SECONDARY_STAT_PlusExpRate = 0xCF,
		SECONDARY_STAT_ItemInvincible = 0xD0,
		SECONDARY_STAT_Awake = 0xD1,
		SECONDARY_STAT_ItemCritical = 0xD2,
		SECONDARY_STAT_ItemEvade = 0xD3,
		SECONDARY_STAT_Event2 = 0xD4,
		SECONDARY_STAT_VampiricTouch = 0xD5,
		SECONDARY_STAT_DDR = 0xD6,
		SECONDARY_STAT_IncCriticalDamMin = 0xD7,
		SECONDARY_STAT_IncCriticalDamMax = 0xD8,
		SECONDARY_STAT_IncTerR = 0xD9,
		SECONDARY_STAT_IncAsrR = 0xDA,
		SECONDARY_STAT_DeathMark = 0xDB,
		SECONDARY_STAT_UsefulAdvancedBless = 0xDC,
		SECONDARY_STAT_Lapidification = 0xDD,
		SECONDARY_STAT_VenomSnake = 0xDE,
		SECONDARY_STAT_CarnivalAttack = 0xDF,
		SECONDARY_STAT_CarnivalDefence = 0xE0,
		SECONDARY_STAT_CarnivalExp = 0xE1,
		SECONDARY_STAT_SlowAttack = 0xE2,
		SECONDARY_STAT_PyramidEffect = 0xE3,
		SECONDARY_STAT_KillingPoint = 0xE4,
		SECONDARY_STAT_HollowPointBullet = 0xE5,
		SECONDARY_STAT_KeyDownMoving = 0xE6,
		SECONDARY_STAT_IgnoreTargetDEF = 0xE7,
		SECONDARY_STAT_ReviveOnce = 0xE8,
		SECONDARY_STAT_Invisible = 0xE9,
		SECONDARY_STAT_EnrageCr = 0xEA,
		SECONDARY_STAT_EnrageCrDamMin = 0xEB,
		SECONDARY_STAT_Judgement = 0xEC,
		SECONDARY_STAT_DojangLuckyBonus = 0xED,
		SECONDARY_STAT_PainMark = 0xEE,
		SECONDARY_STAT_Magnet = 0xEF,
		SECONDARY_STAT_MagnetArea = 0xF0,
		SECONDARY_STAT_VampDeath = 0xF1,
		SECONDARY_STAT_BlessingArmorIncPAD = 0xF2,
		SECONDARY_STAT_KeyDownAreaMoving = 0xF3,
		SECONDARY_STAT_Larkness = 0xF4,
		SECONDARY_STAT_StackBuff = 0xF5,
		SECONDARY_STAT_BlessOfDarkness = 0xF6,
		SECONDARY_STAT_AntiMagicShell = 0xF7,
		SECONDARY_STAT_LifeTidal = 0xF8,
		SECONDARY_STAT_HitCriDamR = 0xF9,
		SECONDARY_STAT_SmashStack = 0xFA,
		SECONDARY_STAT_PartyBarrier = 0xFB,
		SECONDARY_STAT_ReshuffleSwitch = 0xFC,
		SECONDARY_STAT_SpecialAction = 0xFD,
		SECONDARY_STAT_VampDeathSummon = 0xFE,
		SECONDARY_STAT_StopForceAtomInfo = 0xFF,
		SECONDARY_STAT_SoulGazeCriDamR = 0x100,
		SECONDARY_STAT_SoulRageCount = 0x101,
		SECONDARY_STAT_PowerTransferGauge = 0x102,
		SECONDARY_STAT_AffinitySlug = 0x103,
		SECONDARY_STAT_Trinity = 0x104,
		SECONDARY_STAT_IncMaxDamage = 0x105,
		SECONDARY_STAT_BossShield = 0x106,
		SECONDARY_STAT_MobZoneState = 0x107,
		SECONDARY_STAT_GiveMeHeal = 0x108,
		SECONDARY_STAT_TouchMe = 0x109,
		SECONDARY_STAT_Contagion = 0x10A,
		SECONDARY_STAT_ComboUnlimited = 0x10B,
		SECONDARY_STAT_SoulExalt = 0x10C,
		SECONDARY_STAT_IgnorePCounter = 0x10D,
		SECONDARY_STAT_IgnoreAllCounter = 0x10E,
		SECONDARY_STAT_IgnorePImmune = 0x10F,
		SECONDARY_STAT_IgnoreAllImmune = 0x110,
		SECONDARY_STAT_FinalJudgement = 0x111,
		SECONDARY_STAT_IceAura = 0x112,
		SECONDARY_STAT_FireAura = 0x113,
		SECONDARY_STAT_VengeanceOfAngel = 0x114,
		SECONDARY_STAT_HeavensDoor = 0x115,
		SECONDARY_STAT_Preparation = 0x116,
		SECONDARY_STAT_BullsEye = 0x117,
		SECONDARY_STAT_IncEffectHPPotion = 0x118,
		SECONDARY_STAT_IncEffectMPPotion = 0x119,
		SECONDARY_STAT_BleedingToxin = 0x11A,
		SECONDARY_STAT_IgnoreMobDamR = 0x11B,
		SECONDARY_STAT_Asura = 0x11C,
		SECONDARY_STAT_FlipTheCoin = 0x11D,
		SECONDARY_STAT_UnityOfPower = 0x11E,
		SECONDARY_STAT_Stimulate = 0x11F,
		SECONDARY_STAT_ReturnTeleport = 0x120,
		SECONDARY_STAT_DropRIncrease = 0x121,
		SECONDARY_STAT_IgnoreMobpdpR = 0x122,
		SECONDARY_STAT_BdR = 0x123,
		SECONDARY_STAT_CapDebuff = 0x124,
		SECONDARY_STAT_Exceed = 0x125,
		SECONDARY_STAT_DiabolikRecovery = 0x126,
		SECONDARY_STAT_FinalAttackProp = 0x127,
		SECONDARY_STAT_ExceedOverload = 0x128,
		SECONDARY_STAT_OverloadCount = 0x129,
		SECONDARY_STAT_BuckShot = 0x12A,
		SECONDARY_STAT_FireBomb = 0x12B,
		SECONDARY_STAT_HalfstatByDebuff = 0x12C,
		SECONDARY_STAT_SurplusSupply = 0x12D,
		SECONDARY_STAT_SetBaseDamage = 0x12E,
		SECONDARY_STAT_EVAR = 0x12F,
		SECONDARY_STAT_NewFlying = 0x130,
		SECONDARY_STAT_AmaranthGenerator = 0x131,
		SECONDARY_STAT_OnCapsule = 0x132,
		SECONDARY_STAT_CygnusElementSkill = 0x133,
		SECONDARY_STAT_StrikerHyperElectric = 0x134,
		SECONDARY_STAT_EventPointAbsorb = 0x135,
		SECONDARY_STAT_EventAssemble = 0x136,
		SECONDARY_STAT_StormBringer = 0x137,
		SECONDARY_STAT_ACCR = 0x138,
		SECONDARY_STAT_DEXR = 0x139,
		SECONDARY_STAT_Albatross = 0x13A,
		SECONDARY_STAT_Translucence = 0x13B,
		SECONDARY_STAT_PoseType = 0x13C,
		SECONDARY_STAT_LightOfSpirit = 0x13D,
		SECONDARY_STAT_ElementSoul = 0x13E,
		SECONDARY_STAT_GlimmeringTime = 0x13F,
		SECONDARY_STAT_TrueSight = 0x140,
		SECONDARY_STAT_SoulExplosion = 0x141,
		SECONDARY_STAT_SoulMP = 0x142,
		SECONDARY_STAT_FullSoulMP = 0x143,
		SECONDARY_STAT_SoulSkillDamageUp = 0x144,
		SECONDARY_STAT_ElementalCharge = 0x145,
		SECONDARY_STAT_Restoration = 0x146,
		SECONDARY_STAT_CrossOverChain = 0x147,
		SECONDARY_STAT_ChargeBuff = 0x148,
		SECONDARY_STAT_Reincarnation = 0x149,
		SECONDARY_STAT_KnightsAura = 0x14A,
		SECONDARY_STAT_ChillingStep = 0x14B,
		SECONDARY_STAT_DotBasedBuff = 0x14C,
		SECONDARY_STAT_BlessEnsenble = 0x14D,
		SECONDARY_STAT_ComboCostInc = 0x14E,
		SECONDARY_STAT_ExtremeArchery = 0x14F,
		SECONDARY_STAT_NaviFlying = 0x150,
		SECONDARY_STAT_QuiverCatridge = 0x151,
		SECONDARY_STAT_AdvancedQuiver = 0x152,
		SECONDARY_STAT_UserControlMob = 0x153,
		SECONDARY_STAT_ImmuneBarrier = 0x154,
		SECONDARY_STAT_ArmorPiercing = 0x155,
		SECONDARY_STAT_ZeroAuraStr = 0x156,
		SECONDARY_STAT_ZeroAuraSpd = 0x157,
		SECONDARY_STAT_CriticalGrowing = 0x158,
		SECONDARY_STAT_QuickDraw = 0x159,
		SECONDARY_STAT_BowMasterConcentration = 0x15A,
		SECONDARY_STAT_TimeFastABuff = 0x15B,
		SECONDARY_STAT_TimeFastBBuff = 0x15C,
		SECONDARY_STAT_GatherDropR = 0x15D,
		SECONDARY_STAT_AimBox2D = 0x15E,
		SECONDARY_STAT_IncMonsterBattleCaptureRate = 0x15F,
		SECONDARY_STAT_CursorSniping = 0x160,
		SECONDARY_STAT_DebuffTolerance = 0x161,
		SECONDARY_STAT_DotHealHPPerSecond = 0x162,
		SECONDARY_STAT_SpiritGuard = 0x163,
		SECONDARY_STAT_PreReviveOnce = 0x164,
		SECONDARY_STAT_SetBaseDamageByBuff = 0x165,
		SECONDARY_STAT_LimitMP = 0x166,
		SECONDARY_STAT_ReflectDamR = 0x167,
		SECONDARY_STAT_ComboTempest = 0x168,
		SECONDARY_STAT_MHPCutR = 0x169,
		SECONDARY_STAT_MMPCutR = 0x16A,
		SECONDARY_STAT_SelfWeakness = 0x16B,
		SECONDARY_STAT_ElementDarkness = 0x16C,
		SECONDARY_STAT_FlareTrick = 0x16D,
		SECONDARY_STAT_Ember = 0x16E,
		SECONDARY_STAT_Dominion = 0x16F,
		SECONDARY_STAT_SiphonVitality = 0x170,
		SECONDARY_STAT_DarknessAscension = 0x171,
		SECONDARY_STAT_BossWaitingLinesBuff = 0x172,
		SECONDARY_STAT_DamageReduce = 0x173,
		SECONDARY_STAT_ShadowServant = 0x174,
		SECONDARY_STAT_ShadowIllusion = 0x175,
		SECONDARY_STAT_KnockBack = 0x176,
		SECONDARY_STAT_AddAttackCount = 0x177,
		SECONDARY_STAT_ComplusionSlant = 0x178,
		SECONDARY_STAT_JaguarSummoned = 0x179,
		SECONDARY_STAT_JaguarCount = 0x17A,
		SECONDARY_STAT_SSFShootingAttack = 0x17B,
		SECONDARY_STAT_DevilCry = 0x17C,
		SECONDARY_STAT_ShieldAttack = 0x17D,
		SECONDARY_STAT_BMageAura = 0x17E,
		SECONDARY_STAT_DarkLighting = 0x17F,
		SECONDARY_STAT_AttackCountX = 0x180,
		SECONDARY_STAT_BMageDeath = 0x181,
		SECONDARY_STAT_BombTime = 0x182,
		SECONDARY_STAT_NoDebuff = 0x183,
		SECONDARY_STAT_BattlePvP_Mike_Shield = 0x184,
		SECONDARY_STAT_BattlePvP_Mike_Bugle = 0x185,
		SECONDARY_STAT_XenonAegisSystem = 0x186,
		SECONDARY_STAT_AngelicBursterSoulSeeker = 0x187,
		SECONDARY_STAT_HiddenPossession = 0x188,
		SECONDARY_STAT_NightWalkerBat = 0x189,
		SECONDARY_STAT_NightLordMark = 0x18A,
		SECONDARY_STAT_WizardIgnite = 0x18B,
		SECONDARY_STAT_FireBarrier = 0x18C,
		SECONDARY_STAT_ChangeFoxMan = 0x18D,
		SECONDARY_STAT_BattlePvP_Helena_Mark = 0x18E,
		SECONDARY_STAT_BattlePvP_Helena_WindSpirit = 0x18F,
		SECONDARY_STAT_BattlePvP_LangE_Protection = 0x190,
		SECONDARY_STAT_BattlePvP_LeeMalNyun_ScaleUp = 0x191,
		SECONDARY_STAT_BattlePvP_Revive = 0x192,
		SECONDARY_STAT_PinkbeanAttackBuff = 0x193,
		SECONDARY_STAT_PinkbeanRelax = 0x194,
		SECONDARY_STAT_PinkbeanRollingGrade = 0x195,
		SECONDARY_STAT_PinkbeanYoYoStack = 0x196,
		SECONDARY_STAT_RandAreaAttack = 0x197,
		SECONDARY_STAT_NextAttackEnhance = 0x198,
		SECONDARY_STAT_AranBeyonderDamAbsorb = 0x199,
		SECONDARY_STAT_AranCombotempastOption = 0x19A,
		SECONDARY_STAT_NautilusFinalAttack = 0x19B,
		SECONDARY_STAT_ViperTimeLeap = 0x19C,
		SECONDARY_STAT_RoyalGuardState = 0x19D,
		SECONDARY_STAT_RoyalGuardPrepare = 0x19E,
		SECONDARY_STAT_MichaelSoulLink = 0x19F,
		SECONDARY_STAT_MichaelStanceLink = 0x1A0,
		SECONDARY_STAT_TriflingWhimOnOff = 0x1A1,
		SECONDARY_STAT_AddRangeOnOff = 0x1A2,
		SECONDARY_STAT_KinesisPsychicPoint = 0x1A3,
		SECONDARY_STAT_KinesisPsychicOver = 0x1A4,
		SECONDARY_STAT_KinesisPsychicShield = 0x1A5,
		SECONDARY_STAT_KinesisIncMastery = 0x1A6,
		SECONDARY_STAT_KinesisPsychicEnergeShield = 0x1A7,
		SECONDARY_STAT_BladeStance = 0x1A8,
		SECONDARY_STAT_DebuffActiveSkillHPCon = 0x1A9,
		SECONDARY_STAT_DebuffIncHP = 0x1AA,
		SECONDARY_STAT_BowMasterMortalBlow = 0x1AB,
		SECONDARY_STAT_AngelicBursterSoulResonance = 0x1AC,
		SECONDARY_STAT_Fever = 0x1AD,
		SECONDARY_STAT_IgnisRore = 0x1AE,
		SECONDARY_STAT_RpSiksin = 0x1AF,
		SECONDARY_STAT_TeleportMasteryRange = 0x1B0,
		SECONDARY_STAT_FixCoolTime = 0x1B1,
		SECONDARY_STAT_IncMobRateDummy = 0x1B2,
		SECONDARY_STAT_AdrenalinBoost = 0x1B3,
		SECONDARY_STAT_AranSmashSwing = 0x1B4,
		SECONDARY_STAT_AranDrain = 0x1B5,
		SECONDARY_STAT_AranBoostEndHunt = 0x1B6,
		SECONDARY_STAT_HiddenHyperLinkMaximization = 0x1B7,
		SECONDARY_STAT_RWCylinder = 0x1B8,
		SECONDARY_STAT_RWCombination = 0x1B9,
		SECONDARY_STAT_RWMagnumBlow = 0x1BA,
		SECONDARY_STAT_RWBarrier = 0x1BB,
		SECONDARY_STAT_RWBarrierHeal = 0x1BC,
		SECONDARY_STAT_RWMaximizeCannon = 0x1BD,
		SECONDARY_STAT_RWOverHeat = 0x1BE,
		SECONDARY_STAT_UsingScouter = 0x1BF,
		SECONDARY_STAT_RWMovingEvar = 0x1C0,
		SECONDARY_STAT_Stigma = 0x1C1,
		SECONDARY_STAT_EnergyCharged = 0x1C2,
		SECONDARY_STAT_Dash_Speed = 0x1C3,
		SECONDARY_STAT_Dash_Jump = 0x1C4,
		SECONDARY_STAT_RideVehicle = 0x1C5,
		SECONDARY_STAT_PartyBooster = 0x1C6,
		SECONDARY_STAT_GuidedBullet = 0x1C7,
		SECONDARY_STAT_Undead = 0x1C8,
		SECONDARY_STAT_RideVehicleExpire = 0x1C9,
		SECONDARY_STAT_COUNT_PLUS1 = 0x1CA,
		SECONDARY_STAT_NONE = 0xFFFFFFFF,
	};*/


	ADD_TS_FLAG(IndiePAD, 0);
	ADD_TS_FLAG(IndieMAD, 1);
	ADD_TS_FLAG(IndiePDD, 2);
	ADD_TS_FLAG(IndieMDD, 3);
	ADD_TS_FLAG(IndieMHP, 4);
	ADD_TS_FLAG(IndieMHPR, 5);
	ADD_TS_FLAG(IndieMMP, 6);
	ADD_TS_FLAG(IndieMMPR, 7);
	ADD_TS_FLAG(IndieACC, 8);
	ADD_TS_FLAG(IndieEVA, 9);
	ADD_TS_FLAG(IndieJump, 10);
	ADD_TS_FLAG(IndieSpeed, 11);
	ADD_TS_FLAG(IndieAllStat, 12);
	ADD_TS_FLAG(IndieDodgeCriticalTime, 13);
	ADD_TS_FLAG(IndieEXP, 14);
	ADD_TS_FLAG(IndieBooster, 15);
	ADD_TS_FLAG(IndieFixedDamageR, 16);
	ADD_TS_FLAG(PyramidStunBuff, 17);
	ADD_TS_FLAG(PyramidFrozenBuff, 18);
	ADD_TS_FLAG(PyramidFireBuff, 19);
	ADD_TS_FLAG(PyramidBonusDamageBuff, 20);
	ADD_TS_FLAG(IndieRelaxEXP, 21);
	ADD_TS_FLAG(IndieSTR, 22);
	ADD_TS_FLAG(IndieDEX, 23);
	ADD_TS_FLAG(IndieINT, 24);
	ADD_TS_FLAG(IndieLUK, 25);
	ADD_TS_FLAG(IndieDamR, 26);
	ADD_TS_FLAG(IndieScriptBuff, 27);
	ADD_TS_FLAG(IndieMDF, 28);
	ADD_TS_FLAG(IndieMaxDamageOver, 29);
	ADD_TS_FLAG(IndieAsrR, 30);
	ADD_TS_FLAG(IndieTerR, 31);
	ADD_TS_FLAG(IndieCr, 32);
	ADD_TS_FLAG(IndiePDDR, 33);
	ADD_TS_FLAG(IndieCrMax, 34);
	ADD_TS_FLAG(IndieBDR, 35);
	ADD_TS_FLAG(IndieStatR, 36);
	ADD_TS_FLAG(IndieStance, 37);
	ADD_TS_FLAG(IndieIgnoreMobpdpR, 38);
	ADD_TS_FLAG(IndieEmpty, 39);
	ADD_TS_FLAG(IndiePADR, 40);
	ADD_TS_FLAG(IndieMADR, 41);
	ADD_TS_FLAG(IndieCrMaxR, 42);
	ADD_TS_FLAG(IndieEVAR, 43);
	ADD_TS_FLAG(IndieMDDR, 44);
	ADD_TS_FLAG(IndieDrainHP, 45);
	ADD_TS_FLAG(IndiePMdR, 46);
	ADD_TS_FLAG(IndieMaxDamageOverR, 47);
	ADD_TS_FLAG(IndieForceJump, 48);
	ADD_TS_FLAG(IndieForceSpeed, 49);
	ADD_TS_FLAG(IndieQrPointTerm, 50);

	ADD_TS_FLAG(INDIE_STAT_COUNT, 52);
	ADD_TS_FLAG(PAD, 53);
	ADD_TS_FLAG(PDD, 54);
	ADD_TS_FLAG(MAD, 55);
	ADD_TS_FLAG(MDD, 56);
	ADD_TS_FLAG(ACC, 57);
	ADD_TS_FLAG(EVA, 58);
	ADD_TS_FLAG(Craft, 59);
	ADD_TS_FLAG(Speed, 60);
	ADD_TS_FLAG(Jump, 61);
	ADD_TS_FLAG(MagicGuard, 62);
	ADD_TS_FLAG(DarkSight, 63);
	ADD_TS_FLAG(Booster, 64);
	ADD_TS_FLAG(PowerGuard, 65);
	ADD_TS_FLAG(MaxHP, 66);
	ADD_TS_FLAG(MaxMP, 67);
	ADD_TS_FLAG(Invincible, 68);
	ADD_TS_FLAG(SoulArrow, 69);
	ADD_TS_FLAG(Stun, 70);
	ADD_TS_FLAG(Poison, 71);
	ADD_TS_FLAG(Seal, 72);
	ADD_TS_FLAG(Darkness, 73);
	ADD_TS_FLAG(ComboCounter, 74);
	ADD_TS_FLAG(WeaponCharge, 75);
	ADD_TS_FLAG(HolySymbol, 76);
	ADD_TS_FLAG(MesoUp, 77);
	ADD_TS_FLAG(ShadowPartner, 78);
	ADD_TS_FLAG(PickPocket, 79);
	ADD_TS_FLAG(MesoGuard, 80);
	ADD_TS_FLAG(Thaw, 81);
	ADD_TS_FLAG(Weakness, 82);
	ADD_TS_FLAG(Curse, 83);
	ADD_TS_FLAG(Slow, 84);
	ADD_TS_FLAG(Morph, 85);
	ADD_TS_FLAG(Regen, 86);
	ADD_TS_FLAG(BasicStatUp, 87);
	ADD_TS_FLAG(Stance, 88);
	ADD_TS_FLAG(SharpEyes, 89);
	ADD_TS_FLAG(ManaReflection, 90);
	ADD_TS_FLAG(Attract, 91);
	ADD_TS_FLAG(NoBulletConsume, 92);
	ADD_TS_FLAG(Infinity, 93);
	ADD_TS_FLAG(AdvancedBless, 94);
	ADD_TS_FLAG(IllusionStep, 95);
	ADD_TS_FLAG(Blind, 96);
	ADD_TS_FLAG(Concentration, 97);
	ADD_TS_FLAG(BanMap, 98);
	ADD_TS_FLAG(MaxLevelBuff, 99);
	ADD_TS_FLAG(MesoUpByItem, 100);
	ADD_TS_FLAG(Ghost, 101);
	ADD_TS_FLAG(Barrier, 102);
	ADD_TS_FLAG(ReverseInput, 103);
	ADD_TS_FLAG(ItemUpByItem, 104);
	ADD_TS_FLAG(RespectPImmune, 105);
	ADD_TS_FLAG(RespectMImmune, 106);
	ADD_TS_FLAG(DefenseAtt, 107);
	ADD_TS_FLAG(DefenseState, 108);
	ADD_TS_FLAG(DojangBerserk, 109);
	ADD_TS_FLAG(DojangInvincible, 110);
	ADD_TS_FLAG(DojangShield, 111);
	ADD_TS_FLAG(SoulMasterFinal, 112);
	ADD_TS_FLAG(WindBreakerFinal, 113);
	ADD_TS_FLAG(ElementalReset, 114);
	ADD_TS_FLAG(HideAttack, 115);
	ADD_TS_FLAG(EventRate, 116);
	ADD_TS_FLAG(ComboAbilityBuff, 117);
	ADD_TS_FLAG(ComboDrain, 118);
	ADD_TS_FLAG(ComboBarrier, 119);
	ADD_TS_FLAG(BodyPressure, 120);
	ADD_TS_FLAG(RepeatEffect, 121);
	ADD_TS_FLAG(ExpBuffRate, 122);
	ADD_TS_FLAG(StopPortion, 123);
	ADD_TS_FLAG(StopMotion, 124);
	ADD_TS_FLAG(Fear, 125);
	ADD_TS_FLAG(HiddenPieceOn, 126);
	ADD_TS_FLAG(MagicShield, 127);
	ADD_TS_FLAG(MagicResistance, 128);
	ADD_TS_FLAG(SoulStone, 129);
	ADD_TS_FLAG(Flying, 130);
	ADD_TS_FLAG(Frozen, 131);
	ADD_TS_FLAG(AssistCharge, 132);
	ADD_TS_FLAG(Enrage, 133);
	ADD_TS_FLAG(DrawBack, 134);
	ADD_TS_FLAG(NotDamaged, 135);
	ADD_TS_FLAG(FinalCut, 136);
	ADD_TS_FLAG(HowlingAttackDamage, 137);
	ADD_TS_FLAG(BeastFormDamageUp, 138);
	ADD_TS_FLAG(Dance, 139);
	ADD_TS_FLAG(DAMAGE_RATE, 140);

	ADD_TS_FLAG(EMHP, 142);
	ADD_TS_FLAG(EMMP, 143);
	ADD_TS_FLAG(EPAD, 144);
	ADD_TS_FLAG(EMAD, 145);
	ADD_TS_FLAG(EPDD, 146);
	ADD_TS_FLAG(EMDD, 145);
	ADD_TS_FLAG(GUARD, 147);

	ADD_TS_FLAG(Cyclone, 148);
	ADD_TS_FLAG(HowlingCritical, 149);
	ADD_TS_FLAG(HowlingMaxMP, 150);
	ADD_TS_FLAG(HowlingDefence, 151);
	ADD_TS_FLAG(HowlingEvasion, 152);
	ADD_TS_FLAG(Conversion, 153);
	ADD_TS_FLAG(Revive, 154);
	ADD_TS_FLAG(PinkbeanMinibeenMove, 155);
	ADD_TS_FLAG(Sneak, 156);
	ADD_TS_FLAG(Mechanic, 157);
	ADD_TS_FLAG(BeastFormMaxHP, 158);
	ADD_TS_FLAG(Dice, 159);
	ADD_TS_FLAG(BlessingArmor, 160);
	ADD_TS_FLAG(DamR, 161);
	ADD_TS_FLAG(TeleportMasteryOn, 162);

	ADD_TS_FLAG(CombatOrders, 163);
	ADD_TS_FLAG(Beholder, 164);
	ADD_TS_FLAG(DispelItemOption, 165);
	ADD_TS_FLAG(Inflation, 166);
	ADD_TS_FLAG(OnyxDivineProtection, 167);
	ADD_TS_FLAG(Web, 168);
	ADD_TS_FLAG(Bless, 169);
	ADD_TS_FLAG(TimeBomb, 170);
	ADD_TS_FLAG(DisOrder, 171);
	ADD_TS_FLAG(Thread, 172);
	ADD_TS_FLAG(Team, 173);
	ADD_TS_FLAG(Explosion, 174);
	ADD_TS_FLAG(BuffLimit, 175);
	ADD_TS_FLAG(STR, 176);
	ADD_TS_FLAG(INT, 177);
	ADD_TS_FLAG(DEX, 178);
	ADD_TS_FLAG(LUK, 179);
	ADD_TS_FLAG(DispelItemOptionByField, 180);
	ADD_TS_FLAG(DarkTornado, 181);
	ADD_TS_FLAG(PVPDamage, 182);
	ADD_TS_FLAG(PvPScoreBonus, 183);
	ADD_TS_FLAG(PvPInvincible, 184);
	ADD_TS_FLAG(PvPRaceEffect, 185);
	ADD_TS_FLAG(WeaknessMdamage, 186);
	ADD_TS_FLAG(Frozen2, 187);
	ADD_TS_FLAG(PVPDamageSkill, 188);
	ADD_TS_FLAG(AmplifyDamage, 189);
	ADD_TS_FLAG(IceKnight, 190);
	ADD_TS_FLAG(Shock, 191);
	ADD_TS_FLAG(InfinityForce, 192);
	ADD_TS_FLAG(IncMaxHP, 193);
	ADD_TS_FLAG(IncMaxMP, 194);
	ADD_TS_FLAG(HolyMagicShell, 195);
	ADD_TS_FLAG(KeyDownTimeIgnore, 196);
	ADD_TS_FLAG(ArcaneAim, 197);
	ADD_TS_FLAG(MasterMagicOn, 198);
	ADD_TS_FLAG(AsrR, 199);
	ADD_TS_FLAG(TerR, 200);
	ADD_TS_FLAG(DamAbsorbShield, 201);
	ADD_TS_FLAG(DevilishPower, 202);
	ADD_TS_FLAG(Roulette, 203);
	ADD_TS_FLAG(SpiritLink, 204);
	ADD_TS_FLAG(AsrRByItem, 205);
	ADD_TS_FLAG(Event, 206);
	ADD_TS_FLAG(CriticalBuff, 207);
	ADD_TS_FLAG(DropRate, 208);
	ADD_TS_FLAG(PlusExpRate, 209);
	ADD_TS_FLAG(ItemInvincible, 210);
	ADD_TS_FLAG(Awake, 211);
	ADD_TS_FLAG(ItemCritical, 212);
	ADD_TS_FLAG(ItemEvade, 213);
	ADD_TS_FLAG(Event2, 214);

	ADD_TS_FLAG(VampiricTouch, 215);
	ADD_TS_FLAG(DDR, 216);
	ADD_TS_FLAG(IncCriticalDamMin, 217);
	ADD_TS_FLAG(IncCriticalDamMax, 218);
	ADD_TS_FLAG(IncTerR, 219);
	ADD_TS_FLAG(IncAsrR, 220);
	ADD_TS_FLAG(DeathMark, 221);

	ADD_TS_FLAG(UsefulAdvancedBless, 220);
	ADD_TS_FLAG(Lapidification, 221);
	ADD_TS_FLAG(VenomSnake, 222);
	ADD_TS_FLAG(CarnivalAttack, 223);
	ADD_TS_FLAG(CarnivalDefence, 224);
	ADD_TS_FLAG(CarnivalExp, 225);
	ADD_TS_FLAG(SlowAttack, 226);
	ADD_TS_FLAG(PyramidEffect, 227);
	ADD_TS_FLAG(KillingPoint, 228);
	ADD_TS_FLAG(HollowPointBullet, 229);
	ADD_TS_FLAG(KeyDownMoving, 230);
	ADD_TS_FLAG(IgnoreTargetDEF, 231);
	ADD_TS_FLAG(ReviveOnce, 232);
	ADD_TS_FLAG(Invisible, 233);
	ADD_TS_FLAG(EnrageCr, 234);

	ADD_TS_FLAG(EnrageCrDamMin, 235);
	ADD_TS_FLAG(Judgement, 236);
	ADD_TS_FLAG(DojangLuckyBonus, 237);
	ADD_TS_FLAG(PainMark, 238);
	ADD_TS_FLAG(Magnet, 239);
	ADD_TS_FLAG(MagnetArea, 240);

	ADD_TS_FLAG(VampDeath, 243);
	ADD_TS_FLAG(BlessingArmorIncPAD, 244);
	ADD_TS_FLAG(KeyDownAreaMoving, 245);
	ADD_TS_FLAG(Larkness, 246);
	ADD_TS_FLAG(StackBuff, 247);
	ADD_TS_FLAG(BlessOfDarkness, 248);
	ADD_TS_FLAG(AntiMagicShell, 249);
	ADD_TS_FLAG(LifeTidal, 250);
	ADD_TS_FLAG(HitCriDamR, 251);
	ADD_TS_FLAG(SmashStack, 252);
	ADD_TS_FLAG(PartyBarrier, 253);
	ADD_TS_FLAG(ReshuffleSwitch, 254);
	ADD_TS_FLAG(SpecialAction, 255);
	ADD_TS_FLAG(VampDeathSummon, 256);
	ADD_TS_FLAG(StopForceAtomInfo, 257);
	ADD_TS_FLAG(SoulGazeCriDamR, 258);
	ADD_TS_FLAG(SoulRageCount, 259);
	ADD_TS_FLAG(PowerTransferGauge, 260);
	ADD_TS_FLAG(AffinitySlug, 261);
	ADD_TS_FLAG(Trinity, 262);
	ADD_TS_FLAG(IncMaxDamage, 263);
	ADD_TS_FLAG(BossShield, 264);
	ADD_TS_FLAG(MobZoneState, 265);
	ADD_TS_FLAG(GiveMeHeal, 266);
	ADD_TS_FLAG(TouchMe, 267);
	ADD_TS_FLAG(Contagion, 268);
	ADD_TS_FLAG(ComboUnlimited, 269);
	ADD_TS_FLAG(SoulExalt, 270);
	ADD_TS_FLAG(IgnorePCounter, 271);
	ADD_TS_FLAG(IgnoreAllCounter, 272);
	ADD_TS_FLAG(IgnorePImmune, 273);
	ADD_TS_FLAG(IgnoreAllImmune, 274);

	ADD_TS_FLAG(FinalJudgement, 276);
	ADD_TS_FLAG(IceAura, 277);
	ADD_TS_FLAG(FireAura, 278);
	ADD_TS_FLAG(VengeanceOfAngel, 279);
	ADD_TS_FLAG(HeavensDoor, 280);
	ADD_TS_FLAG(Preparation, 281);
	ADD_TS_FLAG(BullsEye, 282);
	ADD_TS_FLAG(IncEffectHPPotion, 283);
	ADD_TS_FLAG(IncEffectMPPotion, 284);
	ADD_TS_FLAG(BleedingToxin, 285);
	ADD_TS_FLAG(IgnoreMobDamR, 286);
	ADD_TS_FLAG(Asura, 287);

	ADD_TS_FLAG(FlipTheCoin, 289);
	ADD_TS_FLAG(UnityOfPower, 290);
	ADD_TS_FLAG(Stimulate, 291);
	ADD_TS_FLAG(ReturnTeleport, 292);
	ADD_TS_FLAG(DropRIncrease, 293);
	ADD_TS_FLAG(IgnoreMobpdpR, 294);
	ADD_TS_FLAG(BDR, 295);
	ADD_TS_FLAG(CapDebuff, 296);
	ADD_TS_FLAG(Exceed, 297);
	ADD_TS_FLAG(DiabolikRecovery, 298);
	ADD_TS_FLAG(FinalAttackProp, 299);
	ADD_TS_FLAG(ExceedOverload, 300);
	ADD_TS_FLAG(OverloadCount, 301);
	ADD_TS_FLAG(BuckShot, 302);
	ADD_TS_FLAG(FireBomb, 303);
	ADD_TS_FLAG(HalfstatByDebuff, 304);
	ADD_TS_FLAG(SurplusSupply, 305);
	ADD_TS_FLAG(SetBaseDamage, 306);
	ADD_TS_FLAG(EVAR, 307);
	ADD_TS_FLAG(NewFlying, 308);
	ADD_TS_FLAG(AmaranthGenerator, 309);
	ADD_TS_FLAG(OnCapsule, 310);
	ADD_TS_FLAG(CygnusElementSkill, 311);
	ADD_TS_FLAG(StrikerHyperElectric, 312);
	ADD_TS_FLAG(EventPointAbsorb, 313);
	ADD_TS_FLAG(EventAssemble, 314);
	ADD_TS_FLAG(StormBringer, 315);
	ADD_TS_FLAG(ACCR, 316);
	ADD_TS_FLAG(DEXR, 317);
	ADD_TS_FLAG(Albatross, 318);
	ADD_TS_FLAG(Translucence, 319);
	ADD_TS_FLAG(PoseType, 320);
	ADD_TS_FLAG(LightOfSpirit, 321);
	ADD_TS_FLAG(ElementSoul, 322);
	ADD_TS_FLAG(GlimmeringTime, 323);
	ADD_TS_FLAG(TrueSight, 324);
	ADD_TS_FLAG(SoulExplosion, 325);
	ADD_TS_FLAG(SoulMP, 326);
	ADD_TS_FLAG(FullSoulMP, 327);
	ADD_TS_FLAG(SoulSkillDamageUp, 328);
	ADD_TS_FLAG(ElementalCharge, 329);
	ADD_TS_FLAG(Restoration, 330);
	ADD_TS_FLAG(CrossOverChain, 331);
	ADD_TS_FLAG(ChargeBuff, 332);
	ADD_TS_FLAG(Reincarnation, 333);
	ADD_TS_FLAG(KnightsAura, 334);
	ADD_TS_FLAG(ChillingStep, 335);
	ADD_TS_FLAG(DotBasedBuff, 336);
	ADD_TS_FLAG(BlessEnsenble, 337);
	ADD_TS_FLAG(ComboCostInc, 338);
	ADD_TS_FLAG(ExtremeArchery, 339);
	ADD_TS_FLAG(NaviFlying, 340);
	ADD_TS_FLAG(QuiverCatridge, 341);
	ADD_TS_FLAG(AdvancedQuiver, 342);
	ADD_TS_FLAG(UserControlMob, 343);
	ADD_TS_FLAG(ImmuneBarrier, 344);
	ADD_TS_FLAG(ArmorPiercing, 345);
	ADD_TS_FLAG(ZeroAuraStr, 346);
	ADD_TS_FLAG(ZeroAuraSpd, 347);
	ADD_TS_FLAG(CriticalGrowing, 348);
	ADD_TS_FLAG(QuickDraw, 349);
	ADD_TS_FLAG(BowMasterConcentration, 350);
	ADD_TS_FLAG(TimeFastABuff, 351);
	ADD_TS_FLAG(TimeFastBBuff, 352);
	ADD_TS_FLAG(GatherDropR, 353);
	ADD_TS_FLAG(AimBox2D, 354);
	ADD_TS_FLAG(IncMonsterBattleCaptureRate, 355);
	ADD_TS_FLAG(CursorSniping, 356);
	ADD_TS_FLAG(DebuffTolerance, 357);
	ADD_TS_FLAG(DotHealHPPerSecond, 358);
	ADD_TS_FLAG(SpiritGuard, 360);
	ADD_TS_FLAG(PreReviveOnce, 361);
	ADD_TS_FLAG(SetBaseDamageByBuff, 362);
	ADD_TS_FLAG(LimitMP, 363);
	ADD_TS_FLAG(ReflectDamR, 364);
	ADD_TS_FLAG(ComboTempest, 365);
	ADD_TS_FLAG(MHPCutR, 366);
	ADD_TS_FLAG(MMPCutR, 367);
	ADD_TS_FLAG(SelfWeakness, 368);
	ADD_TS_FLAG(ElementDarkness, 369);
	ADD_TS_FLAG(FlareTrick, 370);
	ADD_TS_FLAG(Ember, 371);
	ADD_TS_FLAG(Dominion, 372);
	ADD_TS_FLAG(SiphonVitality, 373);
	ADD_TS_FLAG(DarknessAscension, 374);
	ADD_TS_FLAG(BossWaitingLinesBuff, 375);
	ADD_TS_FLAG(DamageReduce, 376);
	ADD_TS_FLAG(ShadowServant, 377);
	ADD_TS_FLAG(ShadowIllusion, 378);
	ADD_TS_FLAG(KnockBack, 379);
	ADD_TS_FLAG(AddAttackCount, 380);
	ADD_TS_FLAG(ComplusionSlant, 381);
	ADD_TS_FLAG(JaguarSummoned, 382);
	ADD_TS_FLAG(JaguarCount, 383);
	ADD_TS_FLAG(SSFShootingAttack, 384);
	ADD_TS_FLAG(DevilCry, 385);
	ADD_TS_FLAG(ShieldAttack, 386);
	ADD_TS_FLAG(BMageAura, 387);
	ADD_TS_FLAG(DarkLighting, 388);
	ADD_TS_FLAG(AttackCountX, 389);
	ADD_TS_FLAG(BMageDeath, 390);
	ADD_TS_FLAG(BombTime, 391);
	ADD_TS_FLAG(NoDebuff, 392);
	ADD_TS_FLAG(BattlePvP_Mike_Shield, 393);
	ADD_TS_FLAG(BattlePvP_Mike_Bugle, 394);
	ADD_TS_FLAG(XenonAegisSystem, 395);
	ADD_TS_FLAG(AngelicBursterSoulSeeker, 396);
	ADD_TS_FLAG(HiddenPossession, 397);
	ADD_TS_FLAG(NightWalkerBat, 398);
	ADD_TS_FLAG(NightLordMark, 399);
	ADD_TS_FLAG(WizardIgnite, 400);
	ADD_TS_FLAG(FireBarrier, 401);
	ADD_TS_FLAG(ChangeFoxMan, 402);

	ADD_TS_FLAG(HayatoChangeMode, 407);
	ADD_TS_FLAG(HayatoCombo, 408);
	ADD_TS_FLAG(BeastModeChange, 426);
	ADD_TS_FLAG(BattlePvP_Helena_Mark, 431);
	ADD_TS_FLAG(BattlePvP_Helena_WindSpirit, 432);
	ADD_TS_FLAG(BattlePvP_LangE_Protection, 433);
	ADD_TS_FLAG(BattlePvP_LeeMalNyun_ScaleUp, 434);
	ADD_TS_FLAG(BattlePvP_Revive, 435);
	ADD_TS_FLAG(PinkbeanAttackBuff, 436);
	ADD_TS_FLAG(PinkbeanRelax, 437);
	ADD_TS_FLAG(PinkbeanRollingGrade, 438);
	ADD_TS_FLAG(PinkbeanYoYoStack, 439);
	ADD_TS_FLAG(RandAreaAttack, 440);
	ADD_TS_FLAG(NextAttackEnhance, 441);
	ADD_TS_FLAG(AranBeyonderDamAbsorb, 442);
	ADD_TS_FLAG(AranCombotempastOption, 443);
	ADD_TS_FLAG(NautilusFinalAttack, 444);
	ADD_TS_FLAG(ViperTimeLeap, 445);
	ADD_TS_FLAG(RoyalGuardState, 446);
	ADD_TS_FLAG(RoyalGuardPrepare, 447);
	ADD_TS_FLAG(MichaelSoulLink, 448);
	ADD_TS_FLAG(MichaelStanceLink, 449);
	ADD_TS_FLAG(TriflingWhimOnOff, 450);
	ADD_TS_FLAG(AddRangeOnOff, 451);
	ADD_TS_FLAG(KinesisPsychicPoint, 452);
	ADD_TS_FLAG(KinesisPsychicOver, 453);
	ADD_TS_FLAG(KinesisPsychicShield, 454);
	ADD_TS_FLAG(KinesisIncMastery, 455);
	ADD_TS_FLAG(KinesisPsychicEnergeShield, 456);
	ADD_TS_FLAG(BladeStance, 457);
	ADD_TS_FLAG(DebuffActiveSkillHPCon, 458);
	ADD_TS_FLAG(DebuffIncHP, 459);
	ADD_TS_FLAG(BowMasterMortalBlow, 460);
	ADD_TS_FLAG(AngelicBursterSoulResonance, 461);
	ADD_TS_FLAG(Fever, 462);
	ADD_TS_FLAG(IgnisRore, 463);
	ADD_TS_FLAG(RpSiksin, 464);
	ADD_TS_FLAG(BunnerBullet, 464);
	ADD_TS_FLAG(TeleportMasteryRange, 465);
	ADD_TS_FLAG(FixCoolTime, 466);
	ADD_TS_FLAG(IncMobRateDummy, 467);
	ADD_TS_FLAG(AdrenalinBoost, 468);
	ADD_TS_FLAG(AranSmashSwing, 469);
	ADD_TS_FLAG(AranDrain, 470);
	ADD_TS_FLAG(AranBoostEndHunt, 471);
	ADD_TS_FLAG(PreciseMissle, 461);
	ADD_TS_FLAG(HiddenHyperLinkMaximization, 472);
	ADD_TS_FLAG(RWCylinder, 473);
	ADD_TS_FLAG(RWCombination, 474);
	ADD_TS_FLAG(RWMagnumBlow, 475);
	ADD_TS_FLAG(RWBarrier, 476);
	ADD_TS_FLAG(RWBarrierHeal, 477);
	ADD_TS_FLAG(RWMaximizeCannon, 478);
	ADD_TS_FLAG(RWOverHeat, 479);
	ADD_TS_FLAG(UsingScouter, 480);
	ADD_TS_FLAG(RWMovingEvar, 481);
	ADD_TS_FLAG(Stigma, 482);

	ADD_TS_FLAG(EnergyCharged, 520);
	ADD_TS_FLAG(Dash_Speed, 521);
	ADD_TS_FLAG(Dash_Jump, 522);
	ADD_TS_FLAG(RideVehicle, 523);
	ADD_TS_FLAG(PartyBooster, 524);
	ADD_TS_FLAG(GuidedBullet, 525);
	ADD_TS_FLAG(Undead, 526);
	ADD_TS_FLAG(RideVehicleExpire, 527);
};

