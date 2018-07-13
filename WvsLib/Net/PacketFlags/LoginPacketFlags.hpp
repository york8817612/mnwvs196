#pragma once
#ifndef LOGIN_FLAG
#define LOGIN_FLAG

#include "PacketFlags.hpp"

#define MAKE_LOGIN_SEND_PACKET_FLAG(flagName, value) namespace FlagInstances{ const static LoginSendPacketFlag flagName {value, #flagName}; } const static int flagName = value
#define MAKE_LOGIN_RECV_PACKET_FLAG(flagName, value) namespace FlagInstances{ const static LoginRecvPacketFlag flagName {value, #flagName}; } const static int flagName = value

MAKE_FLAG_COLLECTION_BODY(LoginSendPacketFlag) 
//�H�U�}�l���U�ʥ]Opcode Flag

//Flags for Center
MAKE_LOGIN_SEND_PACKET_FLAG(Center_RegisterCenterRequest, 1);
MAKE_LOGIN_SEND_PACKET_FLAG(Center_RequestCharacterList, 2);
MAKE_LOGIN_SEND_PACKET_FLAG(Center_RequestCreateNewCharacter, 3);
MAKE_LOGIN_SEND_PACKET_FLAG(Center_RequestGameServerInfo, 4);

//Flags for Game Client
MAKE_LOGIN_SEND_PACKET_FLAG(Client_CheckPasswordResponse, 0x00);
MAKE_LOGIN_SEND_PACKET_FLAG(Client_WorldInformationResponse, 0x01);
MAKE_LOGIN_SEND_PACKET_FLAG(Client_ClientSelectWorldResult, 0x06);
MAKE_LOGIN_SEND_PACKET_FLAG(Client_ClientSelectCharacterResult, 0x07);
MAKE_LOGIN_SEND_PACKET_FLAG(Client_ClientCheckDuplicatedIDResult, 0x0A);
MAKE_LOGIN_SEND_PACKET_FLAG(ClientCreateNewCharacterResult, 0x0B);
MAKE_LOGIN_SEND_PACKET_FLAG(ClientDeleteCharacterResult, 0x0C);
MAKE_LOGIN_SEND_PACKET_FLAG(ClientChangeSPWResult, 0x1A);
MAKE_LOGIN_SEND_PACKET_FLAG(Client_ClientSecondPasswordResult, 0x1B);
MAKE_LOGIN_SEND_PACKET_FLAG(ClientChannelBackgroundResponse, 0x23);
MAKE_LOGIN_SEND_PACKET_FLAG(ClientChooseGender, 0x24);
MAKE_LOGIN_SEND_PACKET_FLAG(ClientGenderSet, 0x25);
MAKE_LOGIN_SEND_PACKET_FLAG(Client_ClientLoginBackgroundResponse, 0x2A);
MAKE_LOGIN_SEND_PACKET_FLAG(Client_ClientStartResponse, 0x2F);
MAKE_LOGIN_SEND_PACKET_FLAG(Client_ClientChannelBackgroundResponse, 0x34);






//����Opcode Flag���U
FLAG_COLLECTION_BODY_END


MAKE_FLAG_COLLECTION_BODY(LoginRecvPacketFlag)
//�H�U�}�l���U�ʥ]Opcode Flag
//Flags for Game Client
MAKE_LOGIN_RECV_PACKET_FLAG(Client_ClientCheckPasswordRequest, 0x69);
MAKE_LOGIN_RECV_PACKET_FLAG(Client_ClientSelectWorld, 0x6A);
MAKE_LOGIN_RECV_PACKET_FLAG(Client_ClientSecondPasswordCheck, 0x6D);
MAKE_LOGIN_RECV_PACKET_FLAG(Client_ClientMigrateIn, 0x6E);
MAKE_LOGIN_RECV_PACKET_FLAG(Client_ClientSelectCharacter, 0x6F);
MAKE_LOGIN_RECV_PACKET_FLAG(Client_ClientRequestServerList, 0x72);
MAKE_LOGIN_RECV_PACKET_FLAG(Client_ClientCheckDuplicatedID, 0x74);
MAKE_LOGIN_RECV_PACKET_FLAG(Client_ClientCreateNewCharactar, 0x7D);
MAKE_LOGIN_RECV_PACKET_FLAG(Client_ClientLoginBackgroundRequest, 0xA0);
MAKE_LOGIN_RECV_PACKET_FLAG(Client_ClientRequestStart, 0xA5);


//����Opcode Flag���U
FLAG_COLLECTION_BODY_END
#endif