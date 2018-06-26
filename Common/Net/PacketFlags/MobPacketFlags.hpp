#pragma once
#ifndef MOB_FLAG
#define MOB_FLAG

#include "Net\PacketFlags\PacketFlags.hpp"

#define MAKE_SEND_MOB_PACKET_FLAG(flagName, value) namespace FlagInstances{ const static MobSendPacketFlag flagName {value, #flagName}; } const static int flagName = value
// 以下開始  SendPacket
MAKE_FLAG_COLLECTION_BODY(MobSendPacketFlag)
//以下開始註冊封包Opcode Flag


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