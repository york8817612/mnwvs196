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

void FieldMan::Initialize() {
	/*WvsLogger::LogRaw("[FieldMan::Initialize]開始載入所有地圖[RegisterAllField Start]....\n");
	auto& mapWz = stWzResMan->GetWz(Wz::Map)["Map"];
	for (auto& mapWzNode : mapWz)
	{
		if (mapWzNode.Name().find("Map") == 0)
		{
			for (auto& mapWzSubNode : mapWzNode)
			{
				RegisterField(atoi(mapWzSubNode.Name().c_str()));
			}
		}
	}
	stWzResMan->ReleaseMemory();
	WvsLogger::LogRaw("[FieldMan::Initialize]所有地圖載入完成[RegisterAllField Done]....\n");*/
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
	在這裡檢查此Field的型態，舊版的在這裡根據FieldType建立不同的地圖實體
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

		//地圖長寬
		int mapSizeX = abs((int)infoData["VRRight"] - (int)infoData["VRLeft"]);
		int mapSizeY = abs((int)infoData["VRTop"] - (int)infoData["VRBottom"]);

		newField->SetFirstUserEnter(infoData["onFirstUerEnter"]);
		newField->SetUserEnter(infoData["onUserEnter"]);

		//以下資訊不可靠，有些地圖(沒有該屬性)無法算
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