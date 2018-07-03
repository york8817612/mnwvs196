#include "User.h"
#include "ClientSocket.h"

#include "..\Database\GA_Character.hpp"
#include "..\Database\GW_CharacterStat.h"
#include "..\Database\GW_CharacterLevel.h"
#include "..\Database\GW_CharacterMoney.h"
#include "..\Database\GW_Avatar.hpp"

#include "..\Common\Net\OutPacket.h"
#include "..\Common\Net\InPacket.h"
#include "..\Common\Net\PacketFlags\ClientPacketFlags.hpp"
#include "..\Common\Net\PacketFlags\GamePacketFlags.hpp"
#include "..\Common\Net\PacketFlags\UserPacketFlags.h"
#include "..\Common\Net\PacketFlags\EPacketFlags.h"
#include "..\Common\Net\PacketFlags\WvsContextPacketFlags.hpp"

#include "FieldMan.h"
#include "Portal.h"
#include "PortalMap.h"
#include "WvsGame.h"
#include "Field.h"
#include "QWUInventory.h"
#include "QWUser.h"
#include "BasicStat.h"
#include "SecondaryStat.h"
#include "USkill.h"
#include "WvsGameConstants.hpp"
#include "AttackInfo.h"
#include "LifePool.h"
#include "SkillInfo.h"
#include "CommandManager.h"
#include "..\Common\Utility\DateTime\GameDateTime.h"

#include "ScriptMan.h"
#include "Script.h"

void User::TryParsingDamageData(AttackInfo * pInfo, InPacket * iPacket)
{
	int nDamageMobCount = pInfo->GetDamagedMobCount();
	int nDamagedCountPerMob = pInfo->GetDamageCountPerMob();
	for (int i = 0; i < nDamageMobCount; ++i)
	{
		int nObjectID = iPacket->Decode4();
		auto& ref = pInfo->m_mDmgInfo[nObjectID];
		iPacket->Decode1();
		iPacket->Decode1();
		iPacket->Decode1();
		iPacket->Decode1();
		iPacket->Decode1();
		iPacket->Decode4();
		iPacket->Decode1();
		iPacket->Decode2();
		iPacket->Decode2();
		iPacket->Decode2();
		iPacket->Decode2();

		if (pInfo->m_nType == EPacketFlags::CLIENT_PACKET::CP_UserMagicAttack)
		{
			iPacket->Decode1();
			if (pInfo->m_nSkillID == 80001835)
				iPacket->Decode1();
			else
				iPacket->Decode2();
		} 
		else
			iPacket->Decode2();

		for (int j = 0; j < nDamagedCountPerMob; ++j) 
		{
			long long int nDmg = iPacket->Decode8();
			printf("Monster %d Damage : %d\n", nObjectID, (int)nDmg);
			ref.push_back(nDmg);
		}
		iPacket->Decode4();
		iPacket->Decode2();
		iPacket->Decode8();
	}

	pInfo->m_nX = iPacket->Decode2();
	pInfo->m_nY = iPacket->Decode2();
}

AttackInfo * User::TryParsingMeleeAttack(int nType, InPacket * iPacket)
{
	AttackInfo* ret = new AttackInfo;
	ret->m_nType = nType;
	ret->m_bFieldKey = iPacket->Decode1();
	ret->m_bAttackInfoFlag = iPacket->Decode1();
	int nSkillID = ret->m_nSkillID = iPacket->Decode4();
	ret->m_nSLV = iPacket->Decode1();

	if (nType != EPacketFlags::CLIENT_PACKET::CP_UserAreaDotAttack)
		iPacket->Decode1();

	ret->m_dwCRC = iPacket->Decode4();

	iPacket->Decode1();
	iPacket->Decode2();
	iPacket->Decode4();

	if (WvsGameConstants::IsKeyDownSkill(nSkillID) || WvsGameConstants::IsSuperNovaSkill(nSkillID))
		ret->m_tKeyDown = iPacket->Decode4();

	if (WvsGameConstants::IsSpecialMeleeAttack(nSkillID))
		ret->m_pGrenade = iPacket->Decode4();

	if (WvsGameConstants::IsZeroSkill(nSkillID))
		iPacket->Decode1();

	if (nType != EPacketFlags::CLIENT_PACKET::CP_UserBodyAttack)
		iPacket->Decode1();

	iPacket->Decode1();

	ret->m_nDisplay = iPacket->Decode2();
	iPacket->Decode4();
	ret->m_nAttackActionType = iPacket->Decode1();
	ret->m_nAttackSpeed = iPacket->Decode1();

	if (nType != EPacketFlags::CLIENT_PACKET::CP_UserBodyAttack)
		ret->m_tLastAttackTime = iPacket->Decode4();

	iPacket->Decode4();
	ret->m_nFinalAttack = iPacket->Decode4();
	if (nSkillID > 0 && ret->m_nFinalAttack > 0)
		iPacket->Decode1();

	if (nSkillID == 5111009)
		iPacket->Decode1();

	if (WvsGameConstants::IsUsingBulletMeleeAttack(nSkillID))
	{
		iPacket->Decode2();
		if (nSkillID == 14000028 || nSkillID == 14000029 || nSkillID == 14121003 || nSkillID == 14121052)
			iPacket->Decode4();
	}

	if (nSkillID == 25111005)
		iPacket->Decode4();

	TryParsingDamageData(ret, iPacket);
	return ret;
}

AttackInfo * User::TryParsingMagicAttack(int nType, InPacket * iPacket)
{
	AttackInfo* ret = new AttackInfo;
	ret->m_nType = nType;
	ret->m_bFieldKey = iPacket->Decode1();
	ret->m_bAttackInfoFlag = iPacket->Decode1();
	int nSkillID = ret->m_nSkillID = iPacket->Decode4();
	ret->m_nSLV = iPacket->Decode1();

	ret->m_dwCRC = iPacket->Decode4();

	iPacket->Decode1();
	iPacket->Decode2();
	iPacket->Decode4();

	if (WvsGameConstants::IsKeyDownSkill(nSkillID))
		ret->m_tKeyDown = iPacket->Decode4();

	iPacket->Decode1();
	iPacket->Decode1();

	ret->m_nDisplay = iPacket->Decode2();
	iPacket->Decode4();
	ret->m_nAttackActionType = iPacket->Decode1();
	if (WvsGameConstants::IsEvanForceSkill(nSkillID))
		ret->m_bEvanForceAction = iPacket->Decode1();

	ret->m_nAttackSpeed = iPacket->Decode1();
	ret->m_tLastAttackTime = iPacket->Decode4();
	iPacket->Decode4();

	TryParsingDamageData(ret, iPacket);
	return ret;
}

