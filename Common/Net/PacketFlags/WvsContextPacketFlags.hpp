#pragma once
#ifndef WVS_CONTEXT_FLAG
#define WVS_CONTEXT_FLAG

#include "PacketFlags.hpp"

#define MAKE_WVS_CONTEXT_PACKET_FLAG(flagName, value) namespace FlagInstances{ const static WvsContextPacketFlag flagName {value, #flagName}; } const static int flagName = value

MAKE_FLAG_COLLECTION_BODY(WvsContextPacketFlag)
//以下開始註冊封包Opcode Flag

MAKE_WVS_CONTEXT_PACKET_FLAG(SP_UpdateMapleTVShowTime, 0x148);
MAKE_WVS_CONTEXT_PACKET_FLAG(SP_ReturnToTitle, 0x149);
MAKE_WVS_CONTEXT_PACKET_FLAG(SP_ReturnToCharacterSelect, 0x14A);
MAKE_WVS_CONTEXT_PACKET_FLAG(SP_FlameWizardFlameWalkEffect, 0x14B);
MAKE_WVS_CONTEXT_PACKET_FLAG(SP_FlameWizardFlareBlink, 0x14C);
MAKE_WVS_CONTEXT_PACKET_FLAG(SP_SummonedAvatarSync, 0x14D);
MAKE_WVS_CONTEXT_PACKET_FLAG(SP_CashShopEventInfo, 0x14E);
MAKE_WVS_CONTEXT_PACKET_FLAG(SP_BlackList, 0x14F);

//結束Opcode Flag註冊
FLAG_COLLECTION_BODY_END

#endif