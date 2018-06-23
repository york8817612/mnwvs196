#pragma once
#ifndef LOGIN_FLAG
#define LOGIN_FLAG

#include "PacketFlags.hpp"

#define MAKE_LOGIN_PACKET_FLAG(flagName, value) namespace FlagInstances{ const static LoginPacketFlag flagName {value, #flagName}; } const static int flagName = value

MAKE_FLAG_COLLECTION_BODY(LoginPacketFlag) 
//�H�U�}�l���U�ʥ]Opcode Flag

//Flags for Center
MAKE_LOGIN_PACKET_FLAG(RegisterCenterRequest, 1);
MAKE_LOGIN_PACKET_FLAG(RequestCharacterList, 2);
MAKE_LOGIN_PACKET_FLAG(RequestCreateNewCharacter, 3);
MAKE_LOGIN_PACKET_FLAG(RequestGameServerInfo, 4);


//Flags for Client

MAKE_LOGIN_PACKET_FLAG(CheckPasswordResponse, 0x00);
MAKE_LOGIN_PACKET_FLAG(WorldInformationResponse, 0x01);
MAKE_LOGIN_PACKET_FLAG(ClientSelectWorldResult, 0x06);
MAKE_LOGIN_PACKET_FLAG(ClientSelectCharacterResult, 0x07);
MAKE_LOGIN_PACKET_FLAG(ClientCheckDuplicatedIDResult, 0x0A);
MAKE_LOGIN_PACKET_FLAG(ClientCreateNewCharacterResult, 0x0B);
MAKE_LOGIN_PACKET_FLAG(ClientDeleteCharacterResult, 0x0C);
MAKE_LOGIN_PACKET_FLAG(ClientChangeSPWResult, 0x1A);
MAKE_LOGIN_PACKET_FLAG(ClientSecondPasswordResult, 0x1B);
MAKE_LOGIN_PACKET_FLAG(ClientChannelBackgroundResponse, 0x23);
MAKE_LOGIN_PACKET_FLAG(ClientChooseGender, 0x24);
MAKE_LOGIN_PACKET_FLAG(ClientGenderSet, 0x25);
MAKE_LOGIN_PACKET_FLAG(ClientLoginBackgroundResponse, 0x2A);
MAKE_LOGIN_PACKET_FLAG(ClientStartResponse, 0x2F);

//����Opcode Flag���U
FLAG_COLLECTION_BODY_END

#endif