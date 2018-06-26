#include "Net\OutPacket.h"
#include "MobStat.h"

MobStat::MobStat()
{

}

MobStat::~MobStat()
{

}

bool MobStat::CHECK_MS_NORML(TS_Flag & flags, MobStatNumbers eMSN)
{
	if (flags & MobStat::TS_Flag::TS_Flag(eMSN)) {
		return true;
	}
	else
	{
		return false;
	}
}

MobStat::TS_Flag::TS_Flag(int dwFlagValue)
{
	m_nFlagPos = dwFlagValue / 32;
	int nValue = (1 << (31 - (dwFlagValue % 32)));
	for (int i = 0; i < FLAG_COUNT; ++i)
		m_aData[i] = 0;
	m_aData[m_nFlagPos] |= nValue;
}

void MobStat::TS_Flag::Encode(OutPacket * oPacket)
{
	for (int i = 0; i < FLAG_COUNT; ++i) {
		printf("Encode TS Flag : [%d] = %d\n", i, m_aData[i]);
		oPacket->Encode4(m_aData[i]);
	}
}

MobStat::TS_Flag & MobStat::TS_Flag::operator|=(const TS_Flag & rhs)
{
	m_aData[rhs.m_nFlagPos] |= rhs.m_aData[rhs.m_nFlagPos];
	return *this;
}

bool MobStat::TS_Flag::operator&(const TS_Flag & rhs)
{
	return (m_aData[rhs.m_nFlagPos] & rhs.m_aData[rhs.m_nFlagPos]) != 0;
}

MobStat::TS_Flag MobStat::TS_Flag::GetDefault()
{
	TS_Flag ret(0);
	ret.m_aData[0] = 0;
	ret |= TS_Flag::TS_Flag(MobStatNumbers::MOB_STAT_Dark);
	ret |= TS_Flag::TS_Flag(MobStatNumbers::MOB_STAT_Mystery);
	ret |= TS_Flag::TS_Flag(MobStatNumbers::MOB_STAT_MultiDamSkill);
	ret |= TS_Flag::TS_Flag(MobStatNumbers::MOB_STAT_RWChoppingHammer);
	ret |= TS_Flag::TS_Flag(MobStatNumbers::MOB_STAT_Burned);
	ret |= TS_Flag::TS_Flag(MobStatNumbers::MOB_STAT_BalogDisable);
	ret |= TS_Flag::TS_Flag(MobStatNumbers::MOB_STAT_ExchangeAttack);
	ret |= TS_Flag::TS_Flag(MobStatNumbers::MOB_STAT_AddBuffStat);
	ret |= TS_Flag::TS_Flag(MobStatNumbers::MOB_STAT_LinkTeam);
	ret |= TS_Flag::TS_Flag(MobStatNumbers::MOB_STAT_SoulExplosion);
	ret |= TS_Flag::TS_Flag(MobStatNumbers::MOB_STAT_SeperateSoulP);
	ret |= TS_Flag::TS_Flag(MobStatNumbers::MOB_STAT_SeperateSoulC);
	ret |= TS_Flag::TS_Flag(MobStatNumbers::MOB_STAT_Ember);
	ret |= TS_Flag::TS_Flag(MobStatNumbers::MOB_STAT_TrueSight);
	ret |= TS_Flag::TS_Flag(MobStatNumbers::MOB_STAT_Laser);
	ret |= TS_Flag::TS_Flag(MobStatNumbers::MOB_STAT_StatResetSkill);
	ret |= TS_Flag::TS_Flag(MobStatNumbers::MOB_STAT_COUNT);
	return ret;
}

