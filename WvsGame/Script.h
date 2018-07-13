#pragma once
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>

// Lua is written in C, so compiler needs to know how to link its libraries

#include "..\WvsLib\Script\lapi.h"
#include "..\WvsLib\Script\lua.h"
#include "..\WvsLib\Script\lauxlib.h"
#include "..\WvsLib\Script\lualib.h"
#include "..\WvsLib\Script\luawrapper.hpp"

class User;
class InPacket;
class OutPacket;

class Script
{
	enum ScriptType
	{
		OnSay = 0x00,
		OnSayImage = 0x02,
		OnAskYesNo = 0x03,
		OnAskText = 0x04,
		OnAskNumber = 0x05,
		OnAskMenu = 0x06,
		OnAskQuiz = 0xFF,
		OnAskSpeedQuiz = 0xFF,
		OnAskAvatar = 0x0B,
		OnAskAvatarZero = 0x26,
		OnAskMixHair = 0x2C,
		OnAskMixHairExZero = 0x2D,
		OnAskCustomMixHairAndProb = 0x2F,
		OnAskMixHairNew = 0x30,
		OnAskMixHairNewExZero = 0x30,
		OnAskAndroid = 0x0C,
		OnAskPet = 0x0D,
		OnAskPetAll = 0x0E,
		OnAskActionPetEvolution = 0x0F,
		OnInitialQuiz = 0x08,
		OnInitialSpeedQuiz = 0x09,
		OnICQuiz = 0x0A,
		OnAskAcceptDecline = 0x11,
		OnAskBoxText = 0xFF,
		OnAskSlideMenu = 0x14,
		OnAskSelectMenu = 0x1B,
		OnAskAngelicBuster = 0x1C,
		OnSayIllustration = 0x1D, //1D 1E
		OnAskYesNoIllustration = 0x1F, //0x1F 0x20 0x22 0x23
		OnAskMenuIllustration = 0x21, //0x21, 0x24
		OnAskWeaponBox = 0x28,
		OnAskUserSurvey = 0x2A,
		OnAskScreenShinningStarMsg = 0x33,
		OnAskNumberUseKeyPad = 0x36,
		OnSpinOffGuitarRhythmGame = 0x37,
		OnGhostParkEnter = 0x38,
	};

	// 正服取得
	enum $0B1AAA3469CBCDDB3DA2DA176D1995CB
	{
		SM_SAY = 0x0,
		SM_SAYIMAGE = 0x1,
		SM_ASKYESNO = 0x2,
		SM_ASKTEXT = 0x3,
		SM_ASKNUMBER = 0x4,
		SM_ASKMENU = 0x5,
		SM_ASKQUIZ = 0x6,
		SM_ASKSPEEDQUIZ = 0x7,
		SM_ASKICQUIZ = 0x8,
		SM_ASKAVATAREX = 0x9,
		SM_ASKANDROID = 0xA,
		SM_ASKPET = 0xB,
		SM_ASKPETALL = 0xC,
		SM_ASKACTIONPET_EVOLUTION = 0xD,
		SM_SCRIPT = 0xE,
		SM_ASKACCEPT = 0xF,
		SM_ASKBOXTEXT = 0x10,
		SM_ASKSLIDEMENU = 0x11,
		SM_ASKINGAMEDIRECTION = 0x12,
		SM_PLAYMOVIECLIP = 0x13,
		SM_ASKCENTER = 0x14,
		SM_ASKSELECTMENU = 0x15,
		SM_ASKANGELICBUSTER = 0x16,
		SM_SAY_ILLUSTRATION = 0x17,
		SM_SAY_DUAL_ILLUSTRATION = 0x18,
		SM_ASKYESNO_ILLUSTRATION = 0x19,
		SM_ASKACCEPT_ILLUSTRATION = 0x1A,
		SM_ASKMENU_ILLUSTRATION = 0x1B,
		SM_ASKYESNO_DUAL_ILLUSTRATION = 0x1C,
		SM_ASKACCEPT_DUAL_ILLUSTRATION = 0x1D,
		SM_ASKMENU_DUAL_ILLUSTRATION = 0x1E,
		SM_ASKSSN2 = 0x1F,
		SM_ASKAVATAREXZERO = 0x20,
		SM_MONOLOGUE = 0x21,
		SM_ASK_WEAPONBOX = 0x22,
		SM_ASKBOXTEXT_BGIMG = 0x23,
		SM_ASK_USER_SURVEY = 0x24,
		SM_SUCCESS_CAMERA = 0x25,
		SM_ASKMIXHAIR = 0x26,
		SM_ASKMIXHAIR_EX_ZERO = 0x27,
		SM_ASKCUSTOMMIXHAIR = 0x28,
		SM_ASKCUSTOMMIXHAIR_AND_PROB = 0x29,
		SM_ASKMIXHAIR_NEW = 0x2A,
		SM_ASKMIXHAIR_NEW_EX_ZERO = 0x2B,
		SM_NPCACTION = 0x2C,
		SM_ASK_SCREEN_SHINNING_STAR_MSG = 0x2D,
		SM_INPUT_UI = 0x2E,
		SM_ASKNUMBER_KEYPAD = 0x2F,
		SM_SPINOFF_GUITAR_RHYTHMGAME = 0x30,
		SM_ASK_GHOSTPARK_ENTER_UI = 0x31,
		SM_CAMERA_MSG = 0x32,
		SM_SLIDE_PUZZLE = 0x33,
		SM_DISGUISE = 0x34,
		SM_NEED_CLIENT_RESPONSE = 0x35,
	};