AttackInfo * User::TryParsingShootAttack(int nType, InPacket * iPacket)
{
	AttackInfo* ret = new AttackInfo;
	iPacket->Decode1();
	ret->m_nType = nType;
	ret->m_bFieldKey = iPacket->Decode1();
	ret->m_bAttackInfoFlag = iPacket->Decode1();
	int nSkillID = ret->m_nSkillID = iPacket->Decode4();
	ret->m_nSLV = iPacket->Decode1();
	ret->m_bAddAttackProc = iPacket->Decode1();
	ret->m_dwCRC = iPacket->Decode4();

	iPacket->Decode1();
	ret->m_nSlot = iPacket->Decode2();
	ret->m_nCsStar = iPacket->Decode4();

	//int tKeyDown = 0, pGrenade;

	if (WvsGameConstants::IsKeyDownSkill(nSkillID))
		ret->m_tKeyDown = iPacket->Decode4();

	if (WvsGameConstants::IsZeroSkill(nSkillID))
		iPacket->Decode1();

	iPacket->Decode1();
	iPacket->Decode1();

	ret->m_apMinion = iPacket->Decode4();
	ret->m_bCheckExJablinResult = iPacket->Decode1();

	if (nSkillID == 3111013)
	{
		iPacket->Decode4();
		int nX = iPacket->Decode2();
		int nY = iPacket->Decode2();
	}

	ret->m_nDisplay = iPacket->Decode2();
	iPacket->Decode4();
	ret->m_nAttackActionType = iPacket->Decode1();

	if (nSkillID == 23111001 || nSkillID == 80001915 || nSkillID == 36111010)
	{
		iPacket->Decode4();
		iPacket->Decode4();
		iPacket->Decode4();
	}

	ret->m_nAttackSpeed = iPacket->Decode1();
	ret->m_tLastAttackTime = iPacket->Decode4();
	ret->m_nSoulArrow = iPacket->Decode4();
	ret->m_nWeaponType = iPacket->Decode2();
	ret->m_nShootRange = iPacket->Decode1();

	iPacket->Decode2(); //unk
	iPacket->Decode2(); //unk
	iPacket->Decode2(); //unk
	iPacket->Decode2(); //unk

	TryParsingDamageData(ret, iPacket);
	return ret;
}

AttackInfo * User::TryParsingAreaDot(int nType, InPacket * iPacket)
{
	return TryParsingMeleeAttack(nType, iPacket);
}

AttackInfo * User::TryParsingBodyAttack(int nType, InPacket * iPacket)
{
	return TryParsingMeleeAttack(nType, iPacket);
}

void User::OnIssueReloginCookie(InPacket * iPacket)
{
	MigrateOut();
}

User::User(ClientSocket *_pSocket, InPacket *iPacket)
	: pSocket(_pSocket),
	  pCharacterData(new GA_Character()),
	  m_pBasicStat(new BasicStat),
	  m_pSecondaryStat(new SecondaryStat)
{
	pCharacterData->DecodeCharacterData(iPacket);
	_pSocket->SetUser(this);
	m_pField = (FieldMan::GetInstance()->GetField(pCharacterData->nFieldID));
	m_pField->OnEnter(this);
	auto bindT = std::bind(&User::Update, this);
	auto pUpdateTimer = AsnycScheduler::CreateTask(bindT, 2000, true);
	m_pUpdateTimer = pUpdateTimer;
	pUpdateTimer->Start();
}

User::~User()
{
	OutPacket oPacket;
	oPacket.Encode2(GamePacketFlag::RequestMigrateOut);
	oPacket.Encode4(pSocket->GetSocketID());
	oPacket.Encode4(GetUserID());
	pCharacterData->EncodeCharacterData(&oPacket);
	WvsGame::GetInstance<WvsGame>()->GetCenter()->SendPacket(&oPacket);

	auto bindT = std::bind(&User::Update, this);
	((AsnycScheduler::AsnycScheduler<decltype(bindT)>*)m_pUpdateTimer)->Abort();
	//m_pField->OnLeave(this);
	LeaveField();
	
	try {
		if (GetScript())
			GetScript()->Abort();
	}
	catch (...) {}

	delete pCharacterData;
	delete m_pBasicStat;
	delete m_pSecondaryStat;
}

int User::GetUserID() const
{
	return pCharacterData->nCharacterID;
}

void User::SendPacket(OutPacket *oPacket)
{
	pSocket->SendPacket(oPacket);
}

GA_Character * User::GetCharacterData()
{
	return pCharacterData;
}

Field * User::GetField()
{
	return m_pField;
}

