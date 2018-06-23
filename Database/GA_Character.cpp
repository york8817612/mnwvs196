#include "GA_Character.hpp"
#include "WvsUnified.h"
#include "..\Common\Net\OutPacket.h"
#include "..\WvsGame\WvsGameConstants.hpp"
#include "GW_ItemSlotEquip.h"
#include "GW_ItemSlotBundle.h"
#include "GW_CharacterStat.h"
#include "GW_CharacterLevel.h"
#include "GW_CharacterMoney.h"
#include "GW_Avatar.hpp"
#include "GW_SkillRecord.h"
#include "GW_QuestRecord.h"

#include <algorithm>
#include <chrono>

GA_Character::GA_Character()
	: mAvatarData(new GW_Avatar()),
	  mStat(new GW_CharacterStat()),
	  mLevel(new GW_CharacterLevel()),
	  mMoney(new GW_CharacterMoney())
{
	mAtomicRemovedIndexCounter = 10000;
}

GA_Character::~GA_Character()
{
	printf("Destruct Data Called\n");
	delete mAvatarData;
	delete mStat;
	delete mLevel;
	delete mMoney;
	for (auto& slot : mItemSlot)
	{
		// TODO: is_block_type_valid(header->_block_use)
		/*for (auto& pItem : slot)
			delete pItem.second;*/
	}

	for (auto& skill : mSkillRecord)
		delete skill.second;
}

void GA_Character::Load(int nCharacterID)
{
	LoadAvatar(nCharacterID);
	mMoney->Load(nCharacterID);
	LoadItemSlot();
	LoadSkillRecord();
	LoadQuestRecord();
}

void GA_Character::LoadAvatar(int nCharacterID)
{
	this->nCharacterID = nCharacterID;
	mStat->Load(nCharacterID);
	mLevel->Load(nCharacterID);
	mAvatarData->Load(this);


	Poco::Data::Statement queryStatement(GET_DB_SESSION);
	queryStatement << "SELECT * FROM Characters Where CharacterID = " << nCharacterID;
	Poco::Data::RecordSet recordSet(queryStatement);
	queryStatement.execute();
	
	nAccountID = recordSet["AccountID"];
	nWorldID = recordSet["WorldID"];
	strName = recordSet["CharacterName"].toString();
	nGuildID = recordSet["GuildID"];
	nPartyID = recordSet["PartyID"];
	nFieldID = recordSet["FieldID"];
	nGender = recordSet["Gender"];
}

void GA_Character::EncodeAvatar(OutPacket *oPacket)
{
	EncodeStat(oPacket);
	EncodeAvatarLook(oPacket);
}

void GA_Character::EncodeStat(OutPacket *oPacket)
{
	oPacket->Encode4(nCharacterID);
	oPacket->Encode4(nCharacterID);
	oPacket->Encode4(nWorldID);
	oPacket->EncodeBuffer((unsigned char*)strName.c_str(), 15);
	oPacket->Encode1(nGender);
	oPacket->Encode1(0);
	oPacket->Encode1(mStat->nSkin);
	oPacket->Encode4(mStat->nFace);
	oPacket->Encode4(mStat->nHair);
	oPacket->Encode1(nMixBaseHairColor);
	oPacket->Encode1(nMixAddHairColor);
	oPacket->Encode1(nMixHairBaseProb);
	oPacket->Encode1(mLevel->nLevel);
	oPacket->Encode2(mStat->nJob);
	oPacket->Encode2(mStat->nStr);
	oPacket->Encode2(mStat->nDex);
	oPacket->Encode2(mStat->nInt);
	oPacket->Encode2(mStat->nLuk);
	oPacket->Encode4(mStat->nHP);
	oPacket->Encode4(mStat->nMaxHP);
	oPacket->Encode4(mStat->nMP);
	oPacket->Encode4(mStat->nMaxMP);
	oPacket->Encode2(mStat->nAP);

	mStat->EncodeExtendSP(oPacket);

	oPacket->Encode8(mStat->nExp);
	oPacket->Encode4(mStat->nPOP);
	oPacket->Encode4(nWP);
	oPacket->Encode8(mStat->nGachaponExp); //Gach EXP
	oPacket->Encode4(CURRENT_SYSTEM_TIME.dwHighDateTime);
	oPacket->Encode4(CURRENT_SYSTEM_TIME.dwLowDateTime);
	oPacket->Encode4(nFieldID);
	oPacket->Encode1(nPortal); //Inital Spawn Point
	oPacket->Encode2(mStat->nSubJob); //Get Subcategory

	if (WvsGameConstants::IsDslayerJobBorn(mStat->nJob)
		|| WvsGameConstants::IsResHybridJob(mStat->nJob)
		|| WvsGameConstants::IsBeastTamerJob(mStat->nJob))
	{
		oPacket->Encode4(nDefFaceAcc);
	}

	oPacket->Encode1(nFatigue);
	oPacket->Encode4(0); // nLastFatigueUpdateTime

	oPacket->Encode4(mStat->nCharismaEXP);
	oPacket->Encode4(mStat->nInsightEXP);
	oPacket->Encode4(mStat->nWillEXP);
	oPacket->Encode4(mStat->nCraftEXP);
	oPacket->Encode4(mStat->nSenseEXP);
	oPacket->Encode4(mStat->nCharmEXP);

	for (int i = 0; i < 6; ++i)
		oPacket->Encode2(0);

	oPacket->Encode1(0);
	oPacket->Encode4(CURRENT_SYSTEM_TIME.dwHighDateTime);
	oPacket->Encode4(CURRENT_SYSTEM_TIME.dwLowDateTime);

	oPacket->Encode4(0); // nPvPExp
	oPacket->Encode1(0); // nPvPGrade
	oPacket->Encode4(0); // nPvPPoint
	oPacket->Encode1(0x05); // nPvPModeLevel
	oPacket->Encode1(0x06); // nPvPModeType
	oPacket->Encode4(0); // nEventPoint
	oPacket->Encode4(0);

	// TODO: Part Time Job.
	oPacket->Encode1(0); // nAlbaActivityID
	oPacket->Encode4(DB_DATE_20790101_23.dwLowDateTime);
	oPacket->Encode4(DB_DATE_20790101_23.dwHighDateTime);
	oPacket->Encode4(0); // nAlbaDuration
	oPacket->Encode1(0); // bAlbaSpecialReward

	// TODO: CHARACTERCARD::Decode
	for (int i = 0; i < 9; ++i)
	{
		oPacket->Encode4(0);
		oPacket->Encode1(0);
		oPacket->Encode4(0);
	}

	oPacket->Encode4(0/*ftLastLogout.dwHighDateTime*/);
	oPacket->Encode4(0/*ftLastLogout.dwLowDateTime*/);

	// Unk Function
	oPacket->Encode8(0);
	oPacket->Encode8(0);
	oPacket->Encode4(0);
	oPacket->Encode4(0);
	oPacket->Encode4(0);
	oPacket->Encode1(1); // bBurning??

	for (int i = 0; i < 25; ++i)
		oPacket->Encode1(0);

	oPacket->Encode1(0);
	oPacket->Encode1(0);
	oPacket->Encode1(0);
	oPacket->Encode1(0);
	oPacket->Encode1(0);
}

