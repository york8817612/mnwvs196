#pragma once
#include "PacketFlags.hpp"

#define MAKE_CENTER_SEND_PACKET_FLAG(flagName, value) namespace FlagInstances{ const static CenterSendPacketFlag flagName {value, #flagName}; } const static int flagName = value

MAKE_FLAG_COLLECTION_BODY(CenterSendPacketFlag)
//�H�U�}�l���U�ʥ]Opcode Flag

MAKE_CENTER_SEND_PACKET_FLAG(RegisterCenterAck, 1);
MAKE_CENTER_SEND_PACKET_FLAG(CenterStatChanged, 2);
MAKE_CENTER_SEND_PACKET_FLAG(CharacterListResponse, 3);
MAKE_CENTER_SEND_PACKET_FLAG(CharacterCreateResponse, 4);
MAKE_CENTER_SEND_PACKET_FLAG(GameServerInfoResponse, 5);
MAKE_CENTER_SEND_PACKET_FLAG(CenterMigrateInResult, 6);
MAKE_CENTER_SEND_PACKET_FLAG(TransferChannelResult, 7);
MAKE_CENTER_SEND_PACKET_FLAG(MigrateCashShopResult, 8);

//����Opcode Flag���U
FLAG_COLLECTION_BODY_END