void User::MakeEnterFieldPacket(OutPacket *oPacket)
{
	oPacket->Encode2(EPacketFlags::SERVER_PACKET::LP_UserEnterField);
	oPacket->Encode4(GetUserID());
	oPacket->Encode1(pCharacterData->mLevel->nLevel);
	oPacket->EncodeStr(pCharacterData->strName);

	//MapleQuestStatus ultExplorer = chr.getQuestNoAdd(MapleQuest.getInstance(111111));
	/*if ((ultExplorer != null) && (ultExplorer.getCustomData() != null)) {
		oPacket->EncodeStr(ultExplorer.getCustomData());
	}
	else {*/
		oPacket->EncodeStr("");
	//}

	//if (chr.getGuildId() > 0) {
	//	MapleGuild gs = World.Guild.getGuild(chr.getGuildId());
	//	if (gs != null) {
	//		oPacket->EncodeStr(gs.getName());
	//		oPacket->Encode2(gs.getLogoBG()); // m_nGuildMarkBg
	//		oPacket->Encode1(gs.getLogoBGColor()); // m_nGuildMarkBgColor
	//		oPacket->Encode2(gs.getLogo()); // m_nGuildMark
	//		oPacket->Encode1(gs.getLogoColor()); // m_nGender
	//		oPacket->Encode1(0); // m_nAccountGender
	//		oPacket->Encode4(0); // m_nPopularity
	//	}
	//	else {
	//		mplew.writeZeroBytes(13);
	//	}
	//}
	//else {
		oPacket->EncodeBuffer(nullptr, 13);
	//}

	oPacket->Encode4(0); // m_nNameTagMark

	//PacketHelper.addSpawnPlayerBuffStat(mplew, chr);
	auto tsFlag = TemporaryStat::TS_Flag::GetDefault();
	m_pSecondaryStat->EncodeForRemote(oPacket, tsFlag);

	oPacket->Encode2(pCharacterData->mStat->nJob); // m_nJobCode
	oPacket->Encode2(pCharacterData->mStat->nSubJob); // m_nSubJobCode
	oPacket->Encode4(0);//[33 01 00 00] // m_nTotalCHUC
	pCharacterData->EncodeAvatarLook(oPacket);
	if (WvsGameConstants::IsZeroJob(pCharacterData->mStat->nJob)) {
		pCharacterData->EncodeAvatarLook(oPacket);
	}

	//PacketHelper.UnkFunctin6(mplew);
	{
		int v7 = 2;
		do {
			oPacket->Encode4(0);
			while (true) {
				int res = 255;
				oPacket->Encode1(res);
				if (res == 255) {
					break;
				}
				oPacket->Encode4(0);
			}
			v7 += 36;
		} while (v7 < 74);
	}

	oPacket->Encode4(0);
	oPacket->Encode4(0);

	//int buffSrc = chr.getBuffSource(MapleBuffStat.SECONDARY_STAT_RideVehicle);
	//if ((chr.getBuffedValue(MapleBuffStat.SECONDARY_STAT_Flying) != null) && (buffSrc > 0)) {
	//	addMountId(mplew, chr, buffSrc);
	//	oPacket->Encode4(chr.getId());
	//	oPacket->Encode4(0);
	//}
	//else {
		oPacket->Encode4(0);
		oPacket->Encode4(0);
		int size = 0;
		oPacket->Encode4(size);
		for (int i = 0; i < size; i++) {
			oPacket->Encode4(0);
			oPacket->Encode4(0);
		}
	//}

	oPacket->Encode4(0/*Math.min(250, chr.getInventory(MapleInventoryType.CASH).countById(5110000))*/); //頭上「紅心巧克力」個數
	oPacket->Encode4(0/*chr.getItemEffect()*/);
	oPacket->Encode4(0);
	oPacket->Encode4(0/*chr.getTitleEffect()*/);
	oPacket->Encode4(0/*chr.getDamageSkin()*/);
	oPacket->Encode4(0);
	oPacket->Encode4(0);
	oPacket->Encode4(0);
	oPacket->Encode4(0);
	oPacket->Encode4(0);
	oPacket->Encode2(-1);
	oPacket->EncodeStr("");
	oPacket->EncodeStr("");
	oPacket->Encode2(-1);
	oPacket->Encode2(-1);
	oPacket->Encode1(0);
	// m_nPortableChairID
	oPacket->Encode4(/*GameConstants.getInventoryType(chr.getChair()) == MapleInventoryType.SETUP ? chr.getChair() : */0);
	std::string text = "";
	oPacket->Encode4(text.size()); 
	if (text.size()) {
		oPacket->EncodeStr(text); // m_sPortableChairMsg
	}
	oPacket->Encode4(0);
	oPacket->Encode2(this->GetPosX());
	oPacket->Encode2(this->GetPosY());
	oPacket->Encode1(this->GetMoveAction());
	oPacket->Encode2(this->GetFh());

	/*for (int i = 0; i <= 3; i++) { */
		//MaplePet pet = chr.getSummonedPet(i);
		oPacket->Encode1(0/*pet != null*/);
		/*if (pet == null) {
			break;
		}
		oPacket->Encode4(i);
		PetPacket.addPetInfo(mplew, chr, pet, false);
	}*/
	oPacket->Encode1(0/*chr.getHaku() != null && MapleJob.is陰陽師(chr.getJob())*/);
	/*if (chr.getHaku() != null && MapleJob.is陰陽師(chr.getJob())) {
		MapleHaku haku = chr.getHaku();
		oPacket->Encode4(haku.getObjectId());
		oPacket->Encode4(40020109);
		oPacket->Encode1(1);
		mplew.writePos(haku.getPosition());
		oPacket->Encode1(0);
		oPacket->Encode2(haku.getStance());
		oPacket->Encode1(0);
	}*/
	oPacket->Encode4(/*chr.getMount() != null ? chr.getMount().getLevel() : */1); 
	oPacket->Encode4(/*chr.getMount() != null ? chr.getMount().getExp() : */0);
	oPacket->Encode4(/*chr.getMount() != null ? chr.getMount().getFatigue() : */0);

	//PacketHelper.addAnnounceBox(mplew, chr);
	{
		oPacket->Encode1(0);
	}
	// m_bADBoardRemote
	oPacket->Encode1(/*(chr.getChalkboard() != null) && (chr.getChalkboard().length() > 0) ? 1 : */0);
	/*if ((chr.getChalkboard() != null) && (chr.getChalkboard().length() > 0)) {
		oPacket->EncodeStr(chr.getChalkboard());
	}*/

	/*Triple<List<MapleRing>, List<MapleRing>, List<MapleRing>> rings = chr.getRings(false);
	List<MapleRing> allrings = rings.getLeft();
	allrings.addAll(rings.getMid());
	addRingInfo(mplew, allrings);*/
	{
	oPacket->Encode1(0);
	}
	//addRingInfo(mplew, allrings);
	{
		oPacket->Encode1(0);
	}
	//addMRingInfo(mplew, rings.getRight(), chr);
	{
		oPacket->Encode1(0);
	}
	int v65 = 0;
	oPacket->Encode1(v65);
	for (int o = 0; o < v65; o++) {
		oPacket->Encode4(0);
	}
	// m_nDelayedEffectFlag
	int unk_mask = 0;
	oPacket->Encode1(/*chr.getStat().Berserk ? 1 : */0); //unk_mask
	if ((unk_mask & 1) != 0) {
	}
	if ((unk_mask & 2) != 0) {
	}
	if ((unk_mask & 8) != 0) {
		oPacket->Encode4(0); // m_tDelayedPvPEffectTime
	}
	if ((unk_mask & 0x10) != 0) {
		oPacket->Encode4(0); // m_tDelayedPvPEffectTime
	}
	if ((unk_mask & 0x20) != 0) {
		oPacket->Encode4(0); // m_tHitPeriodRemain_Revive
	}
	// m_nEvanDragonGlide_Riding
	oPacket->Encode4(0/*chr.getMount().getItemId()*/);

	//if (MapleJob.is凱撒(chr.getJob())) {
	//	String x = chr.getOneInfo(12860, "extern");
	//	oPacket->Encode4(x == null ? 0 : Integer.parseInt(x)); // m_nKaiserMorphRotateHueExtern
	//	x = chr.getOneInfo(12860, "inner");
	//	oPacket->Encode4(x == null ? 0 : Integer.parseInt(x)); // m_nKaiserMorphRotateHueInnner
	//	x = chr.getOneInfo(12860, "premium");
	//	oPacket->Encode1(x == null ? 0 : Integer.parseInt(x)); // m_bKaiserMorphPrimiumBlack
	//}

	oPacket->Encode1(0);
	oPacket->Encode4(0); // CUser::SetMakingMeisterSkillEff

					   //PacketHelper.addFarmInfo(mplew, chr.getClient(), 0);

	for (int i = 0; i < 5; i++) {
		oPacket->Encode1(-1); // m_aActiveEventNameTag
	}

	// m_CustomizeEff.sEffectInfo
	int v84 = 0;
	oPacket->Encode4(v84);
	if (v84 > 0) {
		oPacket->EncodeStr("");
	}
	oPacket->Encode1(1); // m_bSoulEffect
	if (false) {
		int v87 = 0;
		oPacket->Encode4(v87);
		for (int i = 0; i < v87; i++) {
			oPacket->Encode4(0);
		}
	}
	// CUser::SetFlareBlink
	// m_pFlameWizardHelper.p
	bool v90 = false;
	oPacket->Encode1(v90);
	if (v90) {
		bool v91 = false;
		oPacket->Encode1(v91);
		if (v91) {
			oPacket->Encode4(0);
			oPacket->Encode4(0);
			oPacket->Encode2(0);
			oPacket->Encode2(0);
		}
	}

	oPacket->Encode1(0); // for CUser::StarPlanetRank::Decode
	oPacket->Encode4(0); // for CUser::DecodeStarPlanetTrendShopLook
	oPacket->Encode4(0); // for CUser::DecodeTextEquipInfo

	oPacket->Encode1(0); // CUser::DecodeFreezeHotEventInfo
	oPacket->Encode4(0); // CUser::DecodeEventBestFriendInfo
	oPacket->Encode4(0);

	oPacket->Encode1(0);

	oPacket->Encode4(0);

	oPacket->Encode4(0);
	oPacket->Encode4(0);

	oPacket->Encode4(0);
	oPacket->Encode4(0);

	oPacket->Encode4(0); // for
}

void User::MakeLeaveFieldPacket(OutPacket * oPacket)
{
}