void GA_Character::EncodeSkillRecord(OutPacket * oPacket)
{
	oPacket->Encode1(1);
	oPacket->Encode2((short)mSkillRecord.size());
	for (auto& skill : mSkillRecord)
		skill.second->Encode(oPacket);
	oPacket->Encode2(0);
	oPacket->Encode4(0);
}

void GA_Character::EncodeAvatarLook(OutPacket *oPacket)
{
	mAvatarData->Encode(oPacket);
}

void GA_Character::Save(bool isNewCharacter)
{
	if (isNewCharacter)
	{
		nCharacterID = (int)IncCharacterID();
		Poco::Data::Statement newRecordStatement(GET_DB_SESSION);
		newRecordStatement << "INSERT INTO Characters(CharacterID, AccountID) VALUES(" << nCharacterID << ", " << nAccountID << ")";
		newRecordStatement.execute();
		newRecordStatement.reset(GET_DB_SESSION);
		//newRecordStatement << "SELECT CharacterID FROM Characters"
	}
	Poco::Data::Statement queryStatement(GET_DB_SESSION);
	queryStatement << "UPDATE Characters Set "
		<< "WorldID = '" << nWorldID << "', "
		<< "Gender = '" << nGender << "', "
		<< "CharacterName = '" << strName << "', "
		<< "GuildID = '" << nGuildID << "', "
		<< "PartyID = '" << nPartyID << "', "
		<< "FieldID = '" << nFieldID << "' WHERE CharacterID = " << nCharacterID;
	queryStatement.execute();
	mAvatarData->Save(this, isNewCharacter);
	mMoney->Save(nCharacterID, isNewCharacter);
	mLevel->Save(nCharacterID, isNewCharacter);
	mStat->Save(nCharacterID, isNewCharacter);

	for (auto& eqp : mItemSlot[1])
		((GW_ItemSlotEquip*)(eqp.second))->Save(nCharacterID, GW_ItemSlotBase::GW_ItemSlotType::EQUIP);
	for (auto& con : mItemSlot[2])
		((GW_ItemSlotBundle*)(con.second))->Save(nCharacterID, GW_ItemSlotBase::GW_ItemSlotType::CONSUME);
	for (auto& etc : mItemSlot[3])
		((GW_ItemSlotBundle*)(etc.second))->Save(nCharacterID, GW_ItemSlotBase::GW_ItemSlotType::ETC);
	for (auto& ins : mItemSlot[4])
		((GW_ItemSlotBundle*)(ins.second))->Save(nCharacterID, GW_ItemSlotBase::GW_ItemSlotType::INSTALL);

	for (auto& skill : mSkillRecord)
		skill.second->Save();
}

int GA_Character::FindEmptySlotPosition(int nTI)
{
	if (nTI <= 0 || nTI > 5)
		return 0;
	int lastIndex = 1;
	std::lock_guard<std::mutex> dataLock(mCharacterLock);
	auto itemSlot = mItemSlot[nTI];
	for (auto& slot : itemSlot)
	{
		if (slot.first < 0) //skip equipped
			continue;
		if (slot.first >= 10000)
			return 0;
		if (slot.first > lastIndex || (slot.first == lastIndex && slot.second == nullptr))
			return lastIndex;
		lastIndex = slot.first + 1;
	}
	return lastIndex;
}

GW_ItemSlotBase* GA_Character::GetItem(int nTI, int nPOS)
{
	if (nTI <= 0 || nTI > 5)
		return nullptr;
	auto result = mItemSlot[nTI].find(nPOS);
	if (result == mItemSlot[nTI].end())
		return nullptr;
	return result->second;
}

void GA_Character::RemoveItem(int nTI, int nPOS)
{
	if (nTI <= 0 || nTI > 5)
		return;
	std::lock_guard<std::mutex> dataLock(mCharacterLock);
	int nNewPos = (int)mAtomicRemovedIndexCounter++;
	auto pItem = GetItem(nTI, nPOS);
	if (pItem != nullptr)
	{
		mItemSlot[nTI].erase(pItem->nPOS);
		pItem->nPOS = nNewPos;
		pItem->liItemSN *= -1;
		mItemSlot[nTI][nNewPos] = pItem;
	}
}

