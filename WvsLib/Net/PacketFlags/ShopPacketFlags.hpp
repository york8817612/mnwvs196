#pragma once
#ifndef SHOP_FLAG
#define SHOP_FLAG

#include "PacketFlags.hpp"

#define MAKE_SHOP_PACKET_FLAG(flagName, value) namespace FlagInstances{ const static ShopSendPacketFlag flagName {value, #flagName}; } const static int flagName = value

MAKE_FLAG_COLLECTION_BODY(ShopSendPacketFlag)
//�H�U�}�l���U�ʥ]Opcode Flag

//Flags to Center
MAKE_SHOP_PACKET_FLAG(RequestMigrateIn, 0x8001);
MAKE_SHOP_PACKET_FLAG(RequestTransferToGame, 0x8002);
MAKE_SHOP_PACKET_FLAG(RequestMigrateOut, 0x8003);


//Flags to Game
MAKE_SHOP_PACKET_FLAG(Client_SetCashShop, 0x1C4); //Stage::OnSetCashShop

//����Opcode Flag���U
FLAG_COLLECTION_BODY_END

#endif