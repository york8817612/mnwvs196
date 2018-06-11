#include "Field.h"
#include "LifePool.h"
#include "Net\PacketFlags\MobPacketFlags.hpp"
#include "Net\InPacket.h"
#include "Net\OutPacket.h"
#include "Mob.h"
#include "MovePath.h"
#include "PortalMap.h"
#include "TownPortalPool.h"
#include "DropPool.h"

#include <mutex>
#include <functional>


std::mutex fieldUserMutex;

Field::Field()
	: m_pLifePool(new LifePool), 
	  m_updateBinder(std::bind(&Field::UpdateTrigger, this)),
	  m_pPortalMap(new PortalMap),
	  m_pTownPortalPool(new TownPortalPool)
{
	m_pDropPool = new DropPool(this);
	m_asyncUpdateTimer = AsnycScheduler::CreateTask(m_updateBinder, 5000, true);
	//this->m_asyncUpdateTimer = (void*)timer;
	InitLifePool();
}

Field::~Field()
{
	delete m_pLifePool;
	delete m_pPortalMap;
	m_asyncUpdateTimer->Abort();
	delete m_asyncUpdateTimer;
}

void Field::BroadcastPacket(OutPacket * oPacket)
{
	std::lock_guard<std::mutex> userGuard(fieldUserMutex);
	for (auto& user : m_mUser)
		user.second->SendPacket(oPacket);
}

void Field::SetCould(bool cloud)
{
	m_bCloud = cloud;
}

bool Field::IsCloud() const
{
	return m_bCloud;
}

void Field::SetTown(bool town)
{
	m_bTown = town;
}

bool Field::IsTown() const
{
	return m_bTown;
}

void Field::SetSwim(bool swim)
{
	m_bSwim = swim;
}

bool Field::IsSwim() const
{
	return m_bSwim;
}

void Field::SetFly(bool fly)
{
	m_bFly = fly;
}

bool Field::IsFly() const
{
	return m_bFly;
}

void Field::SetFieldID(int nFieldID)
{
	this->m_nFieldID = nFieldID;
}

int Field::GetFieldID() const
{
	return m_nFieldID;
}

void Field::SetReturnMap(int returnMap)
{
	m_nReturnMap = returnMap;
}

int Field::GetReturnMap() const
{
	return m_nReturnMap;
}

void Field::SetForcedReturn(int forcedReturn)
{
	m_nForcedReturn = forcedReturn;
}

int Field::GetForcedReturn() const
{
	return m_nForcedReturn;
}

void Field::SetMobRate(int mobRate)
{
	m_nMobRate = mobRate;
}

int Field::GetMobRate() const
{
	return m_nMobRate;
}

void Field::SetFieldType(int fieldType)
{
	m_nFieldType = fieldType;
}

int Field::GetFieldType() const
{
	return m_nFieldType;
}

void Field::SetFieldLimit(int fieldLimit)
{
	m_nFieldLimit = fieldLimit;
}

int Field::GetFieldLimit() const
{
	return m_nFieldLimit;
}

void Field::SetCreateMobInterval(int interval)
{
	m_nCreateMobInterval = interval;
}

int Field::GetCreateMobInterval() const
{
	return m_nCreateMobInterval;
}

void Field::SetFiexdMobCapacity(int capacity)
{
	m_nFixedMobCapacity = capacity;
}

int Field::GetFixedMobCapacity() const
{
	return m_nFixedMobCapacity;
}

void Field::SetFirstUserEnter(const std::string & script)
{
	m_strFirstUserEnter = script;
}

const std::string & Field::GetFirstUserEnter() const
{
	return m_strFirstUserEnter;
}

void Field::SetUserEnter(const std::string & script)
{
	m_strUserEnter = script;
}

void Field::SetMapSizeX(int x)
{
	m_nMapSizeX = x;
}

int Field::GetMapSizeX()
{
	return m_nMapSizeX;
}

void Field::SetMapSizeY(int y)
{
	m_nMapSizeY = y;
}

int Field::GetMapSizeY()
{
	return m_nMapSizeY;
}

void Field::InitLifePool()
{
	std::lock_guard<std::mutex> lifePoolGuard(fieldUserMutex);
	m_pLifePool->Init(this, m_nFieldID);
}

LifePool * Field::GetLifePool()
{
	return m_pLifePool;
}

DropPool * Field::GetDropPool()
{
	return m_pDropPool;
}

void Field::OnEnter(User *pUser)
{
	std::lock_guard<std::mutex> userGuard(fieldUserMutex);
	if (!m_asyncUpdateTimer->IsStarted())
		m_asyncUpdateTimer->Start();
	m_mUser[pUser->GetUserID()] = pUser;
	m_pLifePool->OnEnter(pUser);
	m_pDropPool->OnEnter(pUser);
}

void Field::OnLeave(User * pUser)
{
	std::lock_guard<std::mutex> userGuard(fieldUserMutex);
	m_mUser.erase(pUser->GetUserID());
	m_pLifePool->RemoveController(pUser);
	if (m_mUser.size() == 0 && m_asyncUpdateTimer->IsStarted())
		m_asyncUpdateTimer->Abort();
}

//�o�eoPacket���Ӧa�Ϫ���LUser�A�䤤pExcept�O�ҥ~��H
void Field::SplitSendPacket(OutPacket *oPacket, User *pExcept)
{
	std::lock_guard<std::mutex> userGuard(fieldUserMutex);
	for (auto& user : m_mUser)
	{
		if ((pExcept == nullptr) || user.second->GetUserID() != pExcept->GetUserID())
			user.second->SendPacket(oPacket);
	}
}

