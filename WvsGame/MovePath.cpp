#include "MovePath.h"

#include <..\WvsLib\Net\InPacket.h>
#include <..\WvsLib\Net\OutPacket.h>

MovePath::MovePath()
{
}


MovePath::~MovePath()
{
}

void MovePath::Decode(InPacket * iPacket)
{
	m_tEncodedGatherDuration = iPacket->Decode4();
	m_x = iPacket->Decode2();
	m_y = iPacket->Decode2();
	m_vx = iPacket->Decode2();
	m_vy = iPacket->Decode2();
	int nCount = iPacket->Decode1();
	for (int i = 0; i < nCount; ++i)
	{
		ELEM elem;
		elem.nAttr = iPacket->Decode1();
		switch (elem.nAttr)
		{
			case MPA_NORMAL:
			case MPA_HANGONBACK:
			case MPA_FALLDOWN:
			case MPA_DRAGDOWN:
			case MPA_WINGS:
			case MPA_MOB_ATTACK_RUSH:
			case MPA_MOB_ATTACK_RUSH_STOP:
			case MPA_MOB_ATTACK_LEAP: 
			{
				elem.x = iPacket->Decode2();
				elem.y = iPacket->Decode2();
				elem.vx = iPacket->Decode2();
				elem.vy = iPacket->Decode2();
				elem.fh = iPacket->Decode2();
				
				if (elem.nAttr == MPA_FALLDOWN || elem.nAttr == MPA_DRAGDOWN)
					elem.fhFootStart = iPacket->Decode2();

				elem.offsetX = iPacket->Decode2();
				elem.offsetY = iPacket->Decode2();
				break;
			}
			case MPA_IMPACT_IGNORE_MOVEPATH:
			case MPA_MOB_TELEPORT:
			case MPA_UNK4:
			{
				elem.x = iPacket->Decode2();
				elem.y = iPacket->Decode2();
				elem.vx = iPacket->Decode2();
				elem.vy = iPacket->Decode2();
				elem.fh = iPacket->Decode2();
				break;
			}
			case MPA_JUMP:
			case MPA_IMPACT:
			case MPA_STARTWINGS:
			case MPA_MOB_TOSS:
			case MPA_MOB_TOSS_SLOWDOWN:
			case MPA_DASH_SLIDE:
			case MPA_MOB_LADDER:
			case MPA_MOB_RIGHTANGLE:
			case MPA_MOB_STOPNODE_START:
			case MPA_MOB_BEFORE_NODE:
			{
				elem.vx = iPacket->Decode2();
				elem.vy = iPacket->Decode2();
				if (elem.nAttr == MPA_MOB_TOSS || elem.nAttr == MPA_MOB_TOSS_SLOWDOWN)
					elem.fhFootStart = iPacket->Decode2();
				break;
			}
			case MPA_FLASHJUMP:
			case MPA_DOUBLE_JUMP:
			case MPA_DOUBLE_JUMP_DOWN:
			case MPA_TRIPLE_JUMP:
			case MPA_FLASHJUMP_CHANGEEFF:
			case MPA_ROCKET_BOOSTER:
			case MPA_BACKSTEP_SHOT:
			case MPA_CANNON_JUMP:
			case MPA_QUICK_SILVER_JUMP:
			case MPA_MOBPOWERKNOCKBACK:
			case MPA_VERTICALJUMP:
			case MPA_CUSTOMIMPACT:
			case MPA_CUSTOMIMPACT2:
			case MPA_COMBATSTEP:
			case MPA_HIT:
			case MPA_TIMEBOMBATTACK:
			case MPA_SNOWBALLTOUCH:
			case MPA_BUFFZONEEFFECT:
			case MPA_LEAF_TORNADO:
			case MPA_STYLISH_ROPE:
			case MPA_ROPE_CONNECT:
			case MPA_STRIKER_UPPERCUT:
			case MPA_CRAWL:
			case MPA_DB_BLADE_ASCENSION:
			case MPA_ANGLE_IMPACT:
			case MPA_STARPLANET_RIDING_BOOSTER:
			case MPA_USER_TOSS:
			case MPA_SLASH_JUMP:
			case MPA_BATTLEPVP_MUGONG_SOMER_SAULT:
			case MPA_BATTLEPVP_HELENA_STEPSHOT:
			case MPA_SUN_OF_GLORY:
			case MPA_HOOKSHOT:
			case MPA_FINAL_TOSS:
			case MPA_NIGHTLORD_SHADOWWEB:
			case MPA_RW_EXPLOSION_CANNON:
			case MPA_UNK2: {
				elem.x = m_x;
				elem.y = m_y;
				break;
			}
			case MPA_IMMEDIATE:
			case MPA_TELEPORT:
			case MPA_RANDOM_TELEPORT:
			case MPA_DEMON_TRACE_TELEPORT:
			case MPA_RETURN_TELEPORT:
			case MPA_ASSAULTER:
			case MPA_ASSASSINATION:
			case MPA_RUSH:
			case MPA_SITDOWN:
			case MPA_BLINK_LIGHT:
			case MPA_TELEPORT_ZERO1:
			case MPA_TELEPORT_BY_MOBSKILLAREA:
			case MPA_ZERO_TAG:
			case MPA_RETREAT_SHOT:
			case MPA_UNK1:
			case MPA_PINKBEAN_POGO_STICK:
			case MPA_PINKBEAN_POGO_STICK_END:
			case MPA_PINKBEAN_ROLLING_AIR:
			case MPA_TELEPORT_KINESIS1:
			case MPA_TELEPORT_ARAN1:
			{
				elem.x = iPacket->Decode2();
				elem.y = iPacket->Decode2();
				elem.fh = iPacket->Decode2();
				break;
			}
			case MPA_STARTFALLDOWN:
			case MPA_STARTDRAGDOWN:
			{
				elem.vx = iPacket->Decode2();
				elem.vy = iPacket->Decode2();
				elem.fhFootStart = iPacket->Decode2();
				break;
			}
			case MPA_FLYING_BLOCK: {
				elem.x = iPacket->Decode2();
				elem.y = iPacket->Decode2();
				elem.vx = iPacket->Decode2();
				elem.vy = iPacket->Decode2();
				break;
			}
			case MPA_STATCHANGE: {
				elem.x = m_x;
				elem.y = m_y;
				elem.bStat = iPacket->Decode1();
				break;
			}
		}
		if (elem.nAttr != MPA_STATCHANGE)
		{
			elem.bMoveAction = iPacket->Decode1();
			elem.tElapse = iPacket->Decode2();
			elem.bForcedStop = iPacket->Decode1();
		}
		m_lElem.push_back(std::move(elem));
	}
}