void MobStat::EncodeTemporary(OutPacket *oPacket, TS_Flag & flags) {
	flags.Encode(oPacket);
	
	for (int i = 0; i < MobStatNumbers::MOB_STAT_Burned; i++)
	{
		if (flags & TS_Flag::TS_Flag(i))
		{
			oPacket->Encode4(0);
			oPacket->Encode4(0);
			oPacket->Encode2(0);
		}
	}
	//for (MonsterStatusEffect buff : buffs) {
	//	mobstat.add(buff.getStati());
	//	if (buff.getStati().getBitNumber() < MonsterStatus.M_Burned.getBitNumber()) {
	//		oPacket->Encode4(buff.getX()); // n
	//		if (buff.getMobSkill() != null) {
	//			oPacket->Encode2(buff.getMobSkill().getSkillId());
	//			oPacket->Encode2(buff.getMobSkill().getSkillLevel());
	//		}
	//		else {
	//			oPacket->Encode4(buff.getSkill() > 0 ? buff.getSkill() : 0); // r
	//		}
	//		oPacket->Encode2((short)((buff.getCancelTask() - System.currentTimeMillis()) / 1000)); //t
	//	}
	//}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_PDR)) {
		oPacket->Encode4(0); // c
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_MDR)) {
		oPacket->Encode4(0); // c
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_PCounter)) {
		oPacket->Encode4(0); // w
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_MCounter)) {
		oPacket->Encode4(0); // w
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_PCounter) || CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_MCounter)) {
		int n = 0, c = 0, b = 0;
		/*for (MonsterStatusEffect buff : buffs) {
			if (buff.getStati().equals(MonsterStatus.M_PCounter) || buff.getStati().equals(MonsterStatus.M_MCounter)) {
				n = buff.getX();
			}
		}*/
		oPacket->Encode4(c); // c
		oPacket->Encode1(b); // b boolean
		oPacket->Encode4(n); // n
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_Fatality)) {
		oPacket->Encode4(0); // w
		oPacket->Encode4(0); // u
		oPacket->Encode4(0); // p
		oPacket->Encode4(0); // y
						   // m
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_Explosion)) {
		oPacket->Encode4(0); // w
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_AddBuffStat)) {
		int result = 0;
		oPacket->Encode1(result);
		if (result != 0) {
			oPacket->Encode4(0); // nPAD
			oPacket->Encode4(0); // nMAD
			oPacket->Encode4(0); // nPDR
			oPacket->Encode4(0); // nMDR
		}
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_DeadlyCharge)) {
		oPacket->Encode4(0); // p
		oPacket->Encode4(0); // w
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_Incizing)) {
		oPacket->Encode4(0); // w
		oPacket->Encode4(0); // u
		oPacket->Encode4(0); // p
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_Speed)) {
		oPacket->Encode1(0); // m
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_BMageDebuff)) {
		oPacket->Encode4(0); // c
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_DarkLightning)) {
		oPacket->Encode4(0); // c
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_BattlePvP_Helena_Mark)) {
		oPacket->Encode4(0); // c
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_MultiPMDR)) { // (186++)
		oPacket->Encode4(0); // c
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_Freeze)) {
		oPacket->Encode4(0);
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_Burned)) {
		//List<MonsterStatusEffect> bleedBuffs = new ArrayList();
		//MonsterStatusEffect
		/*buffs.stream().filter((buff) -> (buff.getStati().getBitNumber() == MonsterStatus.M_Burned.getBitNumber() && buff.getMobSkill() != null)).forEach((buff) -> {
			bleedBuffs.add(buff);
		});*/
		oPacket->Encode1(0/*bleedBuffs.size()*/);
		//if (bleedBuffs.size() > 0) {
		//	bleedBuffs.stream().forEach((buff) -> {
		//		oPacket->Encode4(8695624); // dwCharacterID
		//		oPacket->Encode4(buff.getSkill()); // nSkillID §Þ¯àID
		//		oPacket->Encode4(7100); // nDamage ¨C¬í¶Ë®`?
		//		oPacket->Encode4(1000); // tInterval ©µ¿ð²@¬í : dotInterval * 1000
		//		oPacket->Encode4(187277775); // tEnd
		//		oPacket->Encode4(16450); // tDotAnimation
		//		oPacket->Encode4(15); // nDotCount dotTime
		//		oPacket->Encode4(0); // nSuperPos
		//		oPacket->Encode4(1); // tAttackDelay
		//		oPacket->Encode4(7100); // nDotTickIdx ¨C¬í¶Ë®`?
		//		oPacket->Encode4(0); // nDotTickDamR
		//		oPacket->Encode4(0);
		//		oPacket->Encode4(0);
		//	});
		//}
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_Invincible)) {
		oPacket->Encode1(0); // n // nInvincible
		oPacket->Encode1(0); // b // bBalogDisable 
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_ExchangeAttack)) {
		oPacket->Encode1(0); // b
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_AddDamParty)) {
		oPacket->Encode4(0); // w
		oPacket->Encode4(0); // p
		oPacket->Encode4(0); // c
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_LinkTeam)) {
		oPacket->EncodeStr(""); // sLinkTeam
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_SoulExplosion)) {
		oPacket->Encode4(0); // n
		oPacket->Encode4(0); // r
		oPacket->Encode4(0); // w
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_SeperateSoulP)) {
		oPacket->Encode4(0); // n
		oPacket->Encode4(0); // r
		oPacket->Encode2(0); // t
		oPacket->Encode4(0); // w
		oPacket->Encode4(0); // u
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_SeperateSoulC)) {
		oPacket->Encode4(0); // n
		oPacket->Encode4(0); // r
		oPacket->Encode2(0); // t
		oPacket->Encode4(0); // w
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_Ember)) {
		oPacket->Encode4(0); // n
		oPacket->Encode4(0); // r
		oPacket->Encode4(0); // t
		oPacket->Encode4(0); // w
		oPacket->Encode4(0); // u
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_TrueSight)) {
		oPacket->Encode4(0); // n
		oPacket->Encode4(0); // r
		oPacket->Encode4(0); // t
		oPacket->Encode4(0); // c
		oPacket->Encode4(0); // p
		oPacket->Encode4(0); // u
		oPacket->Encode4(0); // w
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_MultiDamSkill)) {
		oPacket->Encode4(0);
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_Laser)) {
		oPacket->Encode4(0); // n
		oPacket->Encode4(0); // r
		oPacket->Encode4(0); // t
		oPacket->Encode4(0); // w
		oPacket->Encode4(0); // u
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_ElementResetBySummon)) {
		oPacket->Encode4(0); // c
		oPacket->Encode4(0); // p
		oPacket->Encode4(0); // u
		oPacket->Encode4(0); // w
	}
	if (CHECK_MS_NORML(flags, MobStatNumbers::MOB_STAT_BahamutLightElemAddDam)) {
		oPacket->Encode4(0); // p
		oPacket->Encode4(0); // c
	}
}