#pragma once
#include "Net\SocketBase.h"

class LoginSocket :
	public SocketBase
{
private:
	struct LoginData
	{
		//TODO: AccountId ³]©w
		int nAccountID = 1;
		std::string strAccountName;
		unsigned char nLoginState;
	} mLoginData;

	void OnClosed();
	int nWorldID, nChannelID;

public:
	LoginSocket(asio::io_service& serverService);
	~LoginSocket();

	enum LoginResCode
	{
		LoginResCode_ProcFail = 0xFFFFFFFF,
		LoginResCode_Success = 0x0,
		LoginResCode_TempBlocked = 0x1,
		LoginResCode_Blocked = 0x2,
		LoginResCode_Abandoned = 0x3,
		LoginResCode_IncorrectPassword = 0x4,
		LoginResCode_NotRegistered = 0x5,
		LoginResCode_DBFail = 0x6,
		LoginResCode_AlreadyConnected = 0x7,
		LoginResCode_NotConnectableWorld = 0x8,
		LoginResCode_Unknown = 0x9,
		LoginResCode_Timeout = 0xA,
		LoginResCode_NotAdult = 0xB,
		LoginResCode_AuthFail = 0xC,
		LoginResCode_ImpossibleIP = 0xD,
		LoginResCode_NotAuthorizedNexonID = 0xE,
		LoginResCode_NoNexonID = 0xF,
		LoginResCode_IncorrectSSN2 = 0x10,
		LoginResCode_WebAuthNeeded = 0x11,
		LoginResCode_DeleteCharacterFailedOnGuildMaster = 0x12,
		LoginResCode_TempBlockedIP = 0x13,
		LoginResCode_IncorrectSPW = 0x14,
		LoginResCode_DeleteCharacterFailedEngaged = 0x15,
		LoginResCode_SamePasswordAndSPW = 0x16,
		LoginResCode_WaitOTP = 0x17,
		LoginResCode_WrongOTP = 0x18,
		LoginResCode_OverCountErrOTP = 0x19,
		LoginResCode_SystemErr = 0x1A,
		LoginResCode_CancelInputDeleteCharacterOTP = 0x1B,
		LoginResCode_PaymentWarning = 0x1C,
		LoginResCode_DeleteCharacterFailedOnFamily = 0x1D,
		LoginResCode_InvalidCharacterName = 0x1E,
		LoginResCode_IncorrectSSN = 0x1F,
		LoginResCode_SSNConfirmFailed = 0x20,
		LoginResCode_SSNNotConfirmed = 0x21,
		LoginResCode_WorldTooBusy = 0x22,
		LoginResCode_OTPReissuing = 0x23,
		LoginResCode_OTPInfoNotExist = 0x24,
		LoginResCode_Shutdowned = 0x25,
		LoginResCode_DeleteCharacterFailedHasEntrustedShop = 0x26,
		LoginResCode_AlbaPerform = 0x27,
		LoginResCode_TransferredToNxEmailID = 0x28,
		LoginResCode_UntransferredToNxEmailID = 0x29,
		LoginResCode_RequestedMapleIDAlreadyInUse = 0x2A,
		LoginResCode_WaitSelectAccount = 0x2B,
		LoginResCode_DeleteCharacterFailedProtectedItem = 0x2C,
		LoginResCode_UnauthorizedUser = 0x2D,
		LoginResCode_CannotCreateMoreMapleAccount = 0x2E,
		LoginResCode_CreateBanned = 0x2F,
		LoginResCode_CreateTemporarilyBanned = 0x30,
		LoginResCode_EventNewCharacterExpireFail = 0x31,
		LoginResCode_SelectiveShutdowned = 0x32,
		LoginResCode_NonownerRequest = 0x33,
		LoginResCode_OTPRequired = 0x34,
		LoginResCode_GuestServiceClosed = 0x35,
		LoginResCode_BlockedNexonID = 0x36,
		LoginResCode_DupMachineID = 0x37,
		LoginResCode_NotActiveAccount = 0x38,
		LoginResCode_IncorrectSPW4th = 0x39,
		LoginResCode_IncorrectSPW5th = 0x3A,
		LoginResCode_InsufficientSPW = 0x3B,
		LoginResCode_SameCharSPW = 0x3C,
		LoginResCode_WebLaunchingOTPRequired = 0x3D,
		LoginResCode_MergeWorld_CreateCharacterBanned = 0x3E,
		LoginResCode_ChangeNewOTP = 0x3F,
		LoginResCode_BlockedByServiceArea = 0x40,
		LoginResCode_ExceedReservedDeleteCharacter = 0x41,
		LoginResCode_UnionFieldChannelClosed = 0x43,
		LoginResCode_ProtectAccount = 0x44,
		LoginResCode_AntiMacroReq = 0x45,
		LoginResCode_AntiMacroCreateFailed = 0x46,
		LoginResCode_AntiMacroIncorrect = 0x47,
		LoginResCode_LimitCreateCharacter = 0x48,
		LoginResCode_ProtectSSOLogin = 0x49,
		LoginResCode_InvalidMapleIDThroughMobile = 0x4A,
		LoginResCode_InvalidPasswordThroughMobile = 0x4B,
		LoginResCode_HashedPasswordIsEmpty = 0x4C,
		LoginResCode_NGS_For_Ass = 0x4D,
		LoginResCode_AlreadyConnectedThroughMobile = 0x4E,
		LoginResCode_Protected_For_Ass = 0x4F,
		LoginResCode_Blocked_For_Ass = 0x50,
		LoginResCode_WrongVer = 0x51,
		LoginResCode_EMailVerify = 0x52,
		LoginResCode_DenyJob = 0x53,
		LoginResCode_InvalidObject = 0x54,
		LoginResCode_IncorrectLoginType_OtherToMapleID = 0x55,
		LoginResCode_FailedUserCreate = 0x56,
		LoginResCode_MobileTokenInvalid = 0x57,
		LoginResCode_MobileTokenDeviceIDInvalid = 0x58,
		LoginResCode_MobileTokenExpired = 0x59,
		LoginResCode_NotHaveNaverID = 0x5A,
		LoginResCode_UserTossAIPlayer = 0x5B,
		LoginResCode_InactivateMember = 0x5C,
	};


	void OnPacket(InPacket *iPacket);

	void OnClientRequestStart();
	void OnLoginBackgroundRequest();
	void OnCheckPasswordRequst(InPacket *iPacket);
	void SendChannelBackground();
	void SendWorldInformation();
	void OnClientSelectWorld(InPacket *iPacket);
	void OnClientSecondPasswdCheck();
	void OnClientCheckDuplicatedID(InPacket *iPacket);
	void OnClientCreateNewCharacter(InPacket *iPacket);
	void OnClientSelectCharacter(InPacket *iPacket);
};