void User::OnPacket(InPacket *iPacket)
{
	int nType = (unsigned short)iPacket->Decode2();
	switch (nType)
	{
	case EPacketFlags::CLIENT_PACKET::CP_UserRequestInstanceTable:
	{
		std::string strSkill = iPacket->DecodeStr();
		int nLVL1 = iPacket->Decode4();
		int nLVL2 = iPacket->Decode4();
		OutPacket oPacket;
		oPacket.Encode2(0xC1);
		oPacket.EncodeStr(strSkill);
		oPacket.Encode4(nLVL1);
		oPacket.Encode4(nLVL2);
		oPacket.Encode1(0);
		oPacket.Encode4(0);
		SendPacket(&oPacket);
		break;
	}
	case EPacketFlags::CLIENT_PACKET::CP_UserChat:
		OnChat(iPacket);
		break;
	case EPacketFlags::CLIENT_PACKET::CP_UserTransferFieldRequest:
		OnTransferFieldRequest(iPacket);
		break;
	case EPacketFlags::CLIENT_PACKET::CP_UserMove:
		m_pField->OnUserMove(this, iPacket);
		break;
	case EPacketFlags::CLIENT_PACKET::CP_UserChangeSlotPositionRequest:
		QWUInventory::OnChangeSlotPositionRequest(this, iPacket);
		break;
	case EPacketFlags::CLIENT_PACKET::CP_UserAbilityUpRequest:
		OnAbilityUpRequest(iPacket);
		break;
	case EPacketFlags::CLIENT_PACKET::CP_UserAbilityMassUpRequest:

		break;
	case EPacketFlags::CLIENT_PACKET::CP_UserSkillUpRequest:
		USkill::OnSkillUpRequest(this, iPacket);
		break;
	case EPacketFlags::CLIENT_PACKET::CP_UserSkillUseRequest:
		USkill::OnSkillUseRequest(this, iPacket);
		break;
	case EPacketFlags::CLIENT_PACKET::CP_UserSkillCancelRequest:
		USkill::OnSkillCancelRequest(this, iPacket);
		break;
	case EPacketFlags::CLIENT_PACKET::CP_UserCharacterInfoRequest:
		OnCharacterInfoRequest(iPacket);
		break;
	case EPacketFlags::CLIENT_PACKET::CP_UserMeleeAttack:
	case EPacketFlags::CLIENT_PACKET::CP_UserShootAttack:
	case EPacketFlags::CLIENT_PACKET::CP_UserMagicAttack:
	case EPacketFlags::CLIENT_PACKET::CP_UserBodyAttack:
	case EPacketFlags::CLIENT_PACKET::CP_UserAreaDotAttack:
		OnAttack(nType, iPacket);
		break;
	case EPacketFlags::CLIENT_PACKET::CP_UserActivateNickItem:
		OnUserActivateNickItem(iPacket);
		break;
	case EPacketFlags::CLIENT_PACKET::CP_UserDropMoneyRequest:

		break;
	case ClientPacketFlag::OnChangeCharacterRequest:
		OnIssueReloginCookie(iPacket);
		break;
	case ClientPacketFlag::OnSelectNpc:
		OnSelectNpc(iPacket);
		break;
	case ClientPacketFlag::OnScriptMessageAnswer:
		OnScriptMessageAnswer(iPacket);
		break;
	case ClientPacketFlag::OnQuestRequest:
		OnQuestRequest(iPacket);
		break;
	default:
		if (m_pField)
		{
			iPacket->RestorePacket();
			m_pField->OnPacket(this, iPacket);
		}
	}
	ValidateStat();
	SendCharacterStat(false, 0);
}

void User::OnTransferFieldRequest(InPacket * iPacket)
{
	if (!m_pField)
		pSocket->GetSocket().close();
	iPacket->Decode1(); //ms_RTTI_CField ?
	int dwFieldReturn = iPacket->Decode4();
	std::string sPortalName = iPacket->DecodeStr();
	if (sPortalName.size() > 0)
	{
		iPacket->Decode2(); //not sure
		iPacket->Decode2(); //not sure
	}
	std::lock_guard<std::mutex> user_lock(m_mtxUserlock);
	/*
	if(m_character.characterStat.nHP == 0)
	{
		m_basicStat->SetFrom(m_character, m_aRealEquip, m_aRealEqup2, 0, 0, 0);
		m_secondaryStat->Clear();
		....
	}
	*/
	Portal* pPortal = m_pField->GetPortalMap()->FindPortal(sPortalName);
	Field *pTargetField = FieldMan::GetInstance()->GetField(dwFieldReturn == -1 ? pPortal->GetTargetMap() : dwFieldReturn);
	if (pTargetField != nullptr)
	{
		Portal* pTargetPortal = pPortal == nullptr ? nullptr : pTargetField->GetPortalMap()->FindPortal(pPortal->GetTargetPortalName());
		LeaveField();
		m_pField = pTargetField;
		PostTransferField(m_pField->GetFieldID(), pTargetPortal, false);
		m_pField->OnEnter(this);
		pCharacterData->nFieldID = m_pField->GetFieldID();
	}
}

void User::OnChat(InPacket *iPacket)
{
	iPacket->Decode4(); //TIME TICK
	std::string strMsg = iPacket->DecodeStr();
	CommandManager::GetInstance()->Process(this, strMsg);

	unsigned char balloon = iPacket->Decode1();

	OutPacket oPacket;
	oPacket.Encode2(EPacketFlags::SERVER_PACKET::LP_UserChat);
	oPacket.Encode4(GetUserID());
	oPacket.Encode1(0);
	oPacket.EncodeStr(strMsg);
	oPacket.Encode1(balloon);
	oPacket.Encode1(0);
	oPacket.Encode1(-1);

	m_pField->SplitSendPacket(&oPacket, nullptr);
}

void User::PostTransferField(int dwFieldID, Portal * pPortal, int bForce)
{
	OutPacket oPacket;
	oPacket.Encode2(EPacketFlags::SERVER_PACKET::LP_SetField); //Set Stage
	oPacket.Encode4(0); //nChannel
	oPacket.Encode1(0);
	oPacket.Encode4(0);

	oPacket.Encode1(2); //bCharacterData?
	oPacket.Encode4(0);
	oPacket.Encode4(1024);
	oPacket.Encode4(768);

	oPacket.Encode1(0); // Change Stage(1) or Transfer Field(0)
	oPacket.Encode2(0);

	oPacket.Encode1(0); //bUsingBuffProtector
	oPacket.Encode4(dwFieldID);
	oPacket.Encode1(pPortal == nullptr ? 0x80 : pPortal->GetID());
	oPacket.Encode4(pCharacterData->mStat->nHP); //HP

	oPacket.Encode1(0);
	oPacket.Encode1(0);
	oPacket.Encode1(0);

	oPacket.Encode8(std::time(nullptr));
	oPacket.EncodeHexString("64 00 00 00 00 00 00 01 A6 00 00 00 03 00 00 00 83 7D 26 5A 02 00 00 24 66 00 00 00 00 00 03 00 00 00 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 40 E0 FD 3B 37 4F 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 82 16 FB 52 01 00 00 24 0C 00 00 00 00 00 00 00 00 00 00 00 C8 00 00 00 F7 24 11 76 00 00 00 24 0C 00 00 00 01 00 00 24 02 00 00 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 96 00 00 00 00");
	SendPacket(&oPacket);
}

void User::SetMovePosition(int x, int y, char bMoveAction, short nFSN)
{
	SetPosX(x);
	SetPosY(y);
	SetMoveAction(bMoveAction);
	SetFh(nFSN);
}