int GA_Character::FindCashItemSlotPosition(int nTI, long long int liSN)
{
	if (nTI <= 0 || nTI > 5)
		return 0;
	std::lock_guard<std::mutex> dataLock(mCharacterLock);
	auto& itemSlot = mItemSlot[nTI];
	for (auto& slot : itemSlot)
		if (slot.second->liCashItemSN == liSN)
			return slot.second->nPOS;
}

int GA_Character::FindGeneralItemSlotPosition(int nTI, int nItemID, long long int dateExpire, long long int liSN)
{
	if (nTI <= 0 || nTI > 5)
		return 0;
	std::lock_guard<std::mutex> dataLock(mCharacterLock);
	auto& itemSlot = mItemSlot[nTI];
	for (auto& slot : itemSlot)
		if (slot.second->nItemID == nItemID && !CompareFileTime((FILETIME*)&dateExpire, (FILETIME*)(slot.second->liExpireDate)))
			return slot.second->nPOS;
	return 0;
}

int GA_Character::GetEmptySlotCount(int nTI)
{
	int nCount = 0;
	if (nTI <= 0 || nTI > 5)
		return 0;
	std::lock_guard<std::mutex> dataLock(mCharacterLock);
	auto& itemSlot = mItemSlot[nTI];
	int nLastIndeex = 0;
	for (auto& slot : itemSlot)
	{
		if (slot.second == nullptr)
			++nCount;
		nCount += (slot.first - nLastIndeex - 1);
		nLastIndeex = slot.first;
	}
	return nCount;
}

int GA_Character::GetItemCount(int nTI, int nItemID)
{
	int nCount = 0;
	if (nTI <= 0 || nTI > 5)
		return 0;
	std::lock_guard<std::mutex> dataLock(mCharacterLock);
	auto& itemSlot = mItemSlot[nTI];
	for (auto& slot : itemSlot)
		if (slot.second != nullptr && slot.second->nItemID == nItemID)
			++nCount;
	return nCount;
}

void GA_Character::SetItem(int nTI, int nPOS, GW_ItemSlotBase * pItem)
{
	std::lock_guard<std::mutex> dataLock(mCharacterLock);
	if (nTI >= 1 && nTI <= 5)
		mItemSlot[nTI][nPOS] = pItem;
}

decltype(GA_Character::mSkillRecord)& GA_Character::GetCharacterSkillRecord()
{
	return mSkillRecord;
}

GW_SkillRecord * GA_Character::GetSkill(int nSkillID)
{
	auto findResult = mSkillRecord.find(nSkillID);
	return findResult == mSkillRecord.end() ? nullptr : findResult->second;
}

void GA_Character::ObtainSkillRecord(GW_SkillRecord * pRecord)
{
	std::lock_guard<std::mutex> dataLock(mCharacterLock);
	if (pRecord != nullptr)
		mSkillRecord.insert({ pRecord->nSkillID, pRecord });
}

void GA_Character::SetQuest(int nKey, const std::string & sInfo)
{
	auto findIter = mQuestRecord.find(nKey);
	if (findIter == mQuestRecord.end())
	{
		GW_QuestRecord *pNewRecord = new GW_QuestRecord;
		pNewRecord->nState = 1;
		pNewRecord->nQuestID = nKey;
		pNewRecord->sStringRecord = sInfo;
	}
	else
		findIter->second->sStringRecord = sInfo;
}

void GA_Character::RemoveQuest(int nKey)
{
	mQuestRecord.erase(nKey);
}

std::mutex & GA_Character::GetCharacterDatLock()
{
	return mCharacterLock;
}

