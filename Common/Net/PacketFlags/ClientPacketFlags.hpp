#pragma once
#ifndef CLIENT_FLAG
#define CLIENT_FLAG

#include "PacketFlags.hpp"

#define MAKE_CLIENT_PACKET_FLAG(flagName, value) namespace FlagInstances{ const static ClientPacketFlag flagName {value, #flagName}; } const static int flagName = value

MAKE_FLAG_COLLECTION_BODY(ClientPacketFlag)
//以下開始註冊封包Opcode Flag

MAKE_CLIENT_PACKET_FLAG(OnUserSkillUp, 0x4C);
MAKE_CLIENT_PACKET_FLAG(OnMessage, 0x57);

MAKE_CLIENT_PACKET_FLAG(CP_CheckPasswordRequest, 0x69);
MAKE_CLIENT_PACKET_FLAG(CP_SelectWorld, 0x6A);
MAKE_CLIENT_PACKET_FLAG(CP_SecondPasswordCheck, 0x6D);
MAKE_CLIENT_PACKET_FLAG(CP_MigrateIn, 0x6E);
MAKE_CLIENT_PACKET_FLAG(CP_SelectCharacter, 0x6F);
MAKE_CLIENT_PACKET_FLAG(CP_RequestServerList, 0x72);
MAKE_CLIENT_PACKET_FLAG(CP_CheckDuplicatedID, 0x74);
MAKE_CLIENT_PACKET_FLAG(CP_CreateNewCharactar, 0x7D);
MAKE_CLIENT_PACKET_FLAG(CP_CreateNewCharacterInCS, 0x7E);
MAKE_CLIENT_PACKET_FLAG(CP_CreateNewCharacter_PremiumAdventurer, 0x7F);
MAKE_CLIENT_PACKET_FLAG(CP_DeleteCharacter, 0x80);
MAKE_CLIENT_PACKET_FLAG(CP_ExceptionLog, 0x85);
MAKE_CLIENT_PACKET_FLAG(CP_ClientDumpLog, 0x96);
MAKE_CLIENT_PACKET_FLAG(CP_SetGender, 0x9D);
MAKE_CLIENT_PACKET_FLAG(CP_LoginBackgroundRequest, 0xA0);
MAKE_CLIENT_PACKET_FLAG(CP_RequestStart, 0xA5);
MAKE_CLIENT_PACKET_FLAG(CP_CheckSPWOnCreateNewCharacter, 0xA9);
MAKE_CLIENT_PACKET_FLAG(CP_DirectGoToField, 0xAB);
MAKE_CLIENT_PACKET_FLAG(CP_UpdateCharacterSelectList, 0xAC);

MAKE_CLIENT_PACKET_FLAG(ClientCharacterBurning, 0xB0);

MAKE_CLIENT_PACKET_FLAG(OnUserTransferFieldRequest, 0xB3);
MAKE_CLIENT_PACKET_FLAG(OnUserMoveRequest, 0xC4);
MAKE_CLIENT_PACKET_FLAG(CP_UserSitRequest, 0xC5);
MAKE_CLIENT_PACKET_FLAG(CP_UserPortableChairSitRequest, 0xC6);

MAKE_CLIENT_PACKET_FLAG(OnUserAttack_MeleeAttack, 0xC9);
MAKE_CLIENT_PACKET_FLAG(OnUserAttack_ShootAttack, 0xCA);
MAKE_CLIENT_PACKET_FLAG(OnUserAttack_MagicAttack, 0xCB);
MAKE_CLIENT_PACKET_FLAG(OnUserAttack_BodyAttack, 0xCC);
MAKE_CLIENT_PACKET_FLAG(OnUserAttack_AreaDot, 0xCD);
MAKE_CLIENT_PACKET_FLAG(CP_UserMovingShootAttackPrepare, 0xCE);
MAKE_CLIENT_PACKET_FLAG(CP_UserHit, 0xCF);

MAKE_CLIENT_PACKET_FLAG(OnUserChat, 0xD1);
MAKE_CLIENT_PACKET_FLAG(CP_UserActivateNickItem, 0xD7);

MAKE_CLIENT_PACKET_FLAG(OnSelectNpc, 0xE3);
MAKE_CLIENT_PACKET_FLAG(OnScriptMessageAnswer, 0xE5);
MAKE_CLIENT_PACKET_FLAG(CP_UserShopRequest, 0xE6);
MAKE_CLIENT_PACKET_FLAG(CP_UserTrunkRequest, 0xE7);
MAKE_CLIENT_PACKET_FLAG(CP_UserEntrustedShopRequest, 0xE8);
MAKE_CLIENT_PACKET_FLAG(CP_UserStoreBankRequest, 0xE9);

MAKE_CLIENT_PACKET_FLAG(CP_UserGatherItemRequest, 0xF5);
MAKE_CLIENT_PACKET_FLAG(CP_UserSortItemRequest, 0xF6);
MAKE_CLIENT_PACKET_FLAG(OnUserChangeSlotRequest, 0xF7);

MAKE_CLIENT_PACKET_FLAG(CP_UserStatChangeItemUseRequest, 0xFC);

MAKE_CLIENT_PACKET_FLAG(CP_UserAbilityUpRequest, 0x13A);
MAKE_CLIENT_PACKET_FLAG(CP_UserAbilityMassUpRequest, 0x13B);

MAKE_CLIENT_PACKET_FLAG(CP_UserDotHeal, 0x13C);
MAKE_CLIENT_PACKET_FLAG(CP_UserChangeStatRequest, 0x13D);
MAKE_CLIENT_PACKET_FLAG(OnUserSkillUpRequest, 0x13E);
MAKE_CLIENT_PACKET_FLAG(OnUserSkillUseRequest, 0x13F);
MAKE_CLIENT_PACKET_FLAG(OnUserSkillCancelRequest, 0x140);
MAKE_CLIENT_PACKET_FLAG(CP_UserSkillPrepareRequest, 0x141);

MAKE_CLIENT_PACKET_FLAG(CP_UserDropMoneyRequest, 0x144);
MAKE_CLIENT_PACKET_FLAG(CP_UserGivePopularityRequest, 0x145);

MAKE_CLIENT_PACKET_FLAG(CP_UserCharacterInfoRequest, 0x147);
MAKE_CLIENT_PACKET_FLAG(CP_UserActivatePetRequest, 0x148);
MAKE_CLIENT_PACKET_FLAG(CP_UserRegisterPetAutoBuffRequest, 0x149);
MAKE_CLIENT_PACKET_FLAG(CP_UserTemporaryStatUpdateRequest, 0x14A);
MAKE_CLIENT_PACKET_FLAG(CP_UserPortalScriptRequest, 0x14B);

MAKE_CLIENT_PACKET_FLAG(OnQuestRequest, 0x154);

MAKE_CLIENT_PACKET_FLAG(CP_FuncKeyMappedModified, 0x1B5);

MAKE_CLIENT_PACKET_FLAG(OnChangeCharacterRequest, 0x204);

//結束Opcode Flag註冊
FLAG_COLLECTION_BODY_END

#endif