void User::OnAbilityUpRequest(InPacket *iPacket)
{
	int v4 = iPacket->Decode4();
	long long v6;
	unsigned int dwFlaga = iPacket->Decode4();

	/*if (!CCheatInspector::InspectExclRequestTime((v2 + 552), v2, v4, 500))
		CVerboseObj::LogError(v2 + 4, "Illegal ExclRequest : OnAbilityUpRequest");*/

	switch (dwFlaga)
	{
		case BasicStat::BS_STR:
			v6 = QWUser::IncSTR(this, 1, 1);
			break;
		case BasicStat::BS_DEX:
			v6 = QWUser::IncDEX(this, 1, 1);
			break;
		case BasicStat::BS_INT:
			v6 = QWUser::IncINT(this, 1, 1);
			break;
		case BasicStat::BS_LUK:
			v6 = QWUser::IncLUK(this, 1, 1);
			break;
		case BasicStat::BS_MaxHP:
		{
			int nJob = pCharacterData->mStat->nJob;
			int maxhp = 0;
			if (WvsGameConstants::IsBeginnerJob(nJob)) { // Beginner
				maxhp = 8 + rand() % (12 - 8 + 1);
			}
			else if (WvsGameConstants::IsDAvengerJob(nJob)) {
				maxhp = 30;
			}
			else if ((nJob >= 100 && nJob <= 132) || (nJob >= 3200 && nJob <= 3212) || (nJob >= 1100 && nJob <= 1112) || (nJob >= 3100 && nJob <= 3112)) { // Warrior
				maxhp = 36 + rand() % (42 - 36 + 1);
			}
			else if ((nJob >= 200 && nJob <= 232) || (WvsGameConstants::IsEvanJob(nJob)) || (nJob >= 2700 && nJob <= 2712)) { // Magician
				maxhp = 10 + rand() % (20 - 10 + 1);
			}
			else if ((nJob >= 300 && nJob <= 322) || (nJob >= 400 && nJob <= 434) || (nJob >= 1300 && nJob <= 1312) || (nJob >= 1400 && nJob <= 1412) || (nJob >= 3300 && nJob <= 3312) || (nJob >= 2300 && nJob <= 2312)) { // Bowman
				maxhp = 16 + rand() % (20 - 16 + 1);
			}
			else if ((nJob >= 510 && nJob <= 512) || (nJob >= 1510 && nJob <= 1512)) {
				maxhp = 28 + rand() % (32 - 28 + 1);
			}
			else if ((nJob >= 500 && nJob <= 532) || WvsGameConstants::IsJettJob(nJob) || (nJob >= 3500 && nJob <= 3512) || nJob == 1500) { // Pirate
				maxhp = 18 + rand() % (22 - 18 + 1);
			}
			else if (nJob >= 1200 && nJob <= 1212) { // Flame Wizard
				maxhp = 15 + rand() % (21 - 15 + 1);
			}
			else if (nJob >= 2000 && nJob <= 2112) { // Aran
				maxhp = 38 + rand() % (42 - 38 + 1);
			}
			else if (nJob >= 10100 && nJob <= 10112) {
				maxhp = 48 + rand() % (52 - 48 + 1);
			}
			else { // GameMaster
				maxhp = 18 + rand() % (26 - 18 + 1);
			}
			if (maxhp)
			{
				printf("Incorrect AP-Up stat Job(%d) : %d\n", nJob, (int)maxhp);
				ValidateStat();
				SendCharacterStat(false, 0);
				return;
			}
			v6 = QWUser::IncMHP(this, maxhp, 1);
			break;
		}
		case BasicStat::BS_MaxMP:
		{
			int nJob = pCharacterData->mStat->nJob;
			int maxmp = 0;
			if (WvsGameConstants::IsBeginnerJob(nJob))
			{
				maxmp = 6 + rand() % (8 - 6 + 1);
			}
			else if (WvsGameConstants::IsDslayerJobBorn(nJob)
				|| WvsGameConstants::IsZeroJob(nJob)
				|| WvsGameConstants::IsJettJob(nJob)
				|| WvsGameConstants::IsKinesisJob(nJob)
				|| (nJob / 100 == 65 || nJob == 6001)
				|| (nJob / 100 == 42 || nJob == 4002))
			{
				ValidateStat();
				SendCharacterStat(false, 0);
				return;
			}
			else if ((nJob >= 200 && nJob <= 232)
				|| (WvsGameConstants::IsEvanJob(nJob))
				|| (nJob >= 3200 && nJob <= 3212)
				|| (nJob >= 1200 && nJob <= 1212))
			{ // Magician
				maxmp = 38 + rand() % (40 - 38 + 1);
			}
			else if ((nJob >= 300 && nJob <= 322)
				|| (nJob >= 400 && nJob <= 434)
				|| (nJob >= 500 && nJob <= 532)
				|| (nJob >= 3200 && nJob <= 3212)
				|| (nJob >= 3500 && nJob <= 3512)
				|| (nJob >= 1300 && nJob <= 1312)
				|| (nJob >= 1400 && nJob <= 1412)
				|| (nJob >= 1500 && nJob <= 1512)
				|| (nJob >= 2300 && nJob <= 2312))
			{ // Bowman
				maxmp = 10 + rand() % (12 - 10 + 1);
			}
			else if ((nJob >= 100 && nJob <= 132)
				|| (nJob >= 1100 && nJob <= 1112)
				|| (nJob >= 2000 && nJob <= 2112))
			{ // Soul Master
				maxmp = 6 + rand() % (9 - 6 + 1);
			}
			else { // GameMaster
				maxmp = 6 + rand() % (12 - 6 + 1);
			}
			if (maxmp)
			{
				printf("Incorrect SP-Up stat Job(%d) : %d\n", nJob, (int)maxmp);
				ValidateStat();
				SendCharacterStat(false, 0);
				return;
			}
			v6 = QWUser::IncMMP(this, 1, 1);
			break;
		}
		default:
		{
			printf("Incorrect AP-Up stat Job(%d) : %d\n", pCharacterData->mStat->nJob, (int)dwFlaga);
			ValidateStat();
			SendCharacterStat(false, 0);
			return;
		}
	}
	dwFlaga |= BasicStat::BS_AP;
	QWUser::IncAP(this, -1, 1);
	User::ValidateStat();
	User::SendCharacterStat(true, dwFlaga);
}