void GA_Character::DecodeCharacterData(InPacket *iPacket)
{
	long long int flag = iPacket->Decode8();
	iPacket->Decode1();
	for (int i = 0; i < 3; ++i)
		iPacket->Decode4();

	iPacket->Decode1();
	iPacket->Decode4();
	iPacket->Decode1();

	if (flag & 1)
	{
		DecodeStat(iPacket);
		int nBuddyCapicity = iPacket->Decode1(); //BUDDY LIST CAPACITY

		bool hasBlesOfFairyOrigin = iPacket->Decode1() == 1; //BLESS OF FAIRY ORIGIN
		bool hasBlesOfEmpressOrigin = iPacket->Decode1() == 1; //BLESS OF EMPRESS ORIGIN
		bool hasUltraExplorer = iPacket->Decode1() == 1; //ULTRA EXPLORER

		iPacket->Decode8(); //TIME CURRENT TIME
		iPacket->Decode8(); //TIME -2
		iPacket->Decode4();
		iPacket->Decode1();
		iPacket->Decode4();
		iPacket->Decode1();
	}

	if (flag & 2)
	{
		mMoney->nMoney = iPacket->Decode8();
		nCharacterID = iPacket->Decode4();
		iPacket->Decode4();
		iPacket->Decode4();
	}

	if (flag & 8)
		iPacket->Decode4();

	if (flag & 0x80)
	{
		iPacket->Decode4();
		iPacket->Decode4();
		for (int i = 0; i < 3; ++i)
		{
			iPacket->Decode4();
			iPacket->Decode4();
		}
		iPacket->Decode4();
		iPacket->Decode1();
		iPacket->Decode1();
		iPacket->Decode1();
	}

	int nEquipSlotCount = iPacket->Decode1(); //EQP SLOT
	int nConsumeSlotCount = iPacket->Decode1(); //CON SLOT
	int nInstallSlotCount = iPacket->Decode1(); //INS SLOT
	int nETCSlotCount = iPacket->Decode1(); //ETC SLOT
	int nCashSlotCount = iPacket->Decode1(); //CASH SLOT

	DecodeInventoryData(iPacket);

	if (flag & 0x100)
	{
		DecodeSkillRecord(iPacket);
	}

	if (flag & 0x8000)
	{
		int nCoolDownSize = iPacket->Decode2();
		for (int i = 0; i < nCoolDownSize; ++i)
		{

		}
	}

	if (flag & 0x200)
	{
		iPacket->Decode1();
		int nStartedQuestSize = iPacket->Decode2();
		for (int i = 0; i < nStartedQuestSize; ++i)
		{
			GW_QuestRecord *pRecord = new GW_QuestRecord;
			pRecord->nCharacterID = nCharacterID;
			pRecord->nState = 1;
			pRecord->Decode(iPacket, 1);
			mQuestRecord.insert({ pRecord->nQuestID, pRecord });
		}
		iPacket->Decode2();
	}

	if (flag & 0x4000)
	{
		iPacket->Decode1();
		int nCompletedQuestSize = iPacket->Decode2();
		for (int i = 0; i < nCompletedQuestSize; ++i)
		{
			GW_QuestRecord *pRecord = new GW_QuestRecord;
			pRecord->nCharacterID = nCharacterID;
			pRecord->nState = 2;
			pRecord->Decode(iPacket, 2);
			mQuestComplete.insert({ pRecord->nQuestID, pRecord });
		}
	}

	if (flag & 0x400)
	{
		iPacket->Decode2();
	}

	if (flag & 0x800)
	{
		int nCoupleRingSize = iPacket->Decode2();
		int nFriendRingSize = iPacket->Decode2();
		int nMarriageRingSize = iPacket->Decode2();
	}

	if (flag & 0x1000) //ROCK INFO
	{
		for (int i = 0; i < 5; ++i)
			iPacket->Decode4();
		for (int i = 0; i < 10; ++i)
			iPacket->Decode4();
		for (int i = 0; i < 13; ++i)
			iPacket->Decode4();
	}

	if (flag & 0x4000) //QUEST INFO
	{
		int nQuestInfoSize = iPacket->Decode2();
	}

	if (flag & 0x20)
	{
		iPacket->Decode2(); //UNK
	}

	iPacket->Decode1();

	if (flag & 0x40000000)
	{
		iPacket->Decode4(); //UNK
	}

	if (flag & 0x1000)
	{
		iPacket->Decode4(); //UNK
	}

	if (flag & 0x200000)
	{
		//JAGUAR INFO
	}

	//flag & 0x800 && is Zero

	if (flag & 0x4000000)
	{
		iPacket->Decode2(); //UNK
	}

	if (flag & 0x20000000)
	{
		for (int i = 0; i < 15; ++i)
			iPacket->Decode4();
	}

	if (flag & 0x10000000)
	{
		for (int i = 0; i < 5; ++i)
			iPacket->Decode4();
	}

	if (flag & 0x80000000)
	{
		int nAbilityInfoSize = iPacket->Decode2(); //ABILITY INFO
	}

	if (flag & 0x10000)
	{
		iPacket->Decode2(); //UNK
	}

	if (flag & 0x01)
	{
		int nHonorLVL = iPacket->Decode4(); //HONOR LEVEL
		int nHonorEXP = iPacket->Decode4(); //HONOR EXP
		iPacket->Decode4();
		iPacket->Decode4();
		iPacket->Decode4();
	}

	if (flag & 0x2000)
	{
		for (int i = 0; i < 5; ++i)
			iPacket->Decode4();
		for (int i = 0; i < 6; ++i)
			iPacket->Decode4();
		for (int i = 0; i < 4; ++i)
			iPacket->Decode4();
		iPacket->Decode8(); //TIME
		iPacket->Decode1();
		iPacket->Decode1();
	}

	if (flag & 0x4000)
	{
		iPacket->Decode2(); //UNK
	}

	if (flag & 0x8000) //UNK
	{
		iPacket->Decode2();
	}

	if (flag & 0x10000) //UNK
	{
		for (int i = 0; i < 14; ++i) //56
			iPacket->Decode4();
	}

	if (flag & 0x20000) //UNK
	{
		iPacket->Decode2();
	}

	if (flag & 0x2) //UNK
	{
		iPacket->Decode1();
		iPacket->Decode2();
	}

	if (flag & 0x4) //UNK
	{
		iPacket->Decode1();
	}

	//ANGELIC BUSTER
	iPacket->Decode4();
	iPacket->Decode4();
	iPacket->Decode4();

	iPacket->Decode1();
	iPacket->Decode4();
	iPacket->Decode4();
	iPacket->Decode4();

	if (flag & 0x200000) //UNK
	{
		iPacket->Decode8();
		iPacket->Decode8();
	}

	if (flag & 0x10)
	{
		iPacket->Decode2();
		iPacket->Decode2();
	}

	iPacket->Decode1();
	if (flag & 0x400)
	{
		for (int i = 0; i < 4; ++i)
			iPacket->Decode4();
	}

	for (int i = 0; i < 7; ++i) //28
		iPacket->Decode4();

	if (flag & 0x8000000)
	{
		iPacket->Decode4();
		iPacket->Decode4();
		iPacket->Decode8();
	}

	iPacket->Decode2();
	iPacket->Decode2();
	iPacket->Decode4();

	if (flag & 0x10000000) //UNK
	{
		iPacket->Decode2();
	}

	if (flag & 0x20000000) //V MATRIX
	{
		int nVMatrixRecord = iPacket->Decode4();
	}

	if (flag & 0x40000) //UNK
	{
		iPacket->Decode1();
		iPacket->Decode2();
		iPacket->Decode2();
	}

	if (flag & 0x80000)
	{
		iPacket->Decode1();
	}

	if (flag & 0x100000)
	{
		iPacket->Decode4();
		iPacket->Decode4();
	}
}

