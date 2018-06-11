#pragma once
#ifndef MOB_FLAG
#define MOB_FLAG

#include "Net\PacketFlags\PacketFlags.hpp"

#define MAKE_SEND_MOB_PACKET_FLAG(flagName, value) namespace FlagInstances{ const static MobSendPacketFlag flagName {value, #flagName}; } const static int flagName = value
// 以下開始  SendPacket
MAKE_FLAG_COLLECTION_BODY(MobSendPacketFlag)
//以下開始註冊封包Opcode Flag

MAKE_SEND_MOB_PACKET_FLAG(SP_MobEnterField, 0x3C1);
MAKE_SEND_MOB_PACKET_FLAG(SP_MobLeaveField, 0x3C2);
MAKE_SEND_MOB_PACKET_FLAG(SP_MobChangeController, 0x3C3);
MAKE_SEND_MOB_PACKET_FLAG(SP_MobSetAfterAttack, 0x3C4);
MAKE_SEND_MOB_PACKET_FLAG(SP_MobBlockAttack, 0x3C5);
MAKE_SEND_MOB_PACKET_FLAG(SP_UrusOnlyMobSkill, 0x3C6);
MAKE_SEND_MOB_PACKET_FLAG(SP_MobMove, 0x3C7);
MAKE_SEND_MOB_PACKET_FLAG(SP_MobCtrlAck, 0x3C8);
MAKE_SEND_MOB_PACKET_FLAG(SP_MobCtrlHint, 0x3C9);
MAKE_SEND_MOB_PACKET_FLAG(SP_MobStatSet, 0x3CA);
MAKE_SEND_MOB_PACKET_FLAG(SP_MobStatReset, 0x3CB);
MAKE_SEND_MOB_PACKET_FLAG(SP_MobSuspendReset, 0x3CC);
MAKE_SEND_MOB_PACKET_FLAG(SP_MobAffected, 0x3CD);
MAKE_SEND_MOB_PACKET_FLAG(SP_MobDamaged, 0x3CE);
// 0x3CF
// 0x3D0
MAKE_SEND_MOB_PACKET_FLAG(SP_MobSpecialEffectBySkill, 0x3D1);
MAKE_SEND_MOB_PACKET_FLAG(SP_MobHPChange, 0x3D2);
MAKE_SEND_MOB_PACKET_FLAG(SP_MobCrcKeyChanged, 0x3D3);
MAKE_SEND_MOB_PACKET_FLAG(SP_MobCrcDataRequest, 0x3D4);
MAKE_SEND_MOB_PACKET_FLAG(SP_MobHPIndicator, 0x3D5);
MAKE_SEND_MOB_PACKET_FLAG(SP_MobCatchEffect, 0x3D6);

//結束Opcode Flag註冊
FLAG_COLLECTION_BODY_END

// 以下開始  RecvPacket
#define MAKE_RECV_MOB_PACKET_FLAG(flagName, value) namespace FlagInstances{ const static MobRecvPacketFlag flagName {value, #flagName}; } const static int flagName = value

MAKE_FLAG_COLLECTION_BODY(MobRecvPacketFlag)
//以下開始註冊封包Opcode Flag
MAKE_RECV_MOB_PACKET_FLAG(minFlag, 0x369);

MAKE_RECV_MOB_PACKET_FLAG(CP_MobMove, 0x369);

MAKE_RECV_MOB_PACKET_FLAG(maxFlag, 0x384);

//結束Opcode Flag註冊
FLAG_COLLECTION_BODY_END

#endif