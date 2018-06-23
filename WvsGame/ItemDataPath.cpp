#include <string>

static const std::string sCharacterRoot = "";
static const std::string sCustomMerge = "CustomMerge.img";
static const std::string sShoes = "Shoes";
static const std::string sSkillSkin = "SkillSkin";
static const std::string sCape = "Cape";
static const std::string sGlove = "Glove";
static const std::string sCoat = "Coat";
static const std::string sFace = "Face";
static const std::string sWeapon = "Weapon";
static const std::string sHair = "Hair";
static const std::string sPants = "Pants";
static const std::string sAndroid = "Android";
static const std::string sLongcoat = "Longcoat";
static const std::string sMechanic = "Mechanic";
static const std::string sTamingMob = "TamingMob";
static const std::string sPetEquip = "PetEquip";
static const std::string sBits = "Bits";
static const std::string sDragon = "Dragon";
static const std::string sFamiliar = "Familiar";
static const std::string sArcaneForce = "ArcaneForce";
static const std::string sCap = "Cap";
static const std::string sTotem = "Totem";
static const std::string sAccessory = "Accessory";
static const std::string sRing = "Ring";
static const std::string sAfterimage = "Afterimage";
static const std::string sShield = "Shield";

const std::string* GetEquipDataPath(int nItemID) {
	switch (int(nItemID / 10000))
	{
	case 0: return &sCharacterRoot;
	case 1: return &sCharacterRoot;
	case 2: return &sFace;
	case 3: return &sHair;
	case 4: return &sHair;
	case 100: return &sCap;
	case 101: return &sAccessory;
	case 102: return &sAccessory;
	case 103: return &sAccessory;
	case 104: return &sCoat;
	case 105: return &sLongcoat;
	case 106: return &sPants;
	case 107: return &sShoes;
	case 108: return &sGlove;
	case 109: return &sShield;
	case 110: return &sCape;
	case 111: return &sRing;
	case 112: return &sAccessory;
	case 113: return &sAccessory;
	case 114: return &sAccessory;
	case 115: return &sAccessory;
	case 116: return &sAccessory;
	case 118: return &sAccessory;
	case 119: return &sAccessory;
	case 120: return &sTotem;
	case 121: return &sWeapon;
	case 122: return &sWeapon;
	case 123: return &sWeapon;
	case 124: return &sWeapon;
	case 125: return &sWeapon;
	case 126: return &sWeapon;
	case 130: return &sWeapon;
	case 131: return &sWeapon;
	case 132: return &sWeapon;
	case 133: return &sWeapon;
	case 134: return &sWeapon;
	case 135: return &sWeapon;
	case 136: return &sWeapon;
	case 137: return &sWeapon;
	case 138: return &sWeapon;
	case 139: return &sWeapon;
	case 140: return &sWeapon;
	case 141: return &sWeapon;
	case 142: return &sWeapon;
	case 143: return &sWeapon;
	case 144: return &sWeapon;
	case 145: return &sWeapon;
	case 146: return &sWeapon;
	case 147: return &sWeapon;
	case 148: return &sWeapon;
	case 149: return &sWeapon;
	case 150: return &sWeapon;
	case 151: return &sWeapon;
	case 152: return &sWeapon;
	case 153: return &sWeapon;
	case 154: return &sWeapon;
	case 155: return &sWeapon;
	case 156: return &sWeapon;
	case 157: return &sWeapon;
	case 158: return &sWeapon;
	case 160: return &sWeapon;
	case 161: return &sMechanic;
	case 162: return &sMechanic;
	case 163: return &sMechanic;
	case 164: return &sMechanic;
	case 165: return &sMechanic;
	case 166: return &sAndroid;
	case 167: return &sAndroid;
	case 168: return &sBits;
	case 169: return &sWeapon;
	case 170: return &sWeapon;
	case 171: return &sArcaneForce;
	case 180: return &sPetEquip;
	case 181: return &sPetEquip;
	case 190: return &sTamingMob;
	case 191: return &sTamingMob;
	case 193: return &sTamingMob;
	case 194: return &sDragon;
	case 195: return &sDragon;
	case 196: return &sDragon;
	case 197: return &sDragon;
	case 198: return &sTamingMob;
	case 199: return &sTamingMob;
	case 997: return &sFamiliar;
	}
	return nullptr;
}