void GA_Character::DecodeStat(InPacket *iPacket)
{
	nCharacterID = iPacket->Decode4();
	nCharacterID = iPacket->Decode4();
	nWorldID = iPacket->Decode4();

	char cStrNameBuff[16]{ 0 };
	iPacket->DecodeBuffer((unsigned char*)cStrNameBuff, 15);
	strName = std::string(cStrNameBuff);

	nGender = iPacket->Decode1();
	iPacket->Decode1();
	mStat->nSkin = iPacket->Decode1();
	mStat->nFace = iPacket->Decode4();
	mStat->nHair = iPacket->Decode4();
	iPacket->Decode1();
	iPacket->Decode1();
	iPacket->Decode1();
	mLevel->nLevel = iPacket->Decode1(); //LEVEL
	mStat->nJob = iPacket->Decode2();

	mStat->nStr = iPacket->Decode2();
	mStat->nDex = iPacket->Decode2();
	mStat->nInt = iPacket->Decode2();
	mStat->nLuk = iPacket->Decode2();
	mStat->nHP = iPacket->Decode4();
	mStat->nMaxHP = iPacket->Decode4();
	mStat->nMP = iPacket->Decode4();
	mStat->nMaxMP = iPacket->Decode4();

	mStat->nAP = iPacket->Decode2();
	mStat->DecodeExtendSP(iPacket);
	//iPacket->Decode1(); //SP

	mStat->nExp = iPacket->Decode8(); //EXP
	mStat->nPOP = iPacket->Decode4();
	iPacket->Decode4();
	iPacket->Decode8(); //Gach EXP
	iPacket->Decode8(); //
	nFieldID = iPacket->Decode4();
	iPacket->Decode1(); //Inital Spawn Point
	iPacket->Decode2(); //Get Subcategory

						 //isDemonSlayer || isXenon || isDemonAvenger Decode4

	iPacket->Decode1(); //Fatigue
	iPacket->Decode4();

	for (int i = 0; i < 6; ++i)
		iPacket->Decode4();
	for (int i = 0; i < 6; ++i)
		iPacket->Decode2();
	iPacket->Decode1();
	iPacket->Decode8();

	iPacket->Decode4(); //PVP EXP
	iPacket->Decode1(); //PVP RANK
	iPacket->Decode4(); //BATTLE POINTS
	iPacket->Decode1();
	iPacket->Decode1();
	iPacket->Decode4();

	iPacket->Decode1();
	iPacket->Decode4();
	iPacket->Decode8();
	iPacket->Decode4();
	iPacket->Decode1();

	for (int i = 0; i < 9; ++i)
	{
		iPacket->Decode4();
		iPacket->Decode1();
		iPacket->Decode4();
	}

	iPacket->Decode8();
	iPacket->Decode8();
	iPacket->Decode8();
	iPacket->Decode4();
	iPacket->Decode4();
	iPacket->Decode4();
	iPacket->Decode1();

	for (int i = 0; i < 25; ++i)
		iPacket->Decode1();

	iPacket->Decode1();
	iPacket->Decode1();
	iPacket->Decode1();
	iPacket->Decode1();
	iPacket->Decode1();
}

