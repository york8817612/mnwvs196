#include "GW_Avatar.hpp"
#include "WvsUnified.h"

#include "..\Common\Net\OutPacket.h"
#include "..\WvsGame\WvsGameConstants.hpp"

GW_Avatar::GW_Avatar()
{
}

GW_Avatar::~GW_Avatar()
{
}

void GW_Avatar::Load(GA_Character *vCharacter)
{
	mCharacter = vCharacter;
	mStat = mCharacter->mStat;
	nHair = mStat->nHair;
	nFace = mStat->nFace;
	nSkin = mStat->nSkin;

	//EQUIP
	Poco::Data::Statement queryStatement(GET_DB_SESSION);
	queryStatement << "SELECT SN FROM ItemSlot_EQP Where CharacterID = " << mCharacter->nCharacterID;
	queryStatement.execute();
	Poco::Data::RecordSet recordSet(queryStatement);
	GW_ItemSlotEquip eqp;
	for (int i = 0; i < recordSet.rowCount(); ++i, recordSet.moveNext())
	{
		eqp.Load(recordSet["SN"]);
		short nPos = eqp.nPOS * -1;
		if (nPos < 100)
			aHairEquip.push_back(eqp);
		else if (nPos > 100 && nPos != 111)
			aUnseenEquip.push_back(eqp);
		//There should insert totem items.
	}
}

void GW_Avatar::Save(GA_Character *vCharacter, bool newCharacter)
{
	mCharacter = vCharacter;
	mStat = mCharacter->mStat;

	mStat->nHair = nHair >= 0 ? nHair : mStat->nHair;
	mStat->nFace = nFace >= 0 ? nFace : mStat->nFace;
	mStat->nSkin = nSkin >= 0 ? nSkin : mStat->nSkin;
}

void GW_Avatar::Encode(OutPacket *oPacket)
{
	oPacket->Encode1(mCharacter->nGender);
	oPacket->Encode1(mStat->nSkin);
	oPacket->Encode4(mStat->nFace);
	oPacket->Encode4(mStat->nJob);
	oPacket->Encode1(0); //Mega?
	oPacket->Encode4(mStat->nHair);

	for (const auto& eqp : aHairEquip)
	{
		oPacket->Encode1(eqp.nPOS * -1);
		oPacket->Encode4(eqp.nItemID);
	}
	oPacket->Encode1((char)0xFF);
	for (const auto& eqp : aUnseenEquip)
	{
		oPacket->Encode1((eqp.nPOS * -1) - 100);
		oPacket->Encode4(eqp.nItemID);
	}
	oPacket->Encode1((char)0xFF);
	oPacket->Encode1((char)0xFF); //totem

	int cWeaponIdx = -1, weaponIdx = -1, nShieldIdx = -1;
	for (int i = 0; i < aHairEquip.size(); ++i)
		if (aHairEquip[i].nPOS == -111)
			cWeaponIdx = i;
		else if (aHairEquip[i].nPOS == -11)
			weaponIdx = i;
		else if (aHairEquip[i].nPOS == -10)
			nShieldIdx = i;

		oPacket->Encode4(cWeaponIdx == -1 ? 0 : aHairEquip[cWeaponIdx].nItemID);
		oPacket->Encode4(weaponIdx == -1 ? 0 : aHairEquip[weaponIdx].nItemID);
		oPacket->Encode4(nShieldIdx == -1 ? 0 : aHairEquip[nShieldIdx].nItemID);

		oPacket->Encode1(0);
		oPacket->Encode4(0);
		oPacket->Encode4(0);
		oPacket->Encode4(0);

		if (WvsGameConstants::IsDslayerJobBorn(mStat->nJob)
			|| WvsGameConstants::IsResHybridJob(mStat->nJob)
			|| WvsGameConstants::IsBeastTamerJob(mStat->nJob))
		{
			oPacket->Encode4(mCharacter->nDefFaceAcc);
		}
		if (WvsGameConstants::IsZeroJob(mStat->nJob))
		{
			oPacket->Encode1(1);
		}
		if (WvsGameConstants::IsBeastTamerJob(mStat->nJob))
		{
			oPacket->Encode1(1);
			oPacket->Encode4(5010116);
			oPacket->Encode1(1);
			oPacket->Encode4(5010119);
		}

		oPacket->Encode1(mCharacter->nMixedHairColor);
		oPacket->Encode1(mCharacter->nMixHairPercent);
		oPacket->Encode1(0);
		oPacket->Encode4(0);
}

void GW_Avatar::Decode(InPacket * iPacket)
{
	iPacket->Decode1(); //nGender, Gender shouldn't change
	nSkin = iPacket->Decode1();
	nFace = iPacket->Decode4();
	iPacket->Decode4(); //nJob decoded at DecodeStat
	iPacket->Decode1(); //unk
	nHair = iPacket->Decode4();

	char pos = 0;
	while (1) //aHairEquip
	{
		pos = iPacket->Decode1();
		if (pos == 0xFF)
			break;
		iPacket->Decode4(); //ItemID
	}

	while (1) //aUnseenEquip
	{
		pos = iPacket->Decode1();
		if (pos == 0xFF)
			break;
		iPacket->Decode4(); //ItemID
	}

	while (1) //Unk
	{
		pos = iPacket->Decode1();
		if (pos == 0xFF)
			break;
		iPacket->Decode4(); //ItemID
	}

	while (1) //Totem
	{
		pos = iPacket->Decode1();
		if (pos == 0xFF)
			break;
		iPacket->Decode4(); //ItemID
	}

	iPacket->Decode4(); //cWeaponIdx
	iPacket->Decode4(); //?
	iPacket->Decode4(); //nSheldIdx

	iPacket->Decode1();
	iPacket->Decode4();
	iPacket->Decode4();
	iPacket->Decode4();

	iPacket->Decode1();
	iPacket->Decode1();
	iPacket->Decode1();
	iPacket->Decode4();
}