void MovePath::Encode(OutPacket * oPacket)
{
	oPacket->Encode4(m_tEncodedGatherDuration);
	oPacket->Encode2(m_x);
	oPacket->Encode2(m_y);
	oPacket->Encode2(m_vx);
	oPacket->Encode2(m_vy);
	oPacket->Encode1((char)m_lElem.size());
	for (const auto& elem : m_lElem)
	{
		oPacket->Encode1(elem.nAttr);
		switch (elem.nAttr)
		{
			case MPA_NORMAL:
			case MPA_HANGONBACK:
			case MPA_FALLDOWN:
			case MPA_DRAGDOWN:
			case MPA_WINGS:
			case MPA_MOB_ATTACK_RUSH:
			case MPA_MOB_ATTACK_RUSH_STOP:
			case MPA_MOB_ATTACK_LEAP:
			{
				oPacket->Encode2(elem.x);
				oPacket->Encode2(elem.y);
				oPacket->Encode2(elem.vx);
				oPacket->Encode2(elem.vy);
				oPacket->Encode2(elem.fh);

				if (elem.nAttr == MPA_FALLDOWN || elem.nAttr == MPA_DRAGDOWN)
					oPacket->Encode2(elem.fhFootStart);

				oPacket->Encode2(elem.offsetX);
				oPacket->Encode2(elem.offsetY);
				m_fhLast = elem.fh;
				break;
			}
			case MPA_IMPACT_IGNORE_MOVEPATH:
			case MPA_MOB_TELEPORT:
			case MPA_UNK4: {
				oPacket->Encode2(elem.x);
				oPacket->Encode2(elem.y);
				oPacket->Encode2(elem.vx);
				oPacket->Encode2(elem.vy);
				oPacket->Encode2(elem.fh);
				m_fhLast = elem.fh;
				break;
			}
			case MPA_JUMP:
			case MPA_IMPACT:
			case MPA_STARTWINGS:
			case MPA_MOB_TOSS:
			case MPA_MOB_TOSS_SLOWDOWN:
			case MPA_DASH_SLIDE:
			case MPA_MOB_LADDER:
			case MPA_MOB_RIGHTANGLE:
			case MPA_MOB_STOPNODE_START:
			case MPA_MOB_BEFORE_NODE:
			{
				oPacket->Encode2(elem.vx);
				oPacket->Encode2(elem.vy);
				if (elem.nAttr == MPA_MOB_TOSS || elem.nAttr == MPA_MOB_TOSS_SLOWDOWN)
					oPacket->Encode2(elem.fhFootStart);
				break;
			}
			case MPA_FLASHJUMP:
			case MPA_DOUBLE_JUMP:
			case MPA_DOUBLE_JUMP_DOWN:
			case MPA_TRIPLE_JUMP:
			case MPA_FLASHJUMP_CHANGEEFF:
			case MPA_ROCKET_BOOSTER:
			case MPA_BACKSTEP_SHOT:
			case MPA_CANNON_JUMP:
			case MPA_QUICK_SILVER_JUMP:
			case MPA_MOBPOWERKNOCKBACK:
			case MPA_VERTICALJUMP:
			case MPA_CUSTOMIMPACT:
			case MPA_CUSTOMIMPACT2:
			case MPA_COMBATSTEP:
			case MPA_HIT:
			case MPA_TIMEBOMBATTACK:
			case MPA_SNOWBALLTOUCH:
			case MPA_BUFFZONEEFFECT:
			case MPA_LEAF_TORNADO:
			case MPA_STYLISH_ROPE:
			case MPA_ROPE_CONNECT:
			case MPA_STRIKER_UPPERCUT:
			case MPA_CRAWL:
			case MPA_DB_BLADE_ASCENSION:
			case MPA_ANGLE_IMPACT:
			case MPA_STARPLANET_RIDING_BOOSTER:
			case MPA_USER_TOSS:
			case MPA_SLASH_JUMP:
			case MPA_BATTLEPVP_MUGONG_SOMER_SAULT:
			case MPA_BATTLEPVP_HELENA_STEPSHOT:
			case MPA_SUN_OF_GLORY:
			case MPA_HOOKSHOT:
			case MPA_FINAL_TOSS:
			case MPA_NIGHTLORD_SHADOWWEB:
			case MPA_RW_EXPLOSION_CANNON:
			case MPA_UNK2: {
				break;
			}
			case MPA_IMMEDIATE:
			case MPA_TELEPORT:
			case MPA_RANDOM_TELEPORT:
			case MPA_DEMON_TRACE_TELEPORT:
			case MPA_RETURN_TELEPORT:
			case MPA_ASSAULTER:
			case MPA_ASSASSINATION:
			case MPA_RUSH:
			case MPA_SITDOWN:
			case MPA_BLINK_LIGHT:
			case MPA_TELEPORT_ZERO1:
			case MPA_TELEPORT_BY_MOBSKILLAREA:
			case MPA_ZERO_TAG:
			case MPA_RETREAT_SHOT:
			case MPA_UNK1:
			case MPA_PINKBEAN_POGO_STICK:
			case MPA_PINKBEAN_POGO_STICK_END:
			case MPA_PINKBEAN_ROLLING_AIR:
			case MPA_TELEPORT_KINESIS1:
			case MPA_TELEPORT_ARAN1:
			{
				oPacket->Encode2(elem.x);
				oPacket->Encode2(elem.y);
				oPacket->Encode2(elem.fh);
				m_fhLast = elem.fh;
				break;
			}
			case MPA_STARTFALLDOWN:
			case MPA_STARTDRAGDOWN: 
			{
				oPacket->Encode2(elem.vx);
				oPacket->Encode2(elem.vy);
				oPacket->Encode2(elem.fhFootStart);
				break;
			}
			case MPA_FLYING_BLOCK: {
				oPacket->Encode2(elem.x);
				oPacket->Encode2(elem.y);
				oPacket->Encode2(elem.vx);
				oPacket->Encode2(elem.vy);
				break;
			}
			case MPA_STATCHANGE: {
				oPacket->Encode1(elem.bStat);
				break;
			}
		}
		if (elem.nAttr != MPA_STATCHANGE)
		{
			oPacket->Encode1(elem.bMoveAction);
			oPacket->Encode2(elem.tElapse);
			oPacket->Encode1(elem.bForcedStop);
		}
	}
	oPacket->Encode1(0); //Key Pad ?
	oPacket->EncodeBuffer(nullptr, 18); //Dont Know
}