void User::OnDropMoneyRequest(InPacket *iPacket)
{
	/*if (this->m_nGradeCode & 0x10)
	{
		SendCharacterStat(1, 0);
		return;
	}*/
	int tRequestTime = iPacket->Decode4();
	int nAmounta = iPacket->Decode4();

	if (nAmounta >= 10 && nAmounta <= 50000)
	{
		/*if (!CCheatInspector::InspectExclRequestTime(this->m_cheatInspector, this, tRequestTime, 200))
			CVerboseObj::LogError(&pUser->vfptr, "Illegal ExclRequest : OnDropMoneyRequest");*/
		if (this->pCharacterData->mLevel->nLevel <= 0xFu)
		{
			/*CUser::CheckTradeLimitTime(pUser);*/
			//int nTradeMoneyLimit = nAmounta + pUser->m_nTradeMoneyLimit;
			//if (nTradeMoneyLimit > 1000000)
			//{
			//	COutPacket::COutPacket(&oPacket, 54, 0);
			//	//v16 = 0;
			//	COutPacket::Encode1(&oPacket, 0);
			//	CUser::SendPacket(pUser, &oPacket);
			//	//v16 = -1;
			//	//ZArray<unsigned_char>::RemoveAll(&oPacket.m_aSendBuff);
			//	return;
			//}
			//pUser->m_nTradeMoneyLimit = nTradeMoneyLimit;
		}
		/*Field *pField = this->m_pField;
		if (pField)
		{
			if (CWvsPhysicalSpace2D::GetFootholdUnderneath(
				&pField->m_space2D,
				pUser->m_ptCurPos.x,
				pUser->m_ptCurPos.y - 10,
				&cy))
			{
				v8 = pField->m_dropPool.m_bDropEverlasting;
				pDrop = &pField->m_dropPool;
				if (v8)
				{
					CQWUInventory::MoveMoneyToTemp(pUser, nAmounta);
				}
				else
				{
					if (!CQWUser::IncMoney(pUser, -nAmounta, 1, 1))
						return;
					CUser::SendCharacterStat(pUser, 1, 0x40000u);
				}
				Reward::Reward(&reward);
				nCharacterID = pUser->m_dwCharacterID;
				reward.nMoney = nAmounta;
				ptCurPosX = pUser->m_ptCurPos.x;
				ptCurPosY = pUser->m_ptCurPos.y;
				v16 = 1;
				reward.nType = 0;
				reward.nPeriod = 0;
				DropPool mDropPool;
				DropPoolCreate(pDrop, &reward, nCharacterID, 0, 0, 0, ptCurPosX, ptCurPosY, ptCurPosX, cy, 0, 0, 0, 0);
				if (CUser::IsLogging(pUser))
					CUser::SendAdminLog(pUser, "Money\t(Type:Drop, Meso:%d)", nAmounta);
				if (pUser->m_character.characterStat.nMoney > 300000000)
					CUser::SendMesoLog(pUser, "Trade\t(Type:Throw, Target:, Meso:%d)", nAmounta);
				v16 = -1;
				Reward::_Reward(&reward);
			}
		}*/
	}
}

void User::OnCharacterInfoRequest(InPacket *iPacket)
{
	int tRequestTime = iPacket->Decode4();
	/*if (!CCheatInspector::InspectExclRequestTime(&v2->m_cheatInspector, v2, v3, 200))
		CVerboseObj::LogError(&v2->vfptr, "Illegal ExclRequest : OnCharacterInfoRequest");*/
	User::FindUser(iPacket->Decode4());

	OutPacket oPacket;
	oPacket.Encode2(EPacketFlags::LP_CharacterInfo);
	oPacket.Encode4(GetUserID());
	oPacket.Encode1(0); // Boolean
	oPacket.Encode1(pCharacterData->mLevel->nLevel);
	oPacket.Encode2(pCharacterData->mStat->nJob);
	oPacket.Encode2(pCharacterData->mStat->nSubJob);
	oPacket.Encode1(pCharacterData->mStat->nBattleRank);
	oPacket.Encode4(pCharacterData->mStat->nPOP);
	//MapleMarriage marriage = chr.getMarriage();
	oPacket.Encode1(/*marriage != null && marriage.getId() != */0);
	//if (marriage != null && marriage.getId() != 0) {
	//	oPacket.Encode4(marriage.getId()); //marriage id
	//	oPacket.Encode4(marriage.getHusbandId()); //husband char id
	//	oPacket.Encode4(marriage.getWifeId()); //wife char id
	//	oPacket.Encode2(3); //msg type
	//	oPacket.Encode4(chr.getMarriageItemId()); //ring id husband
	//	oPacket.Encode4(chr.getMarriageItemId()); //ring id wife
	//	oPacket.EncodeStr(marriage.getHusbandName(), 15); //husband name
	//	oPacket.EncodeStr(marriage.getWifeName(), 15); //wife name
	//}
	//List prof = chr.getProfessions();
	oPacket.Encode1(0/*prof.size()*/);
	/*for (Iterator ii = prof.iterator(); ii.hasNext();) {
		int i = ((Integer)ii.next());
		oPacket.Encode2(i);
	}
	if (chr.getGuildId() <= 0) {*/
	oPacket.EncodeStr("-");
	oPacket.EncodeStr("");
	/*}
	else {
		MapleGuild gs = World.Guild.getGuild(chr.getGuildId());
		if (gs != null) {
			mplew.writeMapleAsciiString(gs.getName());
			if (gs.getAllianceId() > 0) {
				MapleGuildAlliance allianceName = World.Alliance.getAlliance(gs.getAllianceId());
				if (allianceName != null) {
					mplew.writeMapleAsciiString(allianceName.getName());
				}
				else {
					mplew.writeMapleAsciiString("");
				}
			}
			else {
				mplew.writeMapleAsciiString("");
			}
		}
		else {
			mplew.writeMapleAsciiString("-");
			mplew.writeMapleAsciiString("");
		}
	}*/
	oPacket.Encode1(/*isSelf ? -1 : */0);
	oPacket.Encode1(0);
	oPacket.EncodeStr("");
	oPacket.Encode1(0);
	oPacket.Encode1(0);
	oPacket.Encode1(0);
	oPacket.Encode1(0);
	oPacket.Encode1(0);
	oPacket.Encode1(0);

	/*for (MaplePet pet : chr.getSummonedPets()) {
		oPacket.Encode1(1);
		oPacket.Encode4(chr.getPetIndex(pet));
		oPacket.Encode4(pet.getPetItemId());
		mplew.writeMapleAsciiString(pet.getName());
		oPacket.Encode1(pet.getLevel());
		oPacket.Encode2(pet.getCloseness());
		oPacket.Encode1(pet.getFullness());
		oPacket.Encode2(pet.getFlags());
		Item inv = chr.getInventory(MapleInventoryType.EQUIPPED).getItem((byte)(pet.getSummonedValue() == 2 ? -130 : pet.getSummonedValue() == 1 ? -114 : -138));
		oPacket.Encode4(inv == null ? 0 : inv.getItemId());
		oPacket.Encode4(-1);
	}*/
	oPacket.Encode1(0);

	/*int wishlistSize = chr.getWishlistSize();*/
	oPacket.Encode1(0/*wishlistSize*/);
	/*if (wishlistSize > 0) {
		int[] wishlist = chr.getWishlist();
		for (int x = 0; x < wishlistSize; x++) {
			oPacket.Encode4(wishlist[x]);
		}
	}*/

	//Item medal = chr.getInventory(MapleInventoryType.EQUIPPED).getItem((byte)-46);
	oPacket.Encode4(0/*medal == null ? 0 : medal.getItemId()*/);
	//List<Pair<Integer, Long>> medalQuests = chr.getCompletedMedals();
	oPacket.Encode2(0/*medalQuests.size()*/);
	/*for (Pair x : medalQuests) {
		oPacket.Encode4(((Integer)x.left));
		mplew.writeLong(((Long)x.right));
	}*/

	bool v3 = false;
	oPacket.Encode1(v3);
	//if (v3) {
	//	// function {
	//	oPacket.Encode4(0);
	//	oPacket.Encode4(0);
	//	oPacket.Encode1(0);
	//	mplew.writeMapleAsciiString("");
	//	// } end 
	//	// function {
	//	oPacket.Encode4(0);
	//	oPacket.Encode4(0);
	//	oPacket.Encode1(0);
	//	mplew.writeMapleAsciiString("");
	//	// } end 
	//	oPacket.Encode2(0);

	//	int v4 = 0;
	//	oPacket.Encode2(v4);
	//	for (int i = 0; i < v4; i++) {
	//		// function {
	//		oPacket.Encode4(0);
	//		oPacket.Encode4(0);
	//		oPacket.Encode1(0);
	//		mplew.writeMapleAsciiString("");
	//		// } end 
	//	}
	//}

	for (int i = 0; i < 6; i++/*MapleTrait.MapleTraitType t : MapleTrait.MapleTraitType.values()*/) {
		oPacket.Encode1(0/*chr.getTrait(t).getLevel()*/);
	}

	//List chairs = new ArrayList();
	/*for (Item i : chr.getInventory(MapleInventoryType.SETUP).newList()) {
		if ((i.getItemId() / 10000 == 301) && (!chairs.contains(i.getItemId()))) {
			chairs.add(i.getItemId());
		}
	}*/
	oPacket.Encode4(0/*chairs.size()*/);
	/*for (Iterator ii = chairs.iterator(); ii.hasNext();) {
		int i = ((Integer)ii.next());
		oPacket.Encode4(i);
	}*/

	/*List<Integer> medals = new ArrayList<>();
	for (Item i : chr.getInventory(MapleInventoryType.EQUIP).newList()) {
		if (i.getItemId() >= 1142000 && i.getItemId() < 1152000) {
			medals.add(i.getItemId());
		}
	}*/
	oPacket.Encode4(0/*medals.size()*/);
	/*for (int i : medals) {
		oPacket.Encode4(i);
	}*/
}

