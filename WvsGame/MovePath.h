#pragma once
#include <list>
class InPacket;
class OutPacket;

struct MovePath
{
	enum MovePathAttribute
	{
		MPA_NORMAL = 0x0,
		MPA_JUMP = 0x1,
		MPA_IMPACT = 0x2,
		MPA_IMMEDIATE = 0x3,
		MPA_TELEPORT = 0x4,
		MPA_RANDOM_TELEPORT = 0x5,
		MPA_DEMON_TRACE_TELEPORT = 0x6,
		MPA_RETURN_TELEPORT = 0x7,
		MPA_HANGONBACK = 0x8,
		MPA_ASSAULTER = 0x9,
		MPA_ASSASSINATION = 0xA,
		MPA_RUSH = 0xB,
		MPA_STATCHANGE = 0xC,
		MPA_SITDOWN = 0xD,
		MPA_STARTFALLDOWN = 0xE,
		MPA_FALLDOWN = 0xF,
		MPA_STARTDRAGDOWN = 0x10,
		MPA_DRAGDOWN = 0x11,
		MPA_STARTWINGS = 0x12,
		MPA_WINGS = 0x13,
		MPA_ARAN_ADJUST = 0x14,
		MPA_MOB_TOSS = 0x15,
		MPA_MOB_TOSS_SLOWDOWN = 0x16,
		MPA_FLYING_BLOCK = 0x17,
		MPA_DASH_SLIDE = 0x18,
		MPA_BMAGE_ADJUST = 0x19,
		MPA_BLINK_LIGHT = 0x1A,
		MPA_TELEPORT_ZERO1 = 0x1B,

		MPA_FLASHJUMP = 0x1D,
		MPA_DOUBLE_JUMP,
		MPA_DOUBLE_JUMP_DOWN,
		MPA_TRIPLE_JUMP,
		MPA_FLASHJUMP_CHANGEEFF,
		MPA_ROCKET_BOOSTER,
		MPA_BACKSTEP_SHOT,
		MPA_CANNON_JUMP,
		MPA_QUICK_SILVER_JUMP,
		MPA_MOBPOWERKNOCKBACK,
		MPA_VERTICALJUMP,
		MPA_CUSTOMIMPACT,
		MPA_CUSTOMIMPACT2,
		MPA_COMBATSTEP,
		MPA_HIT,
		MPA_TIMEBOMBATTACK,
		MPA_SNOWBALLTOUCH,
		MPA_BUFFZONEEFFECT,
		MPA_LEAF_TORNADO,
		MPA_STYLISH_ROPE,
		MPA_ROPE_CONNECT,
		MPA_STRIKER_UPPERCUT,
		MPA_CRAWL,
		MPA_TELEPORT_BY_MOBSKILLAREA,
		MPA_ZERO_TAG,
		MPA_RETREAT_SHOT,
		MPA_DB_BLADE_ASCENSION,
		MPA_IMPACT_IGNORE_MOVEPATH,
		MPA_ANGLE_IMPACT,
		MPA_UNK1,
		MPA_STARPLANET_RIDING_BOOSTER,
		MPA_USER_TOSS,
		MPA_SLASH_JUMP,
		MPA_MOB_LADDER,
		MPA_MOB_RIGHTANGLE,
		MPA_MOB_STOPNODE_START,
		MPA_MOB_BEFORE_NODE,
		MPA_MOB_TELEPORT,
		MPA_MOB_ATTACK_RUSH,
		MPA_MOB_ATTACK_RUSH_STOP,
		MPA_MOB_ATTACK_LEAP,
		MPA_BATTLEPVP_MUGONG_SOMER_SAULT,
		MPA_BATTLEPVP_HELENA_STEPSHOT,
		MPA_SUN_OF_GLORY,
		MPA_HOOKSHOT_START,
		MPA_HOOKSHOT,
		MPA_HOOKSHOT_END,
		MPA_PINKBEAN_POGO_STICK,
		MPA_PINKBEAN_POGO_STICK_END,
		MPA_PINKBEAN_ROLLING_AIR,
		MPA_FINAL_TOSS,
		MPA_TELEPORT_KINESIS1,
		MPA_NIGHTLORD_SHADOWWEB,
		MPA_TELEPORT_ARAN1,
		MPA_RW_EXPLOSION_CANNON,
		MPA_UNK2,
		MPA_UNK3,
		MPA_UNK4,
	};

	struct ELEM
	{
		short x, y, vx, vy, offsetX, offsetY, fh, fhFootStart;
		char bMoveAction, bForcedStop, bStat;
		int tElapse, nAttr;
	};
	short m_x, m_y, m_vx, m_vy;
	int m_fhLast, m_tEncodedGatherDuration;

	std::list<ELEM> m_lElem;

	MovePath();
	~MovePath();

	void Decode(InPacket* iPacket);
	void Encode(OutPacket* oPacket);
	
};

