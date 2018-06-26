#pragma once
#ifndef USER_FLAG
#define USER_FLAG

#include "PacketFlags.hpp"

#define MAKE_USER_PACKET_FLAG(flagName, value) namespace FlagInstances{ const static UserPacketFlag flagName {value, #flagName}; } const static int flagName = value

MAKE_FLAG_COLLECTION_BODY(UserPacketFlag)
//以下開始註冊封包Opcode Flag

MAKE_USER_PACKET_FLAG(UserLocal_OnStatChanged, 0x47);
MAKE_USER_PACKET_FLAG(USerLocal_OnTemporaryStatSet, 0x48);
MAKE_USER_PACKET_FLAG(USerLocal_OnTemporaryStatReset, 0x49);
MAKE_USER_PACKET_FLAG(UserCommon_OnChat, 0x21E);

MAKE_USER_PACKET_FLAG(SP_UserMeleeAttack, 0x296);
MAKE_USER_PACKET_FLAG(SP_UserShootAttack, 0x297);
MAKE_USER_PACKET_FLAG(SP_UserMagicAttack, 0x298);
MAKE_USER_PACKET_FLAG(SP_UserBodyAttack, 0x299);
MAKE_USER_PACKET_FLAG(SP_UserSkillPrepare, 0x29A);
MAKE_USER_PACKET_FLAG(SP_UserMovingShootAttackPrepare, 0x29B);
MAKE_USER_PACKET_FLAG(SP_UserSkillCancel, 0x29C);
MAKE_USER_PACKET_FLAG(SP_UserHit, 0x29D);
MAKE_USER_PACKET_FLAG(SP_UserEmotion, 0x29E);
MAKE_USER_PACKET_FLAG(SP_AndroidEmotion, 0x29F);
MAKE_USER_PACKET_FLAG(SP_UserSetActiveEffectItem, 0x2A0);
MAKE_USER_PACKET_FLAG(SP_UserSetActiveNickItem, 0x2A1);
MAKE_USER_PACKET_FLAG(SP_UserSetDefaultWingItem, 0x2A2);
MAKE_USER_PACKET_FLAG(SP_UserSetKaiserTransformItem, 0x2A3);
MAKE_USER_PACKET_FLAG(SP_UserSetCustomRiding, 0x2A4);
MAKE_USER_PACKET_FLAG(SP_UserShowUpgradeTombEffect, 0x2A5);
MAKE_USER_PACKET_FLAG(SP_UserSetActivePortableChair, 0x2A6);
MAKE_USER_PACKET_FLAG(SP_UserAvatarModified, 0x2A7);
MAKE_USER_PACKET_FLAG(SP_UserEffectRemote, 0x2A8);
MAKE_USER_PACKET_FLAG(SP_UserTemporaryStatSet, 0x2A9);
MAKE_USER_PACKET_FLAG(SP_UserTemporaryStatReset, 0x2AA);
MAKE_USER_PACKET_FLAG(SP_UserReceiveHP, 0x2AB);
MAKE_USER_PACKET_FLAG(SP_UserGuildNameChanged, 0x2AC);
MAKE_USER_PACKET_FLAG(SP_UserGuildMarkChanged, 0x2AD);
MAKE_USER_PACKET_FLAG(SP_UserPvPTeamChanged, 0x2AE);
MAKE_USER_PACKET_FLAG(SP_UserGatherActionSet, 0x2AF);
MAKE_USER_PACKET_FLAG(SP_UserUpdatePvPHPTag, 0x2B0);
MAKE_USER_PACKET_FLAG(SP_UserDragonGlide, 0x2B1);
MAKE_USER_PACKET_FLAG(SP_UserKeyDownAreaMovePath, 0x2B2);
MAKE_USER_PACKET_FLAG(SP_UserLaserInfoForRemote, 0x2B3);
MAKE_USER_PACKET_FLAG(SP_UserKaiserColorOrMorphChange, 0x2B4);
MAKE_USER_PACKET_FLAG(SP_UserDestroyGrenade, 0x2B5);
MAKE_USER_PACKET_FLAG(SP_UserSetItemAction, 0x2B6);
MAKE_USER_PACKET_FLAG(SP_UserZeroTag, 0x2B7);
MAKE_USER_PACKET_FLAG(SP_UserIntrusion, 0x2B8);
MAKE_USER_PACKET_FLAG(SP_UserZeroLastAssistState, 0x2B9);
MAKE_USER_PACKET_FLAG(SP_UserSetMoveGrenade, 0x2BA);
MAKE_USER_PACKET_FLAG(SP_UserSetCustomizeEffect, 0x2BB);
MAKE_USER_PACKET_FLAG(SP_UserRuneStoneAction, 0x2BC);
MAKE_USER_PACKET_FLAG(SP_UserKinesisPsychicEnergyShieldEffect, 0x2BD);
MAKE_USER_PACKET_FLAG(SP_UserDragonAction, 0x2BE);
MAKE_USER_PACKET_FLAG(SP_UserDragonBreathEarthEffect, 0x2BF);
MAKE_USER_PACKET_FLAG(SP_UserReleaseRWGrab, 0x2C0);
MAKE_USER_PACKET_FLAG(SP_UserRWMultiChargeCancelRequest, 0x2C1);
MAKE_USER_PACKET_FLAG(SP_UserScouterMaxDamageUpdate, 0x2C2);
MAKE_USER_PACKET_FLAG(SP_UserStigmaDeliveryResponse, 0x2C3);

MAKE_USER_PACKET_FLAG(SP_UserThrowGrenade, 0x2CC);
//結束Opcode Flag註冊
FLAG_COLLECTION_BODY_END

#endif