void User::OnAvatarModified()
{
	OutPacket oPacket;
	oPacket.Encode2(EPacketFlags::SERVER_PACKET::LP_UserAvatarModified);
	oPacket.Encode4(nCharacterID);
	int dwAvatarModFlag = 1;
	oPacket.Encode1(dwAvatarModFlag); //m_dwAvatarModFlag
	if (dwAvatarModFlag & 1)
		this->pCharacterData->EncodeAvatarLook(&oPacket);
	if (dwAvatarModFlag & 2)
		oPacket.Encode1(0); //secondayStat.nSpeed
	if (dwAvatarModFlag & 4)
		oPacket.Encode1(0); //nChoco

	EncodeCoupleInfo(&oPacket);
	EncodeFriendshipInfo(&oPacket);
	EncodeMarriageInfo(&oPacket);

	oPacket.Encode4(0);
	oPacket.Encode4(0);
	oPacket.Encode4(0);

	m_pField->BroadcastPacket(&oPacket);
}

void User::EncodeCoupleInfo(OutPacket * oPacket)
{
	oPacket->Encode1(0);
	for (int i = 0; i < 0; ++i)
	{
		oPacket->Encode4(1);
		oPacket->Encode8(0); //liSN
		oPacket->Encode8(0); //liPairSN
		oPacket->Encode4(0); //nItemID
	}
}

void User::EncodeFriendshipInfo(OutPacket * oPacket)
{
	oPacket->Encode1(0);
	for (int i = 0; i < 0; ++i)
	{
		oPacket->Encode4(1);
		oPacket->Encode8(0); //liSN
		oPacket->Encode8(0); //liPairSN
		oPacket->Encode4(0); //nItemID
	}
}

void User::EncodeMarriageInfo(OutPacket * oPacket)
{
	oPacket->Encode1(0);
	for (int i = 0; i < 0; ++i)
	{
		oPacket->Encode4(nCharacterID); //
		oPacket->Encode4(0); //nPairID
		oPacket->Encode4(0); //nItemID
	}
}

void User::ValidateStat()
{
}

void User::SendCharacterStat(bool bOnExclRequest, long long int liFlag)
{
	OutPacket oPacket;
	oPacket.Encode2(EPacketFlags::SERVER_PACKET::LP_StatChanged);
	oPacket.Encode1((char)bOnExclRequest);
	oPacket.Encode8(liFlag);
	if (liFlag & BasicStat::BasicStatFlag::BS_Skin)
		oPacket.Encode1(pCharacterData->mAvatarData->nSkin);
	if (liFlag & BasicStat::BasicStatFlag::BS_Face)
		oPacket.Encode4(pCharacterData->mAvatarData->nFace);
	if (liFlag & BasicStat::BasicStatFlag::BS_Hair)
		oPacket.Encode4(pCharacterData->mAvatarData->nHair);
	if (liFlag & BasicStat::BasicStatFlag::BS_Level)
		oPacket.Encode1(pCharacterData->mLevel->nLevel);
	if (liFlag & BasicStat::BasicStatFlag::BS_Job)
	{
		oPacket.Encode2(pCharacterData->mStat->nJob);
		oPacket.Encode2(pCharacterData->mStat->nSubJob);
	}

	if (liFlag & BasicStat::BasicStatFlag::BS_STR)
		oPacket.Encode2(pCharacterData->mStat->nStr);
	if (liFlag & BasicStat::BasicStatFlag::BS_DEX)
		oPacket.Encode2(pCharacterData->mStat->nDex);
	if (liFlag & BasicStat::BasicStatFlag::BS_INT)
		oPacket.Encode2(pCharacterData->mStat->nInt);
	if (liFlag & BasicStat::BasicStatFlag::BS_LUK)
		oPacket.Encode2(pCharacterData->mStat->nLuk);
	if (liFlag & BasicStat::BasicStatFlag::BS_HP)
		oPacket.Encode4(pCharacterData->mStat->nHP);
	if (liFlag & BasicStat::BasicStatFlag::BS_MaxHP)
		oPacket.Encode4(pCharacterData->mStat->nMaxHP);
	if (liFlag & BasicStat::BasicStatFlag::BS_MP)
		oPacket.Encode4(pCharacterData->mStat->nMP);
	if (liFlag & BasicStat::BasicStatFlag::BS_MaxMP)
		oPacket.Encode4(pCharacterData->mStat->nMaxMP);

	if (liFlag & BasicStat::BasicStatFlag::BS_AP)
		oPacket.Encode2(pCharacterData->mStat->nAP);

	//not done yet.
	if (liFlag & BasicStat::BasicStatFlag::BS_SP)
		pCharacterData->mStat->EncodeExtendSP(&oPacket);

	if (liFlag & BasicStat::BasicStatFlag::BS_EXP)
		oPacket.Encode8(pCharacterData->mStat->nExp);
	if (liFlag & BasicStat::BasicStatFlag::BS_POP)
		oPacket.Encode4(pCharacterData->mStat->nPOP);
	if (liFlag & BasicStat::BasicStatFlag::BS_Meso)
		oPacket.Encode8(pCharacterData->mMoney->nMoney);

	oPacket.Encode1(0);
	oPacket.Encode1(0);
	oPacket.Encode1(0);
	oPacket.Encode1(0);
	oPacket.Encode1(0);

	SendPacket(&oPacket);
}