void Field::OnPacket(User* pUser, InPacket *iPacket)
{
	int nType = iPacket->Decode2();
	if (nType >= 0x369 && nType <= 0x384)
		m_pLifePool->OnPacket(pUser, nType, iPacket);
	else if (nType == 0x38B)
		m_pDropPool->OnPacket(pUser, nType, iPacket);
	//if(nHeader >= )
	/*if (nHeader >= MobRecvPacketFlag::MobRecvPacketFlag::minFlag && nHeader <= MobRecvPacketFlag::MobRecvPacketFlag::maxFlag)
	{
		printf("Mob Packet Received %d.\n", (int)nHeader);
	}*/

}

void Field::OnUserMove(User * pUser, InPacket * iPacket)
{
	iPacket->Decode1(); //CurFieldKey
	iPacket->Decode8(); //UNK
	iPacket->Decode1(); //UNK
	MovePath movePath;
	movePath.Decode(iPacket);
	auto& lastElem = movePath.m_lElem.rbegin();
	pUser->SetMovePosition(lastElem->x, lastElem->y, lastElem->bMoveAction, lastElem->fh);
	OutPacket oPacket;
	oPacket.Encode2(0x295);
	oPacket.Encode4(pUser->GetUserID());
	movePath.Encode(&oPacket);
	BroadcastPacket(&oPacket);
}

PortalMap * Field::GetPortalMap()
{
	return m_pPortalMap;
}

TownPortalPool * Field::GetTownPortalPool()
{
	return m_pTownPortalPool;
}

void Field::OnMobMove(User * pCtrl, Mob * pMob, InPacket * iPacket)
{
	//std::lock_guard<std::mutex> lifeGuard(m_pLifePool->GetLock());
	bool bIsAzawanMoving = iPacket->Decode1() > 0;
	/*
	  v122 = CMobTemplate::_ZtlSecureGet_dwTemplateID(v121) / 0x2710 == 250
      || CMobTemplate::_ZtlSecureGet_dwTemplateID(v121) / 0x2710 == 251;
		 COutPacket::Encode1(&oPacket, v122);
	*/

	//_ZtlSecureTear_m_nMobCtrlSN
	short nMobCtrlSN = iPacket->Decode2();

	char flag = iPacket->Decode1();
	bool bNextAttackPossible = (flag & 0xFF) > 0;
	bool pCenterSplit = iPacket->Decode1() == 1;

	//ZtlSecureTear_bSN_CS
	char nSkillCommand = iPacket->Decode1();
	char nSLV = iPacket->Decode1();
	short nSkillEffect = iPacket->Decode2();

	std::list<std::pair<short, short>> aUnkList1;
	std::list<short> aUnkList2;

	short unkValue1, unkValue2;

	int unkCount = iPacket->Decode1();
	for (int i = 0; i < unkCount; ++i)
	{
		unkValue1 = iPacket->Decode2();
		unkValue2 = iPacket->Decode2();
		aUnkList1.push_back({ unkValue1, unkValue2 });
	}

	unkCount = iPacket->Decode1();
	for (int i = 0; i < unkCount; ++i)
	{
		unkValue1 = iPacket->Decode2();
		aUnkList2.push_back(unkValue1);
	}

	iPacket->Decode1();
	bool bSkipped = iPacket->Decode4() > 0 && aUnkList2.size() > 0;
	iPacket->Decode8();
	iPacket->Decode4();
	if (iPacket->Decode1() == 0 && Mob::IsLucidSpecialMob(pMob->GetTemplateID()))
		nSLV = (char)iPacket->Decode2();
	MovePath movePath;
	movePath.Decode(iPacket);

	//Encode Ctrl Ack Packet
	OutPacket ctrlAckPacket;
	ctrlAckPacket.Encode2(MobSendPacketFlag::SP_MobCtrlAck);
	ctrlAckPacket.Encode4(pMob->GetFieldObjectID());
	ctrlAckPacket.Encode2(nMobCtrlSN);
	ctrlAckPacket.Encode1(bNextAttackPossible);
	ctrlAckPacket.Encode4((int)pMob->GetMp());
	ctrlAckPacket.Encode4(nSkillCommand);
	ctrlAckPacket.Encode1(nSLV);
	ctrlAckPacket.Encode4(0); //nForcedAttackIdx
	

	//Encode Move Packet
	OutPacket movePacket;
	movePacket.Encode2(MobSendPacketFlag::SP_MobMove); //CMob::OnMove
	movePacket.Encode4(pMob->GetFieldObjectID());
	movePacket.Encode1(bNextAttackPossible);
	movePacket.Encode1(pCenterSplit);

	//dwData
	movePacket.Encode1(nSkillCommand);
	movePacket.Encode1(nSLV);
	movePacket.Encode2(nSkillEffect);

	movePacket.Encode1((char)aUnkList1.size());
	for (const auto& p : aUnkList1)
	{
		movePacket.Encode2(p.first);
		movePacket.Encode2(p.second);
	}

	movePacket.Encode1((char)aUnkList2.size());
	for (short value : aUnkList2)
		movePacket.Encode2(value);
	movePath.Encode(&movePacket);

	for (const auto& elem : movePath.m_lElem)
	{
		pMob->SetPosX(elem.x);
		pMob->SetPosY(elem.y - 1);
		pMob->SetMoveAction(elem.bMoveAction);
		if(elem.fh != 0)
			pMob->SetFh(elem.fh);
	}

	SplitSendPacket(&movePacket, nullptr);
	pCtrl->SendPacket(&ctrlAckPacket);
}

void Field::Update()
{
	//printf("Field Update Called\n");
	m_pLifePool->Update();
}
