#include "SecondaryStat.h"
#include "BasicStat.h"
#include "SkillInfo.h"
#include "SkillEntry.h"
#include "SkillLevelData.h"
#include "..\Database\GA_Character.hpp"
#include "..\Database\GW_ItemSlotEquip.h"
#include "..\Common\Net\OutPacket.h"

#define CHECK_TS_NORMAL(name) \
if (flag & GET_TS_FLAG(name)) \
{ if (EnDecode4Byte(flag))oPacket->Encode4(n##name); else oPacket->Encode2(n##name);\
oPacket->Encode4(r##name); oPacket->Encode4(t##name); }


SecondaryStat::SecondaryStat()
{
}


SecondaryStat::~SecondaryStat()
{
}

void SecondaryStat::SetFrom(int nFieldType, GA_Character * pChar, BasicStat * pBS, void * pFs, void * pNonBodyEquip, int nMHPForPvP, void * pPSD)
{
	const GW_CharacterStat *pCS = pChar->mStat;

	this->nPAD = 0;
	this->nPDD = 0;

	//不知道新的計算公式為何
	this->nMAD = pBS->nINT;
	this->nMDD = pBS->nINT;

	this->nEVA = pBS->nLUK / 2 + pBS->nDEX / 4;
	this->nACC = pBS->nDEX + pBS->nLUK;
	this->nSpeed = 100;
	this->nJump = 100;
	this->nCraft = pBS->nDEX + pBS->nLUK + pBS->nINT;

	int nPDDIncRate = 0; //shield mastery ?

	const GW_ItemSlotEquip* pEquip;
	for (const auto& itemEquipped : pChar->mItemSlot[1])
	{
		pEquip = (const GW_ItemSlotEquip*)itemEquipped.second;

		nPDD += pEquip->nPDD;
		nPAD += pEquip->nPAD;
		nMDD += pEquip->nMDD;
		nMAD += pEquip->nMAD;

		nACC += pEquip->nACC;
		nEVA += pEquip->nEVA;
	}
	SkillEntry
		*pShoutOfEmpress = nullptr,
		*pMichaelShoutOfEmpress = nullptr,
		*pUltimateAdventurer = nullptr,
		*pReinforcementOfEmpress = nullptr;

	int nCheckSLV = 0;
	nCheckSLV = SkillInfo::GetInstance()->GetSkillLevel(pChar, 10000074, &pShoutOfEmpress, 0, 0, 0, 1);
	if (nCheckSLV && pShoutOfEmpress)
	{
		auto pLevelData = pShoutOfEmpress->GetLevelData(nCheckSLV);
		nIncMaxHPr = pLevelData->m_nX;
		nIncMaxMPr = pLevelData->m_nX;
	}

	nCheckSLV = SkillInfo::GetInstance()->GetSkillLevel(pChar, 50000074, &pMichaelShoutOfEmpress, 0, 0, 0, 1);
	if (nCheckSLV && pMichaelShoutOfEmpress)
	{
		auto pLevelData = pMichaelShoutOfEmpress->GetLevelData(nCheckSLV);
		nIncMaxHPr = pLevelData->m_nX;
		nIncMaxMPr = pLevelData->m_nX;
	}

	nCheckSLV = SkillInfo::GetInstance()->GetSkillLevel(pChar, 74, &pUltimateAdventurer, 0, 0, 0, 1);
	if (nCheckSLV && pUltimateAdventurer)
	{
		auto pLevelData = pUltimateAdventurer->GetLevelData(nCheckSLV);
		nLevel = pLevelData->m_nX;
	}

	nCheckSLV = SkillInfo::GetInstance()->GetSkillLevel(pChar, 80, &pReinforcementOfEmpress, 0, 0, 0, 1);
	if (nCheckSLV && pReinforcementOfEmpress)
	{
		auto pLevelData = pReinforcementOfEmpress->GetLevelData(nCheckSLV);
		nLevel = pLevelData->m_nX;
	}

	nIncMaxHPr_Forced = nIncMaxHPr;
}

void SecondaryStat::EncodeForLocal(OutPacket * oPacket, TemporaryStat::TS_Flag & flag)
{
	flag.Encode(oPacket);
	CHECK_TS_NORMAL(STR);
	CHECK_TS_NORMAL(INT);
	CHECK_TS_NORMAL(DEX);
	CHECK_TS_NORMAL(LUK);
	CHECK_TS_NORMAL(PAD);
	CHECK_TS_NORMAL(PDD);
	CHECK_TS_NORMAL(MAD);
	CHECK_TS_NORMAL(MDD);
	CHECK_TS_NORMAL(ACC);
	CHECK_TS_NORMAL(EVA);
	CHECK_TS_NORMAL(EVAR);
	CHECK_TS_NORMAL(Craft);
	CHECK_TS_NORMAL(Speed);
	CHECK_TS_NORMAL(Jump);
	CHECK_TS_NORMAL(EMHP);
	CHECK_TS_NORMAL(EMMP);
	CHECK_TS_NORMAL(EPAD);
	CHECK_TS_NORMAL(EMAD);
	CHECK_TS_NORMAL(EPDD);
	CHECK_TS_NORMAL(EMDD);
	CHECK_TS_NORMAL(MagicGuard);
	CHECK_TS_NORMAL(DarkSight);
	CHECK_TS_NORMAL(Booster);
	CHECK_TS_NORMAL(PowerGuard);
	//WRITE_TS_NORMAL(Guard);
	CHECK_TS_NORMAL(MaxHP);
	CHECK_TS_NORMAL(MaxMP);
	CHECK_TS_NORMAL(Invincible);
	CHECK_TS_NORMAL(SoulArrow);
	CHECK_TS_NORMAL(Stun);
	CHECK_TS_NORMAL(Shock);
	CHECK_TS_NORMAL(Poison);
	CHECK_TS_NORMAL(Seal);
	CHECK_TS_NORMAL(Darkness);
	CHECK_TS_NORMAL(ComboCounter);
	CHECK_TS_NORMAL(WeaponCharge);
	CHECK_TS_NORMAL(ElementalCharge);
	CHECK_TS_NORMAL(HolySymbol);
	CHECK_TS_NORMAL(MesoUp);
	CHECK_TS_NORMAL(ShadowPartner);
	CHECK_TS_NORMAL(PickPocket);
	CHECK_TS_NORMAL(MesoGuard);
	CHECK_TS_NORMAL(Thaw);
	CHECK_TS_NORMAL(Weakness);
	CHECK_TS_NORMAL(WeaknessMdamage);
	CHECK_TS_NORMAL(Curse);
	CHECK_TS_NORMAL(Slow);
	CHECK_TS_NORMAL(TimeBomb);
	CHECK_TS_NORMAL(BuffLimit);
	CHECK_TS_NORMAL(Team);
	CHECK_TS_NORMAL(DisOrder);
	CHECK_TS_NORMAL(Thread);
	CHECK_TS_NORMAL(Morph);
	CHECK_TS_NORMAL(Ghost);
	CHECK_TS_NORMAL(Regen);
	CHECK_TS_NORMAL(BasicStatUp);
	CHECK_TS_NORMAL(Stance);
	CHECK_TS_NORMAL(SharpEyes);
	CHECK_TS_NORMAL(ManaReflection);
	CHECK_TS_NORMAL(Attract);
	CHECK_TS_NORMAL(Magnet);
	CHECK_TS_NORMAL(MagnetArea);
	CHECK_TS_NORMAL(NoBulletConsume);
	CHECK_TS_NORMAL(StackBuff);
	CHECK_TS_NORMAL(Trinity);
	CHECK_TS_NORMAL(Infinity);
	CHECK_TS_NORMAL(AdvancedBless);
	CHECK_TS_NORMAL(IllusionStep);
	CHECK_TS_NORMAL(Blind);
	CHECK_TS_NORMAL(Concentration);
	CHECK_TS_NORMAL(BanMap);
	CHECK_TS_NORMAL(MaxLevelBuff);
	CHECK_TS_NORMAL(Barrier);
	CHECK_TS_NORMAL(DojangShield);
	CHECK_TS_NORMAL(ReverseInput);
	CHECK_TS_NORMAL(MesoUpByItem);
	CHECK_TS_NORMAL(ItemUpByItem);
	CHECK_TS_NORMAL(RespectPImmune);
	CHECK_TS_NORMAL(RespectMImmune);
	CHECK_TS_NORMAL(DefenseAtt);
	CHECK_TS_NORMAL(DefenseState);
	CHECK_TS_NORMAL(DojangBerserk);
	CHECK_TS_NORMAL(DojangInvincible);
	CHECK_TS_NORMAL(SoulMasterFinal);
	CHECK_TS_NORMAL(WindBreakerFinal);
	CHECK_TS_NORMAL(ElementalReset);
	CHECK_TS_NORMAL(HideAttack);
	CHECK_TS_NORMAL(EventRate);
	CHECK_TS_NORMAL(ComboAbilityBuff);
	CHECK_TS_NORMAL(ComboDrain);
	CHECK_TS_NORMAL(ComboBarrier);
	CHECK_TS_NORMAL(PartyBarrier);
	CHECK_TS_NORMAL(BodyPressure);
	CHECK_TS_NORMAL(RepeatEffect);
	CHECK_TS_NORMAL(ExpBuffRate);
	CHECK_TS_NORMAL(StopPortion);
	CHECK_TS_NORMAL(StopMotion);
	CHECK_TS_NORMAL(Fear);
	CHECK_TS_NORMAL(MagicShield);
	CHECK_TS_NORMAL(MagicResistance);
	CHECK_TS_NORMAL(SoulStone);
	CHECK_TS_NORMAL(Flying);
	CHECK_TS_NORMAL(NewFlying);
	CHECK_TS_NORMAL(NaviFlying);
	CHECK_TS_NORMAL(Frozen);
	CHECK_TS_NORMAL(Frozen2);
	CHECK_TS_NORMAL(Web);
	CHECK_TS_NORMAL(Enrage);
	CHECK_TS_NORMAL(NotDamaged);
	CHECK_TS_NORMAL(FinalCut);
	CHECK_TS_NORMAL(HowlingAttackDamage);
	CHECK_TS_NORMAL(BeastFormDamageUp);
	CHECK_TS_NORMAL(Dance);
	CHECK_TS_NORMAL(Cyclone);
	CHECK_TS_NORMAL(OnCapsule);
	CHECK_TS_NORMAL(HowlingCritical);
	CHECK_TS_NORMAL(HowlingMaxMP);
	CHECK_TS_NORMAL(HowlingDefence);
	CHECK_TS_NORMAL(HowlingEvasion);
	CHECK_TS_NORMAL(Conversion);
	CHECK_TS_NORMAL(Revive);
	CHECK_TS_NORMAL(PinkbeanMinibeenMove);
	CHECK_TS_NORMAL(Sneak);
	CHECK_TS_NORMAL(Mechanic);
	CHECK_TS_NORMAL(DrawBack);
	CHECK_TS_NORMAL(BeastFormMaxHP);
	CHECK_TS_NORMAL(Dice);
	CHECK_TS_NORMAL(BlessingArmor);
	CHECK_TS_NORMAL(BlessingArmorIncPAD);
	CHECK_TS_NORMAL(DamR);
	CHECK_TS_NORMAL(TeleportMasteryOn);
	CHECK_TS_NORMAL(CombatOrders);
	CHECK_TS_NORMAL(Beholder);
	CHECK_TS_NORMAL(DispelItemOption);
	CHECK_TS_NORMAL(DispelItemOptionByField);
	CHECK_TS_NORMAL(Inflation);
	//WRITE_TS_NORMAL(OnixDivineProtection);
	CHECK_TS_NORMAL(Bless);
	CHECK_TS_NORMAL(Explosion);
	CHECK_TS_NORMAL(DarkTornado);
	CHECK_TS_NORMAL(IncMaxHP);
	CHECK_TS_NORMAL(IncMaxMP);
	CHECK_TS_NORMAL(PVPDamage);
	CHECK_TS_NORMAL(PVPDamageSkill);
	CHECK_TS_NORMAL(PvPScoreBonus);
	CHECK_TS_NORMAL(PvPInvincible);
	CHECK_TS_NORMAL(PvPRaceEffect);
	CHECK_TS_NORMAL(IceKnight);
	CHECK_TS_NORMAL(HolyMagicShell);
	CHECK_TS_NORMAL(InfinityForce);
	CHECK_TS_NORMAL(AmplifyDamage);
	CHECK_TS_NORMAL(KeyDownTimeIgnore);
	CHECK_TS_NORMAL(MasterMagicOn);
	CHECK_TS_NORMAL(AsrR);
	CHECK_TS_NORMAL(AsrRByItem);
	CHECK_TS_NORMAL(TerR);
	CHECK_TS_NORMAL(DamAbsorbShield);
	CHECK_TS_NORMAL(Roulette);
	CHECK_TS_NORMAL(Event);
	CHECK_TS_NORMAL(SpiritLink);
	CHECK_TS_NORMAL(CriticalBuff);
	CHECK_TS_NORMAL(DropRate);
	CHECK_TS_NORMAL(PlusExpRate);
	CHECK_TS_NORMAL(ItemInvincible);
	CHECK_TS_NORMAL(ItemCritical);
	CHECK_TS_NORMAL(ItemEvade);
	CHECK_TS_NORMAL(Event2);
	CHECK_TS_NORMAL(VampiricTouch);
	CHECK_TS_NORMAL(DDR);
	CHECK_TS_NORMAL(IncCriticalDamMin);
	CHECK_TS_NORMAL(IncCriticalDamMax);
	CHECK_TS_NORMAL(IncTerR);
	CHECK_TS_NORMAL(IncAsrR);
	CHECK_TS_NORMAL(DeathMark);
	CHECK_TS_NORMAL(PainMark);
	CHECK_TS_NORMAL(UsefulAdvancedBless);
	CHECK_TS_NORMAL(Lapidification);
	CHECK_TS_NORMAL(VampDeath);
	CHECK_TS_NORMAL(VampDeathSummon);
	CHECK_TS_NORMAL(VenomSnake);
	CHECK_TS_NORMAL(CarnivalAttack);
	CHECK_TS_NORMAL(CarnivalDefence);
	CHECK_TS_NORMAL(CarnivalExp);
	CHECK_TS_NORMAL(SlowAttack);
	CHECK_TS_NORMAL(PyramidEffect);
	CHECK_TS_NORMAL(HollowPointBullet);
	CHECK_TS_NORMAL(KeyDownMoving);
	CHECK_TS_NORMAL(KeyDownAreaMoving);
	CHECK_TS_NORMAL(CygnusElementSkill);
	CHECK_TS_NORMAL(IgnoreTargetDEF);
	CHECK_TS_NORMAL(Invisible);
	CHECK_TS_NORMAL(ReviveOnce);
	CHECK_TS_NORMAL(AntiMagicShell);
	CHECK_TS_NORMAL(EnrageCr);
	CHECK_TS_NORMAL(EnrageCrDamMin);
	CHECK_TS_NORMAL(BlessOfDarkness);
	CHECK_TS_NORMAL(LifeTidal);
	CHECK_TS_NORMAL(Judgement);
	CHECK_TS_NORMAL(DojangLuckyBonus);
	CHECK_TS_NORMAL(HitCriDamR);
	CHECK_TS_NORMAL(Larkness);
	CHECK_TS_NORMAL(SmashStack);
	CHECK_TS_NORMAL(ReshuffleSwitch);
	CHECK_TS_NORMAL(SpecialAction);
	CHECK_TS_NORMAL(ArcaneAim);
	CHECK_TS_NORMAL(StopForceAtomInfo);
	CHECK_TS_NORMAL(SoulGazeCriDamR);
	CHECK_TS_NORMAL(SoulRageCount);
	CHECK_TS_NORMAL(PowerTransferGauge);
	CHECK_TS_NORMAL(AffinitySlug);
	CHECK_TS_NORMAL(SoulExalt);
	CHECK_TS_NORMAL(HiddenPieceOn);
	CHECK_TS_NORMAL(IncMaxDamage);
	CHECK_TS_NORMAL(BossShield);
	CHECK_TS_NORMAL(MobZoneState);
	CHECK_TS_NORMAL(GiveMeHeal);
	CHECK_TS_NORMAL(TouchMe);
	CHECK_TS_NORMAL(Contagion);
	CHECK_TS_NORMAL(ComboUnlimited);
	CHECK_TS_NORMAL(IgnorePCounter);
	CHECK_TS_NORMAL(IgnoreAllCounter);
	CHECK_TS_NORMAL(IgnorePImmune);
	CHECK_TS_NORMAL(IgnoreAllImmune);
	CHECK_TS_NORMAL(FinalJudgement);
	CHECK_TS_NORMAL(KnightsAura);
	CHECK_TS_NORMAL(IceAura);
	CHECK_TS_NORMAL(FireAura);
	CHECK_TS_NORMAL(VengeanceOfAngel);
	CHECK_TS_NORMAL(HeavensDoor);
	CHECK_TS_NORMAL(Preparation);
	CHECK_TS_NORMAL(BullsEye);
	CHECK_TS_NORMAL(IncEffectHPPotion);
	CHECK_TS_NORMAL(IncEffectMPPotion);
	CHECK_TS_NORMAL(SoulMP);
	CHECK_TS_NORMAL(FullSoulMP);
	CHECK_TS_NORMAL(SoulSkillDamageUp);
	CHECK_TS_NORMAL(BleedingToxin);
	CHECK_TS_NORMAL(IgnoreMobDamR);
	CHECK_TS_NORMAL(Asura);
	CHECK_TS_NORMAL(FlipTheCoin);
	CHECK_TS_NORMAL(UnityOfPower);
	CHECK_TS_NORMAL(Stimulate);
	CHECK_TS_NORMAL(ReturnTeleport);
	CHECK_TS_NORMAL(CapDebuff);
	CHECK_TS_NORMAL(DropRIncrease);
	CHECK_TS_NORMAL(IgnoreMobpdpR);
	CHECK_TS_NORMAL(BDR);
	CHECK_TS_NORMAL(Exceed);
	CHECK_TS_NORMAL(DiabolikRecovery);
	CHECK_TS_NORMAL(FinalAttackProp);
	CHECK_TS_NORMAL(ExceedOverload);
	CHECK_TS_NORMAL(DevilishPower);
	CHECK_TS_NORMAL(OverloadCount);
	CHECK_TS_NORMAL(BuckShot);
	CHECK_TS_NORMAL(FireBomb);
	CHECK_TS_NORMAL(HalfstatByDebuff);
	CHECK_TS_NORMAL(SurplusSupply);
	CHECK_TS_NORMAL(SetBaseDamage);
	CHECK_TS_NORMAL(AmaranthGenerator);
	CHECK_TS_NORMAL(StrikerHyperElectric);
	CHECK_TS_NORMAL(EventPointAbsorb);
	CHECK_TS_NORMAL(EventAssemble);
	CHECK_TS_NORMAL(StormBringer);
	CHECK_TS_NORMAL(ACCR);
	CHECK_TS_NORMAL(DEXR);
	CHECK_TS_NORMAL(Albatross);
	CHECK_TS_NORMAL(Translucence);
	CHECK_TS_NORMAL(PoseType);
	CHECK_TS_NORMAL(LightOfSpirit);
	CHECK_TS_NORMAL(ElementSoul);
	CHECK_TS_NORMAL(GlimmeringTime);
	CHECK_TS_NORMAL(Restoration);
	CHECK_TS_NORMAL(ComboCostInc);
	CHECK_TS_NORMAL(ChargeBuff);
	CHECK_TS_NORMAL(TrueSight);
	CHECK_TS_NORMAL(CrossOverChain);
	CHECK_TS_NORMAL(ChillingStep);
	CHECK_TS_NORMAL(Reincarnation);
	CHECK_TS_NORMAL(DotBasedBuff);
	CHECK_TS_NORMAL(BlessEnsenble);
	CHECK_TS_NORMAL(ExtremeArchery);
	CHECK_TS_NORMAL(QuiverCatridge);
	CHECK_TS_NORMAL(AdvancedQuiver);
	CHECK_TS_NORMAL(UserControlMob);
	CHECK_TS_NORMAL(ImmuneBarrier);
	CHECK_TS_NORMAL(ArmorPiercing);
	CHECK_TS_NORMAL(ZeroAuraStr);
	CHECK_TS_NORMAL(ZeroAuraSpd);
	CHECK_TS_NORMAL(CriticalGrowing);
	CHECK_TS_NORMAL(QuickDraw);
	CHECK_TS_NORMAL(BowMasterConcentration);
	CHECK_TS_NORMAL(TimeFastABuff);
	CHECK_TS_NORMAL(TimeFastBBuff);
	CHECK_TS_NORMAL(GatherDropR);
	CHECK_TS_NORMAL(AimBox2D);
	CHECK_TS_NORMAL(CursorSniping);
	CHECK_TS_NORMAL(IncMonsterBattleCaptureRate);
	CHECK_TS_NORMAL(DebuffTolerance);
	CHECK_TS_NORMAL(DotHealHPPerSecond);
	CHECK_TS_NORMAL(SpiritGuard);
	CHECK_TS_NORMAL(PreReviveOnce);
	CHECK_TS_NORMAL(SetBaseDamageByBuff);
	CHECK_TS_NORMAL(LimitMP);
	CHECK_TS_NORMAL(ReflectDamR);
	CHECK_TS_NORMAL(ComboTempest);
	CHECK_TS_NORMAL(MHPCutR);
	CHECK_TS_NORMAL(MMPCutR);
	CHECK_TS_NORMAL(SelfWeakness);
	CHECK_TS_NORMAL(ElementDarkness);
	CHECK_TS_NORMAL(FlareTrick);
	CHECK_TS_NORMAL(Ember);
	CHECK_TS_NORMAL(Dominion);
	CHECK_TS_NORMAL(SiphonVitality);
	CHECK_TS_NORMAL(DarknessAscension);
	CHECK_TS_NORMAL(BossWaitingLinesBuff);
	CHECK_TS_NORMAL(DamageReduce);
	CHECK_TS_NORMAL(ShadowServant);
	CHECK_TS_NORMAL(ShadowIllusion);
	CHECK_TS_NORMAL(AddAttackCount);
	CHECK_TS_NORMAL(ComplusionSlant);
	CHECK_TS_NORMAL(JaguarSummoned);
	CHECK_TS_NORMAL(JaguarCount);
	CHECK_TS_NORMAL(SSFShootingAttack);
	CHECK_TS_NORMAL(DevilCry);
	CHECK_TS_NORMAL(ShieldAttack);
	CHECK_TS_NORMAL(BMageAura);
	CHECK_TS_NORMAL(DarkLighting);
	CHECK_TS_NORMAL(AttackCountX);
	CHECK_TS_NORMAL(BMageDeath);
	CHECK_TS_NORMAL(BombTime);
	CHECK_TS_NORMAL(NoDebuff);
	CHECK_TS_NORMAL(XenonAegisSystem);
	CHECK_TS_NORMAL(AngelicBursterSoulSeeker);
	CHECK_TS_NORMAL(HiddenPossession);
	CHECK_TS_NORMAL(NightWalkerBat);
	CHECK_TS_NORMAL(NightLordMark);
	CHECK_TS_NORMAL(WizardIgnite);
	CHECK_TS_NORMAL(BattlePvP_Helena_Mark);
	CHECK_TS_NORMAL(BattlePvP_Helena_WindSpirit);
	CHECK_TS_NORMAL(BattlePvP_LangE_Protection);
	CHECK_TS_NORMAL(BattlePvP_LeeMalNyun_ScaleUp);
	CHECK_TS_NORMAL(BattlePvP_Revive);
	CHECK_TS_NORMAL(PinkbeanAttackBuff);
	CHECK_TS_NORMAL(RandAreaAttack);
	CHECK_TS_NORMAL(BattlePvP_Mike_Shield);
	CHECK_TS_NORMAL(BattlePvP_Mike_Bugle);
	CHECK_TS_NORMAL(PinkbeanRelax);
	CHECK_TS_NORMAL(PinkbeanYoYoStack);
	CHECK_TS_NORMAL(NextAttackEnhance);
	CHECK_TS_NORMAL(AranBeyonderDamAbsorb);
	CHECK_TS_NORMAL(AranCombotempastOption);
	CHECK_TS_NORMAL(NautilusFinalAttack);
	CHECK_TS_NORMAL(ViperTimeLeap);
	CHECK_TS_NORMAL(RoyalGuardState);
	CHECK_TS_NORMAL(RoyalGuardPrepare);
	CHECK_TS_NORMAL(MichaelSoulLink);
	CHECK_TS_NORMAL(MichaelStanceLink);
	CHECK_TS_NORMAL(TriflingWhimOnOff);
	CHECK_TS_NORMAL(AddRangeOnOff);
	CHECK_TS_NORMAL(KinesisPsychicPoint);
	CHECK_TS_NORMAL(KinesisPsychicOver);
	CHECK_TS_NORMAL(KinesisPsychicShield);
	CHECK_TS_NORMAL(KinesisIncMastery);
	CHECK_TS_NORMAL(KinesisPsychicEnergeShield);
	CHECK_TS_NORMAL(BladeStance);
	CHECK_TS_NORMAL(DebuffActiveSkillHPCon);
	CHECK_TS_NORMAL(DebuffIncHP);
	CHECK_TS_NORMAL(BowMasterMortalBlow);
	CHECK_TS_NORMAL(AngelicBursterSoulResonance);
	CHECK_TS_NORMAL(Fever);
	CHECK_TS_NORMAL(IgnisRore);
	CHECK_TS_NORMAL(RpSiksin);
	CHECK_TS_NORMAL(TeleportMasteryRange);
	CHECK_TS_NORMAL(FireBarrier);
	CHECK_TS_NORMAL(ChangeFoxMan);
	CHECK_TS_NORMAL(FixCoolTime);
	CHECK_TS_NORMAL(IncMobRateDummy);
	CHECK_TS_NORMAL(AdrenalinBoost);
	CHECK_TS_NORMAL(AranSmashSwing);
	CHECK_TS_NORMAL(AranDrain);
	CHECK_TS_NORMAL(AranBoostEndHunt);
	CHECK_TS_NORMAL(HiddenHyperLinkMaximization);
	CHECK_TS_NORMAL(RWCylinder);
	CHECK_TS_NORMAL(RWCombination);
	CHECK_TS_NORMAL(RWMagnumBlow);
	CHECK_TS_NORMAL(RWBarrier);
	CHECK_TS_NORMAL(RWBarrierHeal);
	CHECK_TS_NORMAL(RWMaximizeCannon);
	CHECK_TS_NORMAL(RWOverHeat);
	CHECK_TS_NORMAL(RWMovingEvar);
	CHECK_TS_NORMAL(Stigma);

	if (flag & GET_TS_FLAG(SoulMP))
	{
		oPacket->Encode4(xSoulMP);
		oPacket->Encode4(rSoulMP);
	}

	if (flag & GET_TS_FLAG(FullSoulMP))
	{
		oPacket->Encode4(xFullSoulMP);
		oPacket->Encode4(rFullSoulMP);
	}

	int nCount = 0;
	oPacket->Encode2(nCount);
	for (int i = 0; i < nCount; ++i)
	{
		oPacket->Encode4(0); //mBuffedForSpecMap
		oPacket->Encode1(0); //bEnable
	}
	oPacket->Encode1((int)nDefenseAtt);
	oPacket->Encode1((int)nDefenseState);
	oPacket->Encode1((int)nPVPDamage);

	if (flag & GET_TS_FLAG(Dice))
	{
		for (int i = 0; i < 22; ++i)
			oPacket->Encode4(0);
	}

	if (flag & GET_TS_FLAG(KillingPoint))
		oPacket->Encode1((int)nKillingPoint);

	if (flag & GET_TS_FLAG(PinkbeanRollingGrade))
		oPacket->Encode1((int)nPinkbeanRollingGrade);

	if (flag & GET_TS_FLAG(Judgement))
		oPacket->Encode4(xJudgement);

	if (flag & GET_TS_FLAG(StackBuff))
		oPacket->Encode1((int)mStackBuff);

	if (flag & GET_TS_FLAG(PinkbeanRollingGrade))
		oPacket->Encode1((int)mTrinity);

	if (flag & GET_TS_FLAG(ElementalCharge))
	{
		oPacket->Encode1((int)mElementalCharge);
		oPacket->Encode2(wElementalCharge);
		oPacket->Encode1((int)uElementalCharge);
		oPacket->Encode1((int)zElementalCharge);
	}

	if (flag & GET_TS_FLAG(LifeTidal))
		oPacket->Encode4((int)nLifeTidal); //for mLifeTidal

	if (flag & GET_TS_FLAG(AntiMagicShell))
		oPacket->Encode1((int)bAntiMagicShell);

	if (flag & GET_TS_FLAG(Larkness))
	{
		//LarknessInfo::Encode
		for (int i = 0; i < 2; ++i)
		{
			oPacket->Encode4(0);
			oPacket->Encode4(0);
		}
		oPacket->Encode4(dgLarkness);
		oPacket->Encode4(lgLarkness);
	}

	if (flag & GET_TS_FLAG(IgnoreTargetDEF))
		oPacket->Encode1((int)bIgnoreTargetDEF);

	if (flag & GET_TS_FLAG(StopForceAtomInfo))
	{
		oPacket->Encode4(0); //nIdx
		oPacket->Encode4(0); //nCount
		oPacket->Encode4(0); //nWeaponID
		oPacket->Encode4(0); //
	}

	if (flag & GET_TS_FLAG(SmashStack))
		oPacket->Encode4(xSmashStack);

	if (flag & GET_TS_FLAG(MobZoneState))
		oPacket->Encode4(0);


	if (flag & GET_TS_FLAG(Slow))
		oPacket->Encode1((int)bSlow);

	if (flag & GET_TS_FLAG(IceAura))
		oPacket->Encode1((int)bIceAura);

	if (flag & GET_TS_FLAG(KnightsAura))
		oPacket->Encode1((int)bKnightsAura);

	if (flag & GET_TS_FLAG(IgnoreMobpdpR))
		oPacket->Encode1((int)bIgnoreMobpdpR);

	if (flag & GET_TS_FLAG(BDR))
		oPacket->Encode1((int)bBDR);

	if (flag & GET_TS_FLAG(DropRIncrease))
	{
		oPacket->Encode4(xDropRIncrease);
		oPacket->Encode1((int)bDropRIncrease);
	}

	if (flag & GET_TS_FLAG(PoseType))
		oPacket->Encode1((int)bPoseType);

	if (flag & GET_TS_FLAG(Beholder))
	{
		oPacket->Encode4(sBeholder);
		oPacket->Encode4(ssBeholder);
	}

	if (flag & GET_TS_FLAG(CrossOverChain))
		oPacket->Encode4(xCrossOverChain); //for xCrossOverChain

	if (flag & GET_TS_FLAG(Reincarnation))
		oPacket->Encode4(xReincarnation); //for xReincarnation

	if (flag & GET_TS_FLAG(ExtremeArchery))
	{
		oPacket->Encode4(bExtremeArchery);
		oPacket->Encode4(xExtremeArchery);
	}

	if (flag & GET_TS_FLAG(QuiverCatridge))
		oPacket->Encode4(xQuiverCatridge); 
	if (flag & GET_TS_FLAG(ImmuneBarrier))
		oPacket->Encode4(xImmuneBarrier);

	if (flag & GET_TS_FLAG(ZeroAuraStr))
		oPacket->Encode1((int)bZeroAuraStr);

	if (flag & GET_TS_FLAG(ZeroAuraSpd))
		oPacket->Encode1((int)bZeroAuraSpd);

	if (flag & GET_TS_FLAG(ArmorPiercing))
		oPacket->Encode4(bArmorPiercing); 

	if (flag & GET_TS_FLAG(SharpEyes))
		oPacket->Encode4(mSharpEyes);
	if (flag & GET_TS_FLAG(AdvancedBless))
		oPacket->Encode4(xAdvancedBless);
	if (flag & GET_TS_FLAG(DotHealHPPerSecond))
		oPacket->Encode4(xDotHealHPPerSecond);
	if (flag & GET_TS_FLAG(SpiritGuard))
		oPacket->Encode4(nSpiritGuard);

	if (flag & GET_TS_FLAG(KnockBack))
	{
		oPacket->Encode4(nKnockBack);
		oPacket->Encode4(bKnockBack);
	}

	if (flag & GET_TS_FLAG(ShieldAttack))
		oPacket->Encode4(xShieldAttack);

	if (flag & GET_TS_FLAG(SSFShootingAttack))
		oPacket->Encode4(xSSFShootingAttack);

	if (flag & GET_TS_FLAG(BMageAura)) 
	{
		oPacket->Encode4(xBMageAura);
		oPacket->Encode1((int)bBMageAura);
	}

	if (flag & GET_TS_FLAG(BattlePvP_Helena_Mark))
		oPacket->Encode4(cBattlePvP_Helena_Mark);
	if (flag & GET_TS_FLAG(PinkbeanAttackBuff))
		oPacket->Encode4(bPinkbeanAttackBuff);

	if (flag & GET_TS_FLAG(RoyalGuardState))
	{
		oPacket->Encode4(bRoyalGuardState);
		oPacket->Encode4(xRoyalGuardState);
	}
	if (flag & GET_TS_FLAG(MichaelSoulLink))
	{
		oPacket->Encode4(xMichaelSoulLink);
		oPacket->Encode1((int)bMichaelSoulLink);
		oPacket->Encode4(cMichaelSoulLink);
		oPacket->Encode4(yMichaelSoulLink);
	}

	if (flag & GET_TS_FLAG(AdrenalinBoost))
		oPacket->Encode1((int)cAdrenalinBoost);

	if (flag & GET_TS_FLAG(RWCylinder))
	{
		oPacket->Encode1((int)bRWCylinder);
		oPacket->Encode2(cRWCylinder);
	}

	if (flag & GET_TS_FLAG(RWMagnumBlow))
	{
		oPacket->Encode2(bRWMagnumBlow);
		oPacket->Encode1(xRWMagnumBlow);
	}

	//ViperEnergyCharge
	oPacket->Encode4(0);

	if (flag & GET_TS_FLAG(BladeStance))
		oPacket->Encode4(xBladeStance);

	if (flag & GET_TS_FLAG(DarkSight))
		oPacket->Encode4(cDarkSight);

	if (flag & GET_TS_FLAG(Stigma))
		oPacket->Encode4(bStigma);

	for (int i = 0; i < 8; ++i)
	{
		if (flag & TemporaryStat::TS_Flag(TemporaryStat::TS_EnergyCharged + i))
		{
			oPacket->Encode4(0);
			oPacket->Encode4(0);
			oPacket->Encode1(0);
			oPacket->Encode4(0);
		}
	}
	EncodeIndieTempStat(oPacket, flag);
	//EecodeIndieTempStat

	if (flag & GET_TS_FLAG(UsingScouter))
		oPacket->Encode4(nUsingScouter);

	oPacket->Encode2(1);
	oPacket->Encode1(0);
	oPacket->Encode1(0);
	oPacket->Encode1(0);
	oPacket->Encode4(0);
	oPacket->Encode1(0);
	oPacket->Encode4(0);
	oPacket->Encode4(0);

	printf("Encode Local TS : \n");
	oPacket->Print();
}

void SecondaryStat::EncodeForRemote(OutPacket * oPacket, TemporaryStat::TS_Flag & flag)
{
	flag.Encode(oPacket);
	// (&v743, &TemporaryStatBase::BUFF_58);
	if (flag & GET_TS_FLAG(Speed)) {
		oPacket->Encode1(0);
	}
	// (&v742, TemporaryStatBase::BUFF_72);
	if (flag & GET_TS_FLAG(ComboCounter)) {
		oPacket->Encode1(0);
	}
	// (&v741, TemporaryStatBase::BUFF_73);
	if (flag & GET_TS_FLAG(WeaponCharge)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v740, TemporaryStatBase::BUFF_328);
	if (flag & GET_TS_FLAG(ElementalCharge)) {
		oPacket->Encode2(0);
	}
	// (&v739, TemporaryStatBase::BUFF_68);
	if (flag & GET_TS_FLAG(Stun)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v738, TemporaryStatBase::BUFF_192);
	if (flag & GET_TS_FLAG(AmplifyDamage)) {
		oPacket->Encode1(0);
	}
	// (&v737, TemporaryStatBase::BUFF_71);
	if (flag & GET_TS_FLAG(Darkness)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v736, TemporaryStatBase::BUFF_70);
	if (flag & GET_TS_FLAG(Seal)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v735, TemporaryStatBase::BUFF_80);
	if (flag & GET_TS_FLAG(Weakness)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v734, TemporaryStatBase::BUFF_187);
	if (flag & GET_TS_FLAG(Frozen2)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v733, TemporaryStatBase::BUFF_81);
	if (flag & GET_TS_FLAG(Curse)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v732, TemporaryStatBase::BUFF_82);
	if (flag & GET_TS_FLAG(Slow)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v731, TemporaryStatBase::BUFF_186);
	if (flag & GET_TS_FLAG(WeaknessMdamage)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v730, TemporaryStatBase::BUFF_191);
	if (flag & GET_TS_FLAG(PVPDamageSkill)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v729, TemporaryStatBase::BUFF_171);
	if (flag & GET_TS_FLAG(DisOrder)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v728, TemporaryStatBase::BUFF_174);
	if (flag & GET_TS_FLAG(Explosion)) {
		oPacket->Encode1(0);
	}
	// (&v727, TemporaryStatBase::BUFF_172);
	if (flag & GET_TS_FLAG(Thread)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v726, TemporaryStatBase::BUFF_173);
	if (flag & GET_TS_FLAG(Team)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v725, TemporaryStatBase::BUFF_69);
	if (flag & GET_TS_FLAG(Poison)) {
		oPacket->Encode2(0);
	}
	// (&v724, TemporaryStatBase::BUFF_69);
	if (flag & GET_TS_FLAG(Poison)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v723, EnDecode4Byte::BUFF_76);
	if (flag & GET_TS_FLAG(ShadowPartner)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v722, &TemporaryStatBase::BUFF_61);
	if (flag & GET_TS_FLAG(DarkSight)) {  //sub_4381CC(v546, 1);

	}
	// (&v721, TemporaryStatBase::BUFF_67);
	if (flag & GET_TS_FLAG(SoulArrow)) {  //sub_438514(v546, 1);
	}
	// (&v720, TemporaryStatBase::BUFF_83);
	if (flag & GET_TS_FLAG(Morph)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v719, TemporaryStatBase::BUFF_99);
	if (flag & GET_TS_FLAG(Ghost)) {
		oPacket->Encode2(0);
	}
	// (&v718, TemporaryStatBase::BUFF_89);
	if (flag & GET_TS_FLAG(Attract)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v717, TemporaryStatBase::BUFF_242);
	if (flag & GET_TS_FLAG(Magnet)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v716, TemporaryStatBase::BUFF_243);
	if (flag & GET_TS_FLAG(MagnetArea)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v715, TemporaryStatBase::BUFF_90);
	if (flag & GET_TS_FLAG(NoBulletConsume)) {
		oPacket->Encode4(0);
	}
	// (&v714, TemporaryStatBase::BUFF_96);
	if (flag & GET_TS_FLAG(BanMap)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v713, TemporaryStatBase::BUFF_100);
	if (flag & GET_TS_FLAG(Barrier)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v712, TemporaryStatBase::BUFF_109);
	if (flag & GET_TS_FLAG(DojangShield)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v711, TemporaryStatBase::BUFF_101);
	if (flag & GET_TS_FLAG(ReverseInput)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v710, TemporaryStatBase::BUFF_103);
	if (flag & GET_TS_FLAG(RespectPImmune)) {
		oPacket->Encode4(0);
	}
	// (&v709, TemporaryStatBase::BUFF_104);
	if (flag & GET_TS_FLAG(RespectMImmune)) {
		oPacket->Encode4(0);
	}
	// (&v708, TemporaryStatBase::BUFF_105);
	if (flag & GET_TS_FLAG(DefenseAtt)) {
		oPacket->Encode4(0);
	}
	// (&v707, TemporaryStatBase::BUFF_106);
	if (flag & GET_TS_FLAG(DefenseState)) {
		oPacket->Encode4(0);
	}
	// (&v706, TemporaryStatBase::BUFF_107);
	if (flag & GET_TS_FLAG(DojangBerserk)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v705, TemporaryStatBase::BUFF_108);
	if (flag & GET_TS_FLAG(DojangInvincible)) {//  sub_43975C(v546, 1);

	}
	// (&v704, TemporaryStatBase::BUFF_120);
	if (flag & GET_TS_FLAG(ExpBuffRate)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	//// (&v703, TemporaryStatBase::BUFF_416);
	//if (flag & GET_TS_FLAG(MapleBuffStat.PLAYERS_BUFF11)) {
	//	oPacket->Encode2(0);
	//	oPacket->Encode4(0);
	//}
	// (&v702, TemporaryStatBase::BUFF_122);
	if (flag & GET_TS_FLAG(StopMotion)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v701, TemporaryStatBase::BUFF_123);
	if (flag & GET_TS_FLAG(Fear)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v700, TemporaryStatBase::BUFF_124);
	if (flag & GET_TS_FLAG(HiddenPieceOn)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v699, TemporaryStatBase::BUFF_126);
	if (flag & GET_TS_FLAG(MagicResistance)) {
		oPacket->Encode4(0);
	}
	// (&v698, TemporaryStatBase::BUFF_129);
	if (flag & GET_TS_FLAG(Frozen)) {//  sub_43A18E(v546, 1);

	}
	// (&v697, TemporaryStatBase::BUFF_130);
	if (flag & GET_TS_FLAG(AssistCharge)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v696, TemporaryStatBase::BUFF_188);
	if (flag & GET_TS_FLAG(Frozen2)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v695, TemporaryStatBase::BUFF_169);
	if (flag & GET_TS_FLAG(Bless)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v694, TemporaryStatBase::BUFF_133);
	if (flag & GET_TS_FLAG(DrawBack)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v693, TemporaryStatBase::BUFF_135);
	if (flag & GET_TS_FLAG(FinalCut)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v692, TemporaryStatBase::BUFF_148);
	if (flag & GET_TS_FLAG(HowlingMaxMP)) {
		oPacket->Encode1(0);
	}
	// (&v691, TemporaryStatBase::BUFF_309);
	if (flag & GET_TS_FLAG(OnCapsule)) {
		oPacket->Encode1(0);
	}
	// (&v690, TemporaryStatBase::BUFF_309);
	if (flag & GET_TS_FLAG(OnCapsule)) {//  sub_43A83C(v546, 1);

	}
	// (&v689, TemporaryStatBase::BUFF_156);
	if (flag & GET_TS_FLAG(Sneak)) {//  sub_43AB0C(v546, 1);

	}
	// (&v688, TemporaryStatBase::BUFF_137);
	if (flag & GET_TS_FLAG(BeastFormDamageUp)) {//  sub_43A512(v546, 1);

	}
	// (&v687, TemporaryStatBase::BUFF_157);
	if (flag & GET_TS_FLAG(Mechanic)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v686, TemporaryStatBase::BUFF_160);
	if (flag & GET_TS_FLAG(BlessingArmor)) {// sub_43AC1A(v546, 1);

	}
	// (&v685, TemporaryStatBase::BUFF_245);
	if (flag & GET_TS_FLAG(BlessingArmorIncPAD)) {//  sub_43AC74(v546, 1);

	}
	// (&v684, TemporaryStatBase::BUFF_166);
	if (flag & GET_TS_FLAG(Inflation)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v683, TemporaryStatBase::BUFF_175);
	if (flag & GET_TS_FLAG(BuffLimit)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v682, TemporaryStatBase::BUFF_182);
	if (flag & GET_TS_FLAG(DarkTornado)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v681, TemporaryStatBase::BUFF_190);
	if (flag & GET_TS_FLAG(IceKnight)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v680, TemporaryStatBase::BUFF_113);
	if (flag & GET_TS_FLAG(HideAttack)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v679, TemporaryStatBase::BUFF_196);
	if (flag & GET_TS_FLAG(IncMaxMP)) {
	} //sub_43ACCE(v546, 1);
	  // (&v678, TemporaryStatBase::BUFF_203);
	if (flag & GET_TS_FLAG(DevilishPower)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v677, EnDecode4Byte::BUFF_205);
	if (flag & GET_TS_FLAG(SpiritLink)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v676, TemporaryStatBase::BUFF_207);
	if (flag & GET_TS_FLAG(Event)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v675, TemporaryStatBase::BUFF_215);
	if (flag & GET_TS_FLAG(Event2)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v674, TemporaryStatBase::BUFF_222);
	if (flag & GET_TS_FLAG(DeathMark)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v673, TemporaryStatBase::BUFF_241);
	if (flag & GET_TS_FLAG(PainMark)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v672, TemporaryStatBase::BUFF_224);
	if (flag & GET_TS_FLAG(Lapidification)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v671, TemporaryStatBase::BUFF_244);
	if (flag & GET_TS_FLAG(VampDeath)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v670, TemporaryStatBase::BUFF_257);
	if (flag & GET_TS_FLAG(VampDeathSummon)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v669, TemporaryStatBase::BUFF_225);
	if (flag & GET_TS_FLAG(VenomSnake)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// 預設Buff********
	// (&v668, TemporaryStatBase::BUFF_230);
	if (flag & GET_TS_FLAG(PyramidEffect)) {
		oPacket->Encode4(-1);
	}
	// 預設Buff********
	// (&v667, TemporaryStatBase::BUFF_231);
	if (flag & GET_TS_FLAG(KillingPoint)) {
		oPacket->Encode1(0);
	}
	// 預設Buff********
	// (&v666, TemporaryStatBase::BUFF_434);
	if (flag & GET_TS_FLAG(PinkbeanRollingGrade)) {
		oPacket->Encode1(0);
	}
	// (&v665, TemporaryStatBase::BUFF_234);
	if (flag & GET_TS_FLAG(IgnoreTargetDEF)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v664, TemporaryStatBase::BUFF_236);
	if (flag & GET_TS_FLAG(Invisible)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v663, TemporaryStatBase::BUFF_239);
	if (flag & GET_TS_FLAG(Judgement)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v662, TemporaryStatBase::BUFF_246);
	if (flag & GET_TS_FLAG(KeyDownAreaMoving)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v661, TemporaryStatBase::BUFF_248);
	if (flag & GET_TS_FLAG(StackBuff)) {
		oPacket->Encode2(0);
	}
	// (&v660, TemporaryStatBase::BUFF_249);
	if (flag & GET_TS_FLAG(BlessOfDarkness)) {
		oPacket->Encode4(0);
	}
	// (&v659, TemporaryStatBase::BUFF_247);
	if (flag & GET_TS_FLAG(Larkness)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v658, TemporaryStatBase::BUFF_255);
	if (flag & GET_TS_FLAG(ReshuffleSwitch)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v657, TemporaryStatBase::BUFF_256);
	if (flag & GET_TS_FLAG(SpecialAction)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v656, TemporaryStatBase::BUFF_258);
	if (flag & GET_TS_FLAG(StopForceAtomInfo)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v655, EnDecode4Byte::BUFF_259);
	if (flag & GET_TS_FLAG(SoulGazeCriDamR)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v654, EnDecode4Byte::BUFF_261);
	if (flag & GET_TS_FLAG(PowerTransferGauge)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v653, TemporaryStatBase::BUFF_262);
	if (flag & GET_TS_FLAG(AffinitySlug)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v652, TemporaryStatBase::BUFF_271);
	if (flag & GET_TS_FLAG(SoulExalt)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v651, TemporaryStatBase::BUFF_125);
	if (flag & GET_TS_FLAG(MagicShield)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v650, TemporaryStatBase::BUFF_253);
	if (flag & GET_TS_FLAG(SmashStack)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v649, TemporaryStatBase::BUFF_266);
	if (flag & GET_TS_FLAG(MobZoneState)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v648, TemporaryStatBase::BUFF_267);
	if (flag & GET_TS_FLAG(GiveMeHeal)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v647, TemporaryStatBase::BUFF_268);
	if (flag & GET_TS_FLAG(TouchMe)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v646, TemporaryStatBase::BUFF_269);
	if (flag & GET_TS_FLAG(ComboUnlimited)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v645, TemporaryStatBase::BUFF_269);
	if (flag & GET_TS_FLAG(ComboUnlimited)) {
		oPacket->Encode4(0);
	}
	// (&v644, TemporaryStatBase::BUFF_270);
	if (flag & GET_TS_FLAG(ComboUnlimited)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v643, TemporaryStatBase::BUFF_272);
	if (flag & GET_TS_FLAG(IgnorePCounter)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v642, TemporaryStatBase::BUFF_273);
	if (flag & GET_TS_FLAG(IgnoreAllCounter)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v641, TemporaryStatBase::BUFF_274);
	if (flag & GET_TS_FLAG(IgnorePImmune)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v640, TemporaryStatBase::BUFF_275);
	if (flag & GET_TS_FLAG(IgnoreAllImmune)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v639, TemporaryStatBase::BUFF_276);
	if (flag & GET_TS_FLAG(FinalJudgement)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v638, TemporaryStatBase::BUFF_333);
	if (flag & GET_TS_FLAG(KnightsAura)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v637, TemporaryStatBase::BUFF_277);
	if (flag & GET_TS_FLAG(IceAura)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v636, TemporaryStatBase::BUFF_278);
	if (flag & GET_TS_FLAG(FireAura)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v635, TemporaryStatBase::BUFF_279);
	if (flag & GET_TS_FLAG(VengeanceOfAngel)) {  //sub_43D488(v546, 1);

	}
	// (&v634, TemporaryStatBase::BUFF_280);
	if (flag & GET_TS_FLAG(HeavensDoor)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v633, TemporaryStatBase::BUFF_202);
	if (flag & GET_TS_FLAG(DamAbsorbShield)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v632, TemporaryStatBase::BUFF_250);
	if (flag & GET_TS_FLAG(AntiMagicShell)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v631, TemporaryStatBase::BUFF_134);
	if (flag & GET_TS_FLAG(NotDamaged)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v630, TemporaryStatBase::BUFF_285);
	if (flag & GET_TS_FLAG(BleedingToxin)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v629, TemporaryStatBase::BUFF_111);
	if (flag & GET_TS_FLAG(WindBreakerFinal)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v628, TemporaryStatBase::BUFF_286);
	if (flag & GET_TS_FLAG(IgnoreMobDamR)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v627, TemporaryStatBase::BUFF_287);
	if (flag & GET_TS_FLAG(Asura)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v626, TemporaryStatBase::BUFF_289);
	if (flag & GET_TS_FLAG(UnityOfPower)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v625, TemporaryStatBase::BUFF_290);
	if (flag & GET_TS_FLAG(Stimulate)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v624, EnDecode4Byte::BUFF_291);
	if (flag & GET_TS_FLAG(ReturnTeleport)) {
		oPacket->Encode1(0);
		oPacket->Encode4(0);
	}
	// (&v623, TemporaryStatBase::BUFF_295);
	if (flag & GET_TS_FLAG(BDR)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v622, TemporaryStatBase::BUFF_300);
	if (flag & GET_TS_FLAG(ExceedOverload)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v621, TemporaryStatBase::BUFF_302);
	if (flag & GET_TS_FLAG(BuckShot)) {
		oPacket->Encode1(0);
		oPacket->Encode4(0);
	}
	// (&v620, TemporaryStatBase::BUFF_304);
	if (flag & GET_TS_FLAG(SurplusSupply)) {
		oPacket->Encode1(0/*chr.getXenonSurplus()*/);
	}
	// (&v619, TemporaryStatBase::BUFF_307);
	if (flag & GET_TS_FLAG(NewFlying)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v618, EnDecode4Byte::BUFF_339);
	if (flag & GET_TS_FLAG(NaviFlying)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v617, TemporaryStatBase::BUFF_308);
	if (flag & GET_TS_FLAG(AmaranthGenerator)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v616, TemporaryStatBase::BUFF_310);
	if (flag & GET_TS_FLAG(CygnusElementSkill)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v615, TemporaryStatBase::BUFF_311);
	if (flag & GET_TS_FLAG(StrikerHyperElectric)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v614, TemporaryStatBase::BUFF_312);
	if (flag & GET_TS_FLAG(EventPointAbsorb)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v613, TemporaryStatBase::BUFF_313);
	if (flag & GET_TS_FLAG(EventAssemble)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v612, TemporaryStatBase::BUFF_317);
	if (flag & GET_TS_FLAG(Albatross)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v611, TemporaryStatBase::BUFF_318);
	if (flag & GET_TS_FLAG(Translucence)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v610, TemporaryStatBase::BUFF_319);
	if (flag & GET_TS_FLAG(PoseType)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v609, TemporaryStatBase::BUFF_320);
	if (flag & GET_TS_FLAG(LightOfSpirit)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v608, TemporaryStatBase::BUFF_321);
	if (flag & GET_TS_FLAG(ElementSoul)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v607, TemporaryStatBase::BUFF_322);
	if (flag & GET_TS_FLAG(GlimmeringTime)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v606, TemporaryStatBase::BUFF_332);
	if (flag & GET_TS_FLAG(Reincarnation)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v605, TemporaryStatBase::BUFF_164);
	if (flag & GET_TS_FLAG(Beholder)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v604, EnDecode4Byte::BUFF_340);
	if (flag & GET_TS_FLAG(QuiverCatridge)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v603, TemporaryStatBase::BUFF_344);
	if (flag & GET_TS_FLAG(ArmorPiercing)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v602, TemporaryStatBase::BUFF_342);
	if (flag & GET_TS_FLAG(UserControlMob)) {//  sub_43E8EC(v546, 1);
	}
	// 預設Buff********
	// (&v601, TemporaryStatBase::BUFF_345);
	if (flag & GET_TS_FLAG(ZeroAuraStr)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// 預設Buff********
	// (&v600, TemporaryStatBase::BUFF_346);
	if (flag & GET_TS_FLAG(ZeroAuraSpd)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v599, EnDecode4Byte::BUFF_343);
	if (flag & GET_TS_FLAG(ImmuneBarrier)) {
		oPacket->Encode4(0);
	}
	// (&v598, EnDecode4Byte::BUFF_343);
	if (flag & GET_TS_FLAG(ImmuneBarrier)) {
		oPacket->Encode4(0);
	}
	// (&v597, TemporaryStatBase::BUFF_420);
	if (flag & GET_TS_FLAG(NightLordMark)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	//// (&v596, TemporaryStatBase::BUFF_421);
	//if (flag & GET_TS_FLAG(MapleBuffStat.ANIMAL_SELECT)) {
	//	oPacket->Encode2(0);
	//	oPacket->Encode4(0);
	//}
	// (&v595, TemporaryStatBase::BUFF_422);
	if (flag & GET_TS_FLAG(FireBarrier)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v594, TemporaryStatBase::BUFF_458);
	if (flag & GET_TS_FLAG(DebuffIncHP)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v593, EnDecode4Byte::BUFF_425);
	/*if (flag & GET_TS_FLAG(MapleBuffStat.IDA_SPECIAL_BUFF_4)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}*/
	// (&v592, TemporaryStatBase::BUFF_401);
	if (flag & GET_TS_FLAG(FireBarrier)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v591, TemporaryStatBase::BUFF_326);
	if (flag & GET_TS_FLAG(FullSoulMP)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
		oPacket->Encode4(0);
	}
	// (&v590, TemporaryStatBase::BUFF_250);
	if (flag & GET_TS_FLAG(AntiMagicShell)) {
		oPacket->Encode1(0);
	}
	// (&v589, EnDecode4Byte::BUFF_138);
	if (flag & GET_TS_FLAG(Dance)) {
		oPacket->Encode4(0);
		oPacket->Encode4(0);
	}
	// (&v588, TemporaryStatBase::BUFF_358);
	if (flag & GET_TS_FLAG(SpiritGuard)) {
		oPacket->Encode4(0);
		oPacket->Encode4(0);
	}
	// (&v587, TemporaryStatBase::BUFF_363);
	if (flag & GET_TS_FLAG(ComboTempest)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v586, TemporaryStatBase::BUFF_303);
	if (flag & GET_TS_FLAG(FireBomb)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v585, TemporaryStatBase::BUFF_379);
	if (flag & GET_TS_FLAG(ComplusionSlant)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v584, TemporaryStatBase::BUFF_380);
	if (flag & GET_TS_FLAG(JaguarSummoned)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// 預設Buff********
	// (&v583, TemporaryStatBase::BUFF_385);
	if (flag & GET_TS_FLAG(BMageAura)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v582, TemporaryStatBase::BUFF_386);
	if (flag & GET_TS_FLAG(DarkLighting)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v581, TemporaryStatBase::BUFF_387);
	if (flag & GET_TS_FLAG(AttackCountX)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v580, TemporaryStatBase::BUFF_399);
	if (flag & GET_TS_FLAG(FireBarrier)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v579, TemporaryStatBase::BUFF_233);
	if (flag & GET_TS_FLAG(KeyDownMoving)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v578, TemporaryStatBase::BUFF_444);
	if (flag & GET_TS_FLAG(MichaelSoulLink)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v577, TemporaryStatBase::BUFF_452);
	if (flag & GET_TS_FLAG(KinesisPsychicEnergeShield)) {
		oPacket->Encode4(0);
		oPacket->Encode4(0);
	}
	// (&v576, TemporaryStatBase::BUFF_453);
	if (flag & GET_TS_FLAG(KinesisPsychicShield)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v575, TemporaryStatBase::BUFF_453);
	if (flag & GET_TS_FLAG(KinesisPsychicShield)) {
		oPacket->Encode4(0);
	}
	// (&v574, TemporaryStatBase::BUFF_458);
	if (flag & GET_TS_FLAG(DebuffIncHP)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v573, TemporaryStatBase::BUFF_402);
	if (flag & GET_TS_FLAG(ChangeFoxMan)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v572, TemporaryStatBase::BUFF_402);
	if (flag & GET_TS_FLAG(ChangeFoxMan)) {
		oPacket->Encode4(0);
	}
	//// (&v571, TemporaryStatBase::BUFF_409);
	//if (flag & GET_TS_FLAG(MILITARY_MIGHT_MMPR)) {
	//	oPacket->Encode2(0);
	//	oPacket->Encode4(0);
	//}
	//// (&v570, TemporaryStatBase::BUFF_403);
	//if (flag & GET_TS_FLAG(MapleBuffStat.BATTOUJUTSU_SOUL)) {
	//	oPacket->Encode2(0);
	//	oPacket->Encode4(0);
	//}
	// (&v569, TemporaryStatBase::BUFF_406);
	/*if (flag & GET_TS_FLAG(MapleBuffStat.SHIKIGAMI)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}*/
	// (&v568, TemporaryStatBase::BUFF_407);
	if (flag & GET_TS_FLAG(HayatoChangeMode)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v567, TemporaryStatBase::BUFF_408);
	if (flag & GET_TS_FLAG(HayatoCombo)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v566, TemporaryStatBase::BUFF_413);
	/*if (flag & GET_TS_FLAG(HITOKIRI_STRIKE)) {
		oPacket->Encode4(0);
	}*/
	// (&v565, TemporaryStatBase::BUFF_399);
	if (flag & GET_TS_FLAG(FireBarrier)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v564, TemporaryStatBase::BUFF_415);
	/*if (flag & GET_TS_FLAG(BELLFLOWER_BARRIER_BOSS_DAMAGE)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}*/
	// (&v563, TemporaryStatBase::BUFF_86);
	if (flag & GET_TS_FLAG(Stance)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v562, TemporaryStatBase::BUFF_467);
	if (flag & GET_TS_FLAG(KinesisPsychicEnergeShield)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v561, TemporaryStatBase::BUFF_417);
	if (flag & GET_TS_FLAG(AngelicBursterSoulSeeker)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v560, TemporaryStatBase::BUFF_418);
	/*if (flag & GET_TS_FLAG(BLACKHEART_CURSE)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}*/
	// (&v559, TemporaryStatBase::BUFF_404);
	/*if (flag & GET_TS_FLAG(COUNTERATTACK)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}*/
	// (&v558, EnDecode4Byte::BUFF_146);
	if (flag & GET_TS_FLAG(Cyclone)) {
		oPacket->Encode4(0);
		oPacket->Encode4(0);
	}
	// (&v557, TemporaryStatBase::BUFF_469);
	if (flag & GET_TS_FLAG(BladeStance)) {
		oPacket->Encode2(0);
		oPacket->Encode4(0);
	}
	// (&v556, TemporaryStatBase::BUFF_319);
	if (flag & GET_TS_FLAG(PoseType)) {
		oPacket->Encode1(0);
	}

	// 預設Buff********
	oPacket->Encode1(0);
	oPacket->Encode1(0);
	oPacket->Encode1(0);

	// 預設Buff********
	// (&v555, TemporaryStatBase::BUFF_345);
	if (flag & GET_TS_FLAG(ZeroAuraStr)) {
		oPacket->Encode1(0);
	}
	// 預設Buff********
	// (&v554, TemporaryStatBase::BUFF_346);
	if (flag & GET_TS_FLAG(ZeroAuraSpd)) {
		oPacket->Encode1(0);
	}
	// 預設Buff********
	// (&v553, TemporaryStatBase::BUFF_385);
	if (flag & GET_TS_FLAG(BMageAura)) {
		oPacket->Encode1(0);
	}
	// 預設Buff********
	// (&v552, TemporaryStatBase::BUFF_427);
	if (flag & GET_TS_FLAG(BattlePvP_Helena_Mark)) {
		oPacket->Encode4(0);
		oPacket->Encode4(0);
		oPacket->Encode4(0);
	}
	// 預設Buff********
	// (&v551, TemporaryStatBase::BUFF_429);
	if (flag & GET_TS_FLAG(BattlePvP_LangE_Protection)) {
		oPacket->Encode4(0);
		oPacket->Encode4(0);
	}
	// (&v550, TemporaryStatBase::BUFF_444);
	if (flag & GET_TS_FLAG(MichaelSoulLink)) {
		oPacket->Encode4(0);
		oPacket->Encode1(0);
		oPacket->Encode4(0);
		oPacket->Encode4(0);
	}

	// 預設Buff********
	////////////////////////sub_AEC05E(v535 + 9896, a3);
	/*StopForceAtom pStopForceAtom = new StopForceAtom();
	pStopForceAtom.Encode(mplew);*/
	oPacket->Encode4(0);
	oPacket->Encode4(0);
	oPacket->Encode4(0);
	oPacket->Encode4(0);
	///////////////////////

	// 預設Buff********
	oPacket->Encode4(0); // nViperEnergyCharge

					   // 預設Buff********
	//TSIndex.encodeAll(mplew, chr);

	if (flag & GET_TS_FLAG(NewFlying)) {
		oPacket->Encode4(0);
	}
}

void SecondaryStat::EncodeIndieTempStat(OutPacket * oPacket, TemporaryStat::TS_Flag & flag)
{
	int nValue, rValue, tValue = 0;
	for (int i = 0; i <= TemporaryStat::TS_INDIE_STAT_COUNT; ++i)
	{
		if (flag & TemporaryStat::TS_Flag(i))
		{
			nValue = m_mSetByTS[i].second.size() != 0 ? *(m_mSetByTS[i].second[0]) : 0;
			rValue = m_mSetByTS[i].second.size() != 0 ? *(m_mSetByTS[i].second[1]) : 0;
			tValue = m_mSetByTS[i].second.size() != 0 ? *(m_mSetByTS[i].second[2]) : 0;
			oPacket->Encode4(1);
			oPacket->Encode4(rValue);
			oPacket->Encode4(nValue);
			oPacket->Encode4(INT_MAX);
			oPacket->Encode4(1);
			oPacket->Encode4(tValue);
			oPacket->Encode4(0);
		}
	}
}

bool SecondaryStat::EnDecode4Byte(TemporaryStat::TS_Flag & flag)
{
	if ((flag & GET_TS_FLAG(CarnivalDefence))
		|| (flag & GET_TS_FLAG(SpiritLink))
		|| (flag & GET_TS_FLAG(DojangLuckyBonus))
		|| (flag & GET_TS_FLAG(SoulGazeCriDamR))
		|| (flag & GET_TS_FLAG(PowerTransferGauge))
		|| (flag & GET_TS_FLAG(ReturnTeleport))
		|| (flag & GET_TS_FLAG(ShadowPartner))
		|| (flag & GET_TS_FLAG(SetBaseDamage))
		|| (flag & GET_TS_FLAG(QuiverCatridge))
		|| (flag & GET_TS_FLAG(ImmuneBarrier))
		|| (flag & GET_TS_FLAG(NaviFlying))
		|| (flag & GET_TS_FLAG(Dance))
		|| (flag & GET_TS_FLAG(SetBaseDamageByBuff))
		|| (flag & GET_TS_FLAG(DotHealHPPerSecond))
		|| (flag & GET_TS_FLAG(MagnetArea))
		|| (flag & GET_TS_FLAG(RideVehicle)))
		return true;
	printf("EnDecode4Byte [False]\n");
	return false;
}

void SecondaryStat::ResetByTime(int tCur)
{
}