void GA_Character::DecodeInventoryData(InPacket *iPacket)
{
	iPacket->Decode8(); // TIME
	iPacket->Decode1();

	unsigned short wPos = 0; //w = WORD

	while ((wPos = iPacket->Decode2()) != 0)
	{
		GW_ItemSlotEquip* eqp = new GW_ItemSlotEquip;
		eqp->nPOS = wPos * -1;
		eqp->nType = GW_ItemSlotBase::GW_ItemSlotType::EQUIP;
		eqp->Decode(iPacket);
		mItemSlot[1].insert({ eqp->nPOS , eqp });
	}

	while ((wPos = iPacket->Decode2()) != 0)
	{
		GW_ItemSlotEquip* eqp = new GW_ItemSlotEquip;
		eqp->nPOS = (wPos + 100) * -1;
		eqp->nType = GW_ItemSlotBase::GW_ItemSlotType::EQUIP;
		eqp->Decode(iPacket);
		mItemSlot[1].insert({ eqp->nPOS, eqp });
	}


	while ((wPos = iPacket->Decode2()) != 0)
	{
		GW_ItemSlotEquip* eqp = new GW_ItemSlotEquip;
		eqp->nPOS = wPos;
		eqp->nType = GW_ItemSlotBase::GW_ItemSlotType::EQUIP;
		eqp->Decode(iPacket);
		mItemSlot[1].insert({ eqp->nPOS, eqp });
	}
	//sub_69B50A
	iPacket->Decode2(); //sub_69B50A 1
	iPacket->Decode2(); //sub_69B50A 2
	iPacket->Decode2(); //sub_69B50A 3
	iPacket->Decode2(); //sub_69B50A 4
	iPacket->Decode2(); //sub_69B50A 5
	iPacket->Decode2(); //sub_69B50A 6
	iPacket->Decode2(); //sub_69B50A 7
	iPacket->Decode2(); //sub_69B50A 8
	iPacket->Decode2(); //sub_69B50A 9
	iPacket->Decode2(); //sub_69B50A 10
	iPacket->Decode2(); //sub_69B50A 11

						 //sub_68CB08
	iPacket->Decode2(); //sub_68CB08 1
	iPacket->Decode2(); //sub_68CB08 2

						 //sub_68CF44
	iPacket->Decode2(); //sub_68CF44 1
	iPacket->Decode2(); //sub_68CF44 2

	unsigned char nPos = 0;
	while ((nPos = iPacket->Decode1()) != 0)
	{
		GW_ItemSlotBundle* bundle = new GW_ItemSlotBundle();
		bundle->nPOS = nPos;
		bundle->nType = GW_ItemSlotBase::GW_ItemSlotType::CONSUME;
		bundle->Decode(iPacket);
		mItemSlot[2].insert({ bundle->nPOS, bundle });
	}

	while ((nPos = iPacket->Decode1()) != 0)
	{
		GW_ItemSlotBundle* bundle = new GW_ItemSlotBundle();
		bundle->nPOS = nPos;
		bundle->nType = GW_ItemSlotBase::GW_ItemSlotType::INSTALL;
		bundle->Decode(iPacket);
		mItemSlot[3].insert({ bundle->nPOS, bundle });
	}

	while ((nPos = iPacket->Decode1()) != 0)
	{
		GW_ItemSlotBundle* bundle = new GW_ItemSlotBundle();
		bundle->nPOS = nPos;
		bundle->nType = GW_ItemSlotBase::GW_ItemSlotType::ETC;
		bundle->Decode(iPacket);
		mItemSlot[4].insert({ bundle->nPOS, bundle });
	}

	iPacket->Decode1(); //CASH

	iPacket->Decode4();
	iPacket->Decode4();
	iPacket->Decode4();

	iPacket->Decode4();
	iPacket->Decode4();

	iPacket->Decode1();
}

void GA_Character::DecodeAvatarLook(InPacket * iPacket)
{
	mAvatarData->Decode(iPacket);
}

void GA_Character::DecodeSkillRecord(InPacket * iPacket)
{
	bool flag = iPacket->Decode1() == 1;
	if (flag)
	{
		short nCount = iPacket->Decode2();
		for (int i = 0; i < nCount; ++i)
		{
			GW_SkillRecord* pSkillRecord = new GW_SkillRecord; 
			pSkillRecord->nMasterLevel = 0;
			pSkillRecord->Decode(iPacket);
			pSkillRecord->nCharacterID = nCharacterID;
			mSkillRecord.insert({ pSkillRecord->nSkillID, pSkillRecord });
		}
		iPacket->Decode2();
		iPacket->Decode4();
	}
}

