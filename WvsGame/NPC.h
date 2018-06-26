#pragma once
#include "FieldObj.h"

class User;
class InPacket;

class Npc : public FieldObj
{
public:
	Npc();
	~Npc();

	enum ShopRes
	{
		ShopReq_Buy = 0x0,
		ShopReq_Sell = 0x1,
		ShopReq_Recharge = 0x2,
		ShopReq_Close = 0x3,
		ShopReq_StarCoinRes = 0x4,
		ShopRes_BuySuccess = 0x0,
		ShopRes_BuyNoStock = 0x1,
		ShopRes_BuyNoMoney = 0x2,
		ShopRes_BuyNoPoint = 0x3,
		ShopRes_BuyNoFloor = 0x4,
		ShopRes_BuyNoQuestEx = 0x5,
		ShopRes_BuyInvalidTime = 0x6,
		ShopRes_BuyUnknown = 0x7,
		ShopRes_SellSuccess = 0x8,
		ShopRes_SellNoStock = 0x9,
		ShopRes_SellIncorrectRequest = 0xA,
		ShopRes_SellOverflow = 0xB,
		ShopRes_SellLimitPriceAtOnetime = 0xC,
		ShopRes_SellUnkonwn = 0xD,
		ShopRes_RechargeSuccess = 0xE,
		ShopRes_RechargeNoStock = 0xF,
		ShopRes_RechargeNoMoney = 0x10,
		ShopRes_RechargeIncorrectRequest = 0x11,
		ShopRes_RechargeUnknown = 0x12,
		ShopRes_BuyNoToken = 0x13,
		ShopRes_BuyNoStarCoin = 0x14,
		ShopRes_LimitLevel_Less = 0x15,
		ShopRes_LimitLevel_More = 0x16,
		ShopRes_CantBuyAnymore = 0x17,
		ShopRes_FailedByBuyLimit = 0x18,
		ShopRes_TradeBlocked = 0x19,
		ShopRes_NpcRandomShopReset = 0x1A,
		ShopRes_BuyStockOver = 0x1B,
		ShopRes_DisabledNPC = 0x1C,
		ShopRes_TradeBlockedNotActiveAccount = 0x1D,
		ShopRes_TradeBlockedSnapShot = 0x1E,
		ShopRes_MarketTempBlock = 0x1F,
		ShopRes_UnalbeWorld = 0x20,
		ShopRes_UnalbeShopVersion = 0x21,
	};

	void OnUpdateLimitedInfo(User* pUser, InPacket *iPacket);
	void SendChangeControllerPacket(User* pUser);
	void MakeEnterFieldPacket(OutPacket *oPacket);
	void MakeLeaveFieldPacket(OutPacket *oPacket);
	void EncodeInitData(OutPacket *oPacket);
};