	// 正服取得(不確定哪個封包的，可能是LP_SetInGameDirectionMode)
	enum InGameDirectionAskNumbers
	{
		InGameDirectionAsk_Not = 0x0,
		InGameDirectionAsk_Delay = 0x1,
		InGameDirectionAsk_PatternInputRequest = 0x2,
		InGameDIrectionAsk_CameraMoveTime = 0x3,
	};

	// 正服取得 LP_UserInGameDirectionEvent 
	enum InGameDirectionEventNumbers
	{
		InGameDirectionEvent_ForcedAction = 0x0,
		InGameDirectionEvent_Delay = 0x1,
		InGameDirectionEvent_EffectPlay = 0x2,
		InGameDirectionEvent_ForcedInput = 0x3,
		InGameDirectionEvent_PatternInputRequest = 0x4,
		InGameDirectionEvent_CameraMove = 0x5,
		InGameDirectionEvent_CameraOnCharacter = 0x6,
		InGameDirectionEvent_CameraZoom = 0x7,
		InGameDirectionEvent_CameraReleaseFromUserPoint = 0x8,
		InGameDirectionEvent_VansheeMode = 0x9,
		InGameDirectionEvent_FaceOff = 0xA,
		InGameDirectionEvent_Monologue = 0xB,
		InGameDirectionEvent_MonologueScroll = 0xC,
		InGameDirectionEvent_AvatarLookSet = 0xD,
		InGameDirectionEvent_RemoveAdditionalEffect = 0xE,
		InGameDirectionEvent_ForcedMove = 0xF,
		InGameDirectionEvent_ForcedFlip = 0x10,
		InGameDirectionEvent_InputUI = 0x11,
	};

	friend class ScriptMan;

	lua_State* L;
	//std::unique_ptr<lua_State, void(*)(lua_State*)> L;
	std::mutex m_mtxWaitLock;
	std::condition_variable m_cndVariable, m_doneVariable;

	static int SelfSayNextGroup(lua_State* L);
	static int SelfSay(lua_State* L);
	static int SelfAskAvatar(lua_State* L);
	static int SelfSayNext(lua_State* L);
	static int SelfAskText(lua_State* L);
	static int SelfAskNumber(lua_State* L);
	static int SelfAskYesNo(lua_State* L);
	static int SelfAskMenu(lua_State* L);
	static int SelfPushArray(lua_State* L);
	static Script* GetSelf(lua_State* L);

	static luaL_Reg SelfTable[];
	static luaL_Reg SelfMetatable[];

	int m_nID, m_nUserInput;
	std::string m_fileName, m_strUserInput;
	User *m_pUser;
	std::thread* m_pThread;
	std::vector<int> m_aArrayObj;
	bool m_bDone = false;

public:
	void Wait();
	void Notify();
	void Run();
	void Abort();
	bool IsDone();

	void OnPacket(InPacket *iPacket);

	Script(const std::string& file, int nNpcID);
	~Script();

	static int LuaRegisterSelf(lua_State* L);

	void SetUser(User *pUser);

	std::thread* GetThread();
	void SetThread(std::thread* pThread);
};