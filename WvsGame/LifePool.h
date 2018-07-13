#pragma once
#include <map>
#include "Npc.h"
#include "Mob.h"
#include "..\WvsLib\Wz\WzResMan.hpp"
#include <atomic>
#include <mutex>

struct AttackInfo;
class User;
class Field;
class Controller;
class InPacket;
class SkillEntry;
class Drop;

class LifePool
{
	const static int MAX_WINDOW_VIEW_X = 1280, MAX_WINDOW_VIEW_Y = 1024, MAX_MOB_GEN = 40;

	std::mutex m_lifePoolMutex;

	std::atomic<int> atomicObjectCounter = 0x1000;

	//�b�o�Ӧa�Ϥ��Ҧ��i�઺NPC����
	std::vector<Npc> m_lNpc;

	//�b�o�Ӧa�Ϥ��Ҧ��i�઺�Ǫ�����
	std::vector<Mob> m_lMob;

	//�b�o�Ӧa�Ϥ��u�����ͥX���Ǫ�, key�OObject ID
	std::map<int, Mob*> m_aMobGen;

	//�b�o�Ӧa�Ϥ��u�����ͥX��Npc, key�OObject ID
	std::map<int, Npc*> m_aNpcGen;

	/* �a�Ϥ��������a�����v�������A�ϥ�std::map ���������A�Ы� Max Heap �P Min Heap
	   Key�O ����ƶq Value : Controller�O�N����̪��������
	   User�����O�smap��iterator �H�K�i��ֳt�R�� [ std::map��iterator���|�]���e�������s�W�R���ӵL�� ]
	*/
	std::multimap<int, Controller*> m_hCtrl;

	/*
	��a�Ϥ��L���󪱮a�i�@��Controller�A�N�Ҧ��Ǫ������v��浹Null Controller�A��K�޲z
	*/
	Controller* m_pCtrlNull;

	//����User�����쪺m_hCtrl iterator
	std::map<int, decltype(m_hCtrl)::iterator> m_mController;

	//�]�w����b�a�Ϥ������ݩ�
	void SetFieldObjAttribute(FieldObj* pFieldObj, WZ::Node& dataNode);

	//���즳�����ݩ�
	int m_nMobCapacityMin, m_nMobCapacityMax, m_aInitMobGenCount = 0, m_nSubMobCount = -1, m_nMobDamagedByMobState = 0;

	bool m_bMobGenEnable = true;

	Field* m_pField;

	void OnMobPacket(User* pUser, int nType, InPacket* iPacket);
	void OnNpcPacket(User* pUser, int nType, InPacket* iPacket);
public:
	LifePool();
	~LifePool();

	void SetMaxMobCapacity(int max)
	{
		m_nMobCapacityMax = max;
	}

	int GetMaxMobCapacity() const
	{
		return m_nMobCapacityMax;
	}

	//�qWz��Ū�����a�Ϥ�Npc��T�A�s��m_lNpc��
	void LoadNpcData(WZ::Node& dataNode);

	//�qWz��Ū�����a�Ϥ�Mob��T�A�s��m_lMob��
	void LoadMobData(WZ::Node& dataNode);

	/*
	���ͥXNpc����
	*/
	void CreateNpc(const Npc&);

	/*
	�Y�a�Ϥ����Ǫ��ƶq�����h�H���D��lMob��CreateMob
	*/
	void TryCreateMob(bool reset);

	/*
	�N���w��Mob�l��X�å[�JaMobGen
	*/
	void CreateMob(const Mob&, int x, int y, int fh, int bNoDropPriority, int nType, unsigned int dwOption, int bLeft, int nMobType, Controller* pOwner);
	void RemoveMob(Mob* pMob);

	void Init(Field* pField, int nFieldID);
	void OnEnter(User* pUser);

	/*
	�����a�i�J�a�ϡA���JController�í��s���t�Ǫ������v[RedistributeLife]
	*/
	void InsertController(User* pUser);

	/*
	���a���}�a�ϡA�N�Ӫ��a����Ǫ����t��minCtrl��nullCtrl
	*/
	void RemoveController(User* pUser);

	/*
	Controller���ʮɡA�q�s���JhCtrl��
	*/
	void UpdateCtrlHeap(Controller* pController);

	/*
	�٨S�g, �@�Τ���
	*/
	bool GiveUpMobController(Controller* pController);

	/*
	���s���t�a�Ϥ����Ǫ������v
	���NnullCtrl���Ǫ����t��minCtrl ���ۥ���maxCtrl�PminCtrl������ƶq
	*/
	void RedistributeLife();

	void Update();

	void OnPacket(User* pUser, int nType, InPacket* iPacket);
	void OnUserAttack(User *pUser, const SkillEntry *pSkill, AttackInfo *pInfo);
	void EncodeAttackInfo(User * pUser, AttackInfo *pInfo, OutPacket *oPacket);

	std::mutex& GetLock();

	Npc* GetNpc(int nFieldObjID);
	Mob* GetMob(int nFieldObjID);
};

