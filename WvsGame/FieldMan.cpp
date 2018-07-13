#include "FieldMan.h"
#include "..\WvsLib\Wz\WzResMan.hpp"
#include "..\WvsLib\Memory\MemoryPoolMan.hpp"
#include "..\WvsLib\Logger\WvsLogger.h"

#include "PortalMap.h"

#include <mutex>

std::mutex fieldManMutex;

FieldMan::FieldMan()
{
}

FieldMan::~FieldMan()
{
}

void FieldMan::RegisterField(int nFieldID)
{
	std::lock_guard<std::mutex> guard(fieldManMutex);
	FieldFactory(nFieldID);
}

void FieldMan::FieldFactory(int nFieldID) 
{
	/*if (mField[nFieldID]->GetFieldID() != 0)
		return;*/
	/*
	�b�o���ˬd��Field�����A�A�ª����b�o�̮ھ�FieldType�إߤ��P���a�Ϲ���
	Field
	Field_Tutorial
	Field_ShowaBath
	Field_WeddingPhoto
	Field_SnowBall
	Field_Tournament
	Field_Coconut
	Field_OXQuiz
	Field_PersonalTimeLimit
	Field_GuildBoss
	Field_MonsterCarnival
	Field_Wedding
	...
	*/
	std::string fieldStr = std::to_string(nFieldID);
	while (fieldStr.size() < 9)
		fieldStr = "0" + fieldStr;
	Field* newField = new Field();
	auto& mapWz = stWzResMan->GetWz(Wz::Map)["Map"]["Map" + std::to_string(nFieldID / 100000000)][fieldStr];
	mapWz = stWzResMan->GetWz(Wz::Map)["Map"]["Map" + std::to_string(nFieldID / 100000000)][std::to_string(nFieldID)];
	auto& infoData = mapWz["info"];

	//if (infoData) {
		newField->SetCould(((int)infoData["cloud"] != 0));
		newField->SetTown(((int)infoData["town"] != 0));
		newField->SetSwim(((int)infoData['swim'] != 0));
		newField->SetFly(((int)infoData['fly'] != 0));
		newField->SetReturnMap(infoData["returnMap"]);
		newField->SetForcedReturn(infoData["forcedReturn"]);
		newField->SetMobRate(infoData["mobRate"]);
		newField->SetFieldType(infoData["fieldType"]);
		newField->SetFieldLimit(infoData["fieldLimit"]);
		newField->SetCreateMobInterval(infoData["createMobInterval"]);
		newField->SetFiexdMobCapacity(infoData["fixedMobCapacity"]);

		//�a�Ϫ��e
		int mapSizeX = abs((int)infoData["VRRight"] - (int)infoData["VRLeft"]);
		int mapSizeY = abs((int)infoData["VRTop"] - (int)infoData["VRBottom"]);

		newField->SetFirstUserEnter(infoData["onFirstUerEnter"]);
		newField->SetUserEnter(infoData["onUserEnter"]);

		//�H�U��T���i�a�A���Ǧa��(�S�����ݩ�)�L�k��
		newField->SetMapSizeX(mapSizeX);
		newField->SetMapSizeY(mapSizeY);
		//WvsLogger::LogFormat(WvsLogger::LEVEL_INFO, "New Field Size X = %d, Y = %d\n", (int)infoData["forcedReturn"], mapSizeY);
	//}
	newField->GetPortalMap()->RestorePortal(newField, mapWz["portal"]);

	mField[nFieldID] = newField;
	mField[nFieldID]->SetFieldID(nFieldID);
	mField[nFieldID]->InitLifePool();
}

Field* FieldMan::GetField(int nFieldID)
{
	//printf("Get Field ID = %d\n", nFieldID);
	//Prevent Double Registerations Or Enter On-Registering Map
	auto fieldResult = mField.find(nFieldID);
	if (fieldResult == mField.end())
		RegisterField(nFieldID);
	return mField[nFieldID];
}