void GA_Character::EncodeCharacterData(OutPacket *oPacket)
{
	long long int flag = 0xFFFFFFFFFFFFFFFF 	;
	oPacket->Encode8(flag);
	oPacket->Encode1(0);
	for (int i = 0; i < 3; ++i)
		oPacket->Encode4(-2);
	oPacket->Encode1(0);
	oPacket->Encode4(0);
	oPacket->Encode1(0);

	if (flag & 1)
	{
		EncodeStat(oPacket);
		oPacket->Encode1(25); //BUDDY LIST CAPACITY

		oPacket->Encode1(0); //BLESS OF FAIRY ORIGIN
		oPacket->Encode1(0); //BLESS OF EMPRESS ORIGIN
		oPacket->Encode1(0); //ULTRA EXPLORER

		oPacket->EncodeTime(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())); //TIME CURRENT TIME
		oPacket->EncodeTime(-2); //TIME -2
		oPacket->Encode4(0);
		oPacket->Encode1((char)0xFF);
		oPacket->Encode4(0);
		oPacket->Encode1((char)0xFF);
	}

	if (flag & 2)
	{
		oPacket->Encode8(mMoney->nMoney);
		oPacket->Encode4(nCharacterID);
		oPacket->Encode4(100);
		oPacket->Encode4(20);
	}

	if (flag & 8)
		oPacket->Encode4(0);

	if (flag & 0x80)
	{
		oPacket->Encode4(0);
		oPacket->Encode4(nCharacterID);
		for (int i = 0; i < 3; ++i)
		{
			oPacket->Encode4(0);
			oPacket->Encode4(0);
		}
		oPacket->Encode4(0);
		oPacket->Encode1(0);
		oPacket->Encode1(0);
		oPacket->Encode1(0);
	}

	oPacket->Encode1(64); //EQP SLOT
	oPacket->Encode1(64); //CON SLOT
	oPacket->Encode1(64); //INS SLOT
	oPacket->Encode1(64); //ETC SLOT
	oPacket->Encode1(64); //CASH SLOT

	EncodeInventoryData(oPacket);

	if (flag & 0x100)
	{
		EncodeSkillRecord(oPacket);
	}

	if (flag & 0x8000)
	{
		oPacket->Encode2(0);
	}

	if (flag & 0x200)
	{
		oPacket->Encode1(1);
		oPacket->Encode2((short)mQuestRecord.size());
		for (auto& record : mQuestRecord)
			record.second->Encode(oPacket);
		oPacket->Encode2(0);
	}

	if (flag & 0x4000)
	{
		oPacket->Encode1(1);
		oPacket->Encode2((short)mQuestComplete.size());
		for (auto& record : mQuestComplete)
			record.second->Encode(oPacket);
	}

	if (flag & 0x400)
	{
		oPacket->Encode2(0);
	}

	if (flag & 0x800)
	{
		oPacket->Encode2(0);
		oPacket->Encode2(0);
		oPacket->Encode2(0);
	}

	if (flag & 0x1000) //ROCK INFO
	{
		for (int i = 0; i < 5; ++i)
			oPacket->Encode4(0);
		for (int i = 0; i < 10; ++i)
			oPacket->Encode4(0);
		for (int i = 0; i < 13; ++i)
			oPacket->Encode4(0);
	}

	if (flag & 0x4000) //QUEST INFO
	{
		oPacket->Encode2(0);
	}

	if (flag & 0x20)
	{
		oPacket->Encode2(0);
	}

	oPacket->Encode1(1);

	if (flag & 0x40000000)
	{
		oPacket->Encode4(0);
	}

	if (flag & 0x1000)
	{
		oPacket->Encode4(0);
	}

	if (flag & 0x200000)
	{
		//JAGUAR INFO
	}

	//flag & 0x800 && is Zero

	if (flag & 0x4000000)
	{
		oPacket->Encode2(0);
	}

	if (flag & 0x20000000)
	{
		for (int i = 0; i < 15; ++i)
			oPacket->Encode4(0);
	}

	if (flag & 0x10000000)
	{
		for (int i = 0; i < 5; ++i)
			oPacket->Encode4(0);
	}

	if (flag & 0x80000000)
	{
		oPacket->Encode2(0); //ABILITY INFO
	}

	if (flag & 0x10000)
	{
		oPacket->Encode2(0);
	}

	if (flag & 0x01)
	{
		oPacket->Encode4(0); //HONOR LEVEL
		oPacket->Encode4(0); //HONOR EXP
		oPacket->Encode4(0);
		oPacket->Encode4(0);
		oPacket->Encode4(0);
	}

	if (flag & 0x2000)
	{
		for(int i = 0; i < 5; ++i)
			oPacket->Encode4(0);
		for (int i = 0; i < 6; ++i)
			oPacket->Encode4(0);
		for (int i = 0; i < 4; ++i)
			oPacket->Encode4(0);
		oPacket->EncodeTime(-2); //TIME
		oPacket->Encode1(0);
		oPacket->Encode1(1);
	}

	if (flag & 0x4000)
	{
		oPacket->Encode2(0);
	}

	if (flag & 0x8000)
	{
		oPacket->Encode2(0);
	}

	if (flag & 0x10000)
	{
		for (int i = 0; i < 14; ++i) //56
			oPacket->Encode4(0);
	}

	if (flag & 0x20000)
	{
		oPacket->Encode2(0);
	}

	if (flag & 0x2)
	{
		oPacket->Encode1(1);
		oPacket->Encode2(0);
	}

	if (flag & 0x4)
	{
		oPacket->Encode1(0);
	}

	//ANGELIC BUSTER
	oPacket->Encode4(0);
	oPacket->Encode4(0);
	oPacket->Encode4(0);

	oPacket->Encode1(0);
	oPacket->Encode4(-1);
	oPacket->Encode4(0);
	oPacket->Encode4(0);

	if (flag & 0x200000)
	{
		oPacket->Encode8(0);
		oPacket->Encode8(0);
	}

	if (flag & 0x10)
	{
		oPacket->Encode2(0);
		oPacket->Encode2(0);
	}

	oPacket->Encode1(0);
	if (flag & 0x400)
	{
		for (int i = 0; i < 4; ++i)
			oPacket->Encode4(0);
	}

	for (int i = 0; i < 7; ++i) //28
		oPacket->Encode4(0);

	if (flag & 0x8000000)
	{
		oPacket->Encode4(0);
		oPacket->Encode4(0);
		oPacket->Encode8(0);
	}

	oPacket->Encode2(0);
	oPacket->Encode2(0);
	oPacket->Encode4(0);

	if (flag & 0x10000000)
	{
		oPacket->Encode2(0);
	}

	if (flag & 0x20000000) //V MATRIX
	{
		oPacket->Encode4(0);
	}

	if (flag & 0x40000)
	{
		oPacket->Encode1(0);
		oPacket->Encode2(0);
		oPacket->Encode2(0);
	}

	if (flag & 0x80000)
	{
		oPacket->Encode1(0);
	}

	if (flag & 0x100000)
	{
		oPacket->Encode4(0);
		oPacket->Encode4(0);
	}
}

void GA_Character::EncodeInventoryData(OutPacket *oPacket)
{
	oPacket->EncodeTime(-2); // TIME
	oPacket->Encode1(0);

	for (const auto &eqp : mItemSlot[1])
		if (eqp.second->nPOS < 0 && eqp.second->nPOS > -100)
			eqp.second->Encode(oPacket);
	oPacket->Encode2(0); //EQUIPPED

	for (const auto &eqp : mItemSlot[1])
		if (eqp.second->nPOS <= -100 && eqp.second->nPOS > -1000)
			eqp.second->Encode(oPacket);
	oPacket->Encode2(0); //EQUIPPED 2

	for (const auto &eqp : mItemSlot[1])
		if (eqp.second->nPOS >= 0) // < -10000��ܭn�R�������~�B�u�|�o�ͦbWvsGame -> WvsCenter
			eqp.second->Encode(oPacket);
	oPacket->Encode2(0);
						 //sub_69B50A
	oPacket->Encode2(0); //sub_69B50A 1
	oPacket->Encode2(0); //sub_69B50A 2
	oPacket->Encode2(0); //sub_69B50A 3
	oPacket->Encode2(0); //sub_69B50A 4
	oPacket->Encode2(0); //sub_69B50A 5
	oPacket->Encode2(0); //sub_69B50A 6
	oPacket->Encode2(0); //sub_69B50A 7
	oPacket->Encode2(0); //sub_69B50A 8
	oPacket->Encode2(0); //sub_69B50A 9
	oPacket->Encode2(0); //sub_69B50A 10
	oPacket->Encode2(0); //sub_69B50A 11

						 //sub_68CB08
	oPacket->Encode2(0); //sub_68CB08 1
	oPacket->Encode2(0); //sub_68CB08 2

						 //sub_68CF44
	oPacket->Encode2(0); //sub_68CF44 1
	oPacket->Encode2(0); //sub_68CF44 2

	for (auto &item : mItemSlot[2])
		item.second->Encode(oPacket);
	oPacket->Encode1(0); //USE

	for (auto &item : mItemSlot[3])
		item.second->Encode(oPacket);
	oPacket->Encode1(0); //INS

	for (auto &item : mItemSlot[4])
		item.second->Encode(oPacket);
	oPacket->Encode1(0); //ETC


	oPacket->Encode1(0); //CASH

	oPacket->Encode4(0);
	oPacket->Encode4(0);
	oPacket->Encode4(0);

	oPacket->Encode4(0);
	oPacket->Encode4(0);

	oPacket->Encode1(0);
}

GA_Character::ATOMIC_COUNT_TYPE GA_Character::InitCharacterID()
{
	Poco::Data::Statement queryStatement(GET_DB_SESSION);
	queryStatement << "SELECT IFNULL((SELECT MAX(CharacterID) From Characters),(0)) As Totle";
	queryStatement.execute();
	Poco::Data::RecordSet recordSet(queryStatement);
	if (recordSet.rowCount() == 0)
		return 0;
	return (ATOMIC_COUNT_TYPE)recordSet["Totle"];
}

void GA_Character::LoadItemSlot()
{

	Poco::Data::Statement queryStatement(GET_DB_SESSION);
	queryStatement << "SELECT SN FROM ItemSlot_EQP Where CharacterID = " << nCharacterID;
	queryStatement.execute();
	Poco::Data::RecordSet recordSet(queryStatement);

	for (int i = 0; i < recordSet.rowCount(); ++i, recordSet.moveNext()) {
		GW_ItemSlotEquip *eqp = new GW_ItemSlotEquip();
		eqp->Load(recordSet["SN"]);
		mItemSlot[1][eqp->nPOS] = eqp;
	}

	queryStatement.reset(GET_DB_SESSION);
	queryStatement << "SELECT SN FROM ItemSlot_CON Where CharacterID = " << nCharacterID;
	queryStatement.execute();
	recordSet.reset(queryStatement);
	for (int i = 0; i < recordSet.rowCount(); ++i, recordSet.moveNext()) {
		GW_ItemSlotBundle *eqp = new GW_ItemSlotBundle();
		eqp->Load(recordSet["SN"], GW_ItemSlotBase::GW_ItemSlotType::CONSUME);
		mItemSlot[2][eqp->nPOS] = eqp;
	}

	queryStatement.reset(GET_DB_SESSION);
	queryStatement << "SELECT SN FROM ItemSlot_ETC Where CharacterID = " << nCharacterID;
	queryStatement.execute();
	recordSet.reset(queryStatement);
	for (int i = 0; i < recordSet.rowCount(); ++i, recordSet.moveNext()) {
		GW_ItemSlotBundle *eqp = new GW_ItemSlotBundle();
		eqp->Load(recordSet["SN"], GW_ItemSlotBase::GW_ItemSlotType::ETC);
		mItemSlot[3][eqp->nPOS] = eqp;
	}

	queryStatement.reset(GET_DB_SESSION);
	queryStatement << "SELECT SN FROM ItemSlot_INS Where CharacterID = " << nCharacterID;
	queryStatement.execute();
	recordSet.reset(queryStatement);
	for (int i = 0; i < recordSet.rowCount(); ++i, recordSet.moveNext()) {
		GW_ItemSlotBundle *eqp = new GW_ItemSlotBundle();
		eqp->Load(recordSet["SN"], GW_ItemSlotBase::GW_ItemSlotType::INSTALL);
		mItemSlot[4][eqp->nPOS] = eqp;
	}
}

void GA_Character::LoadSkillRecord()
{
	Poco::Data::Statement queryStatement(GET_DB_SESSION);
	queryStatement << "SELECT * FROM SkillRecord Where CharacterID = " << nCharacterID;
	queryStatement.execute();
	Poco::Data::RecordSet recordSet(queryStatement);

	for (int i = 0; i < recordSet.rowCount(); ++i, recordSet.moveNext()) {
		GW_SkillRecord* pSkillRecord = new GW_SkillRecord();
		pSkillRecord->Load((void*)&recordSet);
		mSkillRecord.insert({ pSkillRecord->nSkillID, pSkillRecord });
	}
}

void GA_Character::LoadQuestRecord()
{
	Poco::Data::Statement queryStatement(GET_DB_SESSION);
	queryStatement << "SELECT * FROM QuestRecord Where CharacterID = " << nCharacterID;
	queryStatement.execute();
	Poco::Data::RecordSet recordSet(queryStatement);

	for (int i = 0; i < recordSet.rowCount(); ++i, recordSet.moveNext()) {
		GW_QuestRecord* pQuestRecord = new GW_QuestRecord;
		pQuestRecord->Load((void*)&recordSet);
		if(pQuestRecord->nState == 1)
			mQuestRecord.insert({ pQuestRecord->nQuestID, pQuestRecord });
		else
			mQuestComplete.insert({ pQuestRecord->nQuestID, pQuestRecord });
	}
}
