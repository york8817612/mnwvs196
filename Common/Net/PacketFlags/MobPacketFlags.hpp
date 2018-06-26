#pragma once
#ifndef MOB_FLAG
#define MOB_FLAG

#include "Net\PacketFlags\PacketFlags.hpp"

#define MAKE_SEND_MOB_PACKET_FLAG(flagName, value) namespace FlagInstances{ const static MobSendPacketFlag flagName {value, #flagName}; } const static int flagName = value
// �H�U�}�l  SendPacket
MAKE_FLAG_COLLECTION_BODY(MobSendPacketFlag)
//�H�U�}�l���U�ʥ]Opcode Flag


//����Opcode Flag���U
FLAG_COLLECTION_BODY_END

// �H�U�}�l  RecvPacket
#define MAKE_RECV_MOB_PACKET_FLAG(flagName, value) namespace FlagInstances{ const static MobRecvPacketFlag flagName {value, #flagName}; } const static int flagName = value

MAKE_FLAG_COLLECTION_BODY(MobRecvPacketFlag)
//�H�U�}�l���U�ʥ]Opcode Flag
MAKE_RECV_MOB_PACKET_FLAG(minFlag, 0x369);

MAKE_RECV_MOB_PACKET_FLAG(CP_MobMove, 0x369);

MAKE_RECV_MOB_PACKET_FLAG(maxFlag, 0x384);

//����Opcode Flag���U
FLAG_COLLECTION_BODY_END

#endif