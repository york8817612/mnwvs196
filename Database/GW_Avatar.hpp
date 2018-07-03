#pragma once
#include <vector>
#include "GW_ItemSlotEquip.h"
#include "GA_Character.hpp"
#include "GW_CharacterStat.h"

class OutPacket;
class InPacket;

struct GW_Avatar
{
	int nHair, nFace, nSkin;
	std::vector<GW_ItemSlotEquip> aHairEquip, aUnseenEquip, aTotemEquip;
	GA_Character* mCharacter;
	GW_CharacterStat* mStat;

public:
	GW_Avatar();
	~GW_Avatar();

	void Load(GA_Character *vCharacter);
	void Save(GA_Character *vCharacter, bool newCharacter = false);

	void Encode(GA_Character *vCharacter, OutPacket *oPacket);
	void Decode(InPacket* iPacket);
};