void User::SendTemporaryStatReset(TemporaryStat::TS_Flag& flag)
{
	OutPacket oPacket;
	oPacket.Encode2(EPacketFlags::SERVER_PACKET::LP_TemporaryStatReset);
	flag.Encode(&oPacket);
	m_pSecondaryStat->EncodeIndieTempStat(&oPacket, flag);
	oPacket.Encode2(0);
	oPacket.Encode1(0);
	oPacket.Encode1(0);
	oPacket.Encode1(0);
	SendPacket(&oPacket);
}

void User::SendTemporaryStatSet(TemporaryStat::TS_Flag& flag, int tDelay)
{
	OutPacket oPacket;
	oPacket.Encode2(EPacketFlags::SERVER_PACKET::LP_TemporaryStatSet);
	m_pSecondaryStat->EncodeForLocal(&oPacket, flag);
	SendPacket(&oPacket);
}

void User::OnAttack(int nType, InPacket * iPacket)
{
	std::unique_ptr<AttackInfo> pInfo = nullptr;
	switch (nType)
	{
		case EPacketFlags::CLIENT_PACKET::CP_UserMeleeAttack:
			pInfo.reset(TryParsingMeleeAttack(nType, iPacket));
			break;
		case EPacketFlags::CLIENT_PACKET::CP_UserShootAttack:
			pInfo.reset(TryParsingShootAttack(nType, iPacket));
			break;
		case EPacketFlags::CLIENT_PACKET::CP_UserMagicAttack:
			pInfo.reset(TryParsingMagicAttack(nType, iPacket));
			break;
		case EPacketFlags::CLIENT_PACKET::CP_UserBodyAttack:
			pInfo.reset(TryParsingBodyAttack(nType, iPacket));
			break;
		case EPacketFlags::CLIENT_PACKET::CP_UserAreaDotAttack:
			pInfo.reset(TryParsingAreaDot(nType, iPacket));
			break;
	}
	if (pInfo) 
	{
		m_pField->GetLifePool()->OnUserAttack(
			this,
			SkillInfo::GetInstance()->GetSkillByID(pInfo->m_nSkillID),
			pInfo.get()
		);
	}
}

void User::OnLevelUp()
{
}

SecondaryStat * User::GetSecondaryStat()
{
	return this->m_pSecondaryStat;
}

BasicStat * User::GetBasicStat()
{
	return this->m_pBasicStat;
}

std::mutex & User::GetLock()
{
	return m_mtxUserlock;
}

void User::Update()
{
	USkill::ResetTemporaryByTime(this, GameDateTime::GetTime());
}

void User::ResetTemporaryStat(int tCur, int nReasonID)
{
	if (nReasonID == 0)
	{
		
	}
}

void User::OnUserActivateNickItem(InPacket * iPacket)
{
	int nItem = iPacket->Decode4();
	if (nItem != 0) {
		//pCharacterData->
	}

	OutPacket oPacket;
	oPacket.Encode2(EPacketFlags::SERVER_PACKET::LP_UserSetActiveNickItem);
	oPacket.Encode4(pCharacterData->nCharacterID);
	oPacket.Encode4(nItem);
	SendPacket(&oPacket);
}

void User::MigrateOut()
{
	pCharacterData->Save();
	LeaveField();
	OutPacket oPacket;
	oPacket.Encode2(EPacketFlags::SERVER_PACKET::LP_ReturnToTitle);
	SendPacket(&oPacket);

}

User * User::FindUser(int nUserID)
{
	return WvsGame::GetInstance<WvsGame>()->FindUser(nUserID);
}

void User::SendDropPickUpResultPacket(bool bPickedUp, bool bIsMoney, int nItemID, int nCount, bool bOnExcelRequest)
{
	OutPacket oPacket;
	oPacket.Encode2(EPacketFlags::SERVER_PACKET::LP_Message);
	oPacket.Encode1((char)Message::eDropPickUpMessage);
	if (bPickedUp)
	{
		oPacket.Encode1(bIsMoney == true ? 1 : 0);
		if (bIsMoney)
		{
			oPacket.Encode1(0);
			oPacket.Encode8(nCount);
			oPacket.Encode2(0);
		}
		else
		{
			oPacket.Encode4(nItemID);
			oPacket.Encode4(nCount);
		}
	}
	else
	{
		oPacket.Encode1((char)0xFF);
		oPacket.Encode4(0);
		oPacket.Encode4(0);
	}
	SendPacket(&oPacket);
}

void User::SendDropPickUpFailPacket(bool bOnExcelRequest)
{
	OutPacket oPacket;
	oPacket.Encode2(EPacketFlags::SERVER_PACKET::LP_Message);
	oPacket.Encode1((char)Message::eDropPickUpMessage);
	oPacket.Encode1((char)0xFE);
	oPacket.Encode4(0);
	oPacket.Encode4(0);
	SendPacket(&oPacket);
}

Script * User::GetScript()
{
	return m_pScript;
}

void User::SetScript(Script * pScript)
{
	m_pScript = pScript;
}

void User::OnSelectNpc(InPacket * iPacket)
{
	auto pNpc = m_pField->GetLifePool()->GetNpc(iPacket->Decode4());
	if (pNpc != nullptr && GetScript() == nullptr)
	{
		auto pScript = ScriptMan::GetInstance()->GetScript("test.lua", pNpc->GetTemplateID());
		pScript->SetUser(this);
		SetScript(pScript);
		
		std::thread* t = new std::thread(&Script::Run, pScript);
		pScript->SetThread(t);
		t->detach();
	}
}

void User::OnScriptMessageAnswer(InPacket * iPacket)
{
	if (GetScript() != nullptr)
		m_pScript->OnPacket(iPacket);
	if(GetScript() != nullptr)
		m_pScript->Notify();
}

void User::OnScriptRun()
{
	std::unique_lock<std::mutex> lock(m_scriptLock);
	m_cndVariable.wait(lock);

	delete m_pScript;
}

void User::OnScriptDone()
{
	m_cndVariable.notify_all();
}

void User::OnQuestRequest(InPacket * iPacket)
{
	char nAction = iPacket->Decode1();
	int nQuestID = iPacket->Decode4(), tTick, nItemID, nNpcID;
	switch (nAction)
	{
	case 0:
		tTick = iPacket->Decode4();
		nItemID = iPacket->Decode4();
		break;
	case 1:
		nNpcID = iPacket->Decode4();
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	}
}

void User::OnAcceptQuest(InPacket * iPacket, int nQuestID, int dwTemplateID, Npc * pNpc)
{
}

void User::OnCompleteQuest(InPacket * iPacket, int nQuestID, int dwTemplateID, Npc * pNpc, bool bIsAutoComplete)
{
}

void User::OnResignQuest(InPacket * iPacket, int nQuestID)
{
}

void User::OnLostQuestItem(InPacket * iPacket, int nQuestID)
{
}

void User::SendQuestRecordMessage(int nKey, int nState, const std::string & sStringRecord)
{
	OutPacket oPacket;
	oPacket.Encode2(EPacketFlags::SERVER_PACKET::LP_Message);
	oPacket.Encode1((char)Message::eQuestRecordMessage);
	oPacket.Encode4(nKey);
	oPacket.Encode1(nState);
	switch (nState)
	{
		case 0:
			oPacket.Encode1(0);
			break;
		case 1:
			oPacket.EncodeStr(sStringRecord);
			break;
		case 2:
			oPacket.Encode8(GameDateTime::GetTime());
			break;
	}
}

void User::LeaveField()
{
	m_pField->OnLeave(this);
}