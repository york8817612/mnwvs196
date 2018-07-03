#include "InventoryManipulator.h"
#include "..\Database\GA_Character.hpp"
#include "..\Database\GW_ItemSlotBundle.h"
#include "..\Database\GW_ItemSlotBase.h"
#include "..\Common\Net\PacketFlags\EPacketFlags.h"
#include "ItemInfo.h"
#include "SkillInfo.h"

InventoryManipulator::InventoryManipulator()
{
}


InventoryManipulator::~InventoryManipulator()
{
}

void InventoryManipulator::SwapSlot(GA_Character* pCharacterData, std::vector<ChangeLog>& aChangeLog, int nTI, int nPOS1, int nPOS2)
{
	auto pItemSrc = pCharacterData->mItemSlot[nTI][nPOS1];
	auto pItemDst = pCharacterData->mItemSlot[nTI][nPOS2];
	pItemSrc->nPOS = nPOS2;
	if(pItemDst)
		pItemDst->nPOS = nPOS1;

	if (pItemDst)
		pCharacterData->mItemSlot[nTI][nPOS1] = pItemDst;
	else
		pCharacterData->mItemSlot[nTI].erase(nPOS1);
	pCharacterData->mItemSlot[nTI][nPOS2] = pItemSrc;
	InventoryManipulator::InsertChangeLog(aChangeLog, 2, nTI, nPOS1, nullptr, nPOS2, 1);
}

bool InventoryManipulator::IsItemExist(GA_Character* pCharacterData, int nTI, int nItemID)
{
	return pCharacterData->GetItemCount(nTI, nItemID) > 0;
}

bool InventoryManipulator::RawAddItem(GA_Character * pCharacterData, int nTI, GW_ItemSlotBase * pItem, std::vector<ChangeLog>& aChangeLog, int * nIncRet)
{
	/*
	���B�ˬd�O��CashItem
	*/
	if (nTI < 1 || nTI > 5)
		return false;
	auto& itemSlot = pCharacterData->mItemSlot[nTI];
	if (pItem->IsTreatSingly())
	{
		short nPOS = pCharacterData->FindEmptySlotPosition(nTI);
		if (nPOS > 0) 
		{
			itemSlot[nPOS] = pItem;
			pItem->nPOS = nPOS;
			InsertChangeLog(aChangeLog, 0, nTI, nPOS, pItem, 0, 0);
			*nIncRet = 1;
			return true;
		}
		return false;
	}

	if (ItemInfo::GetInstance()->GetBundleItem(pItem->nItemID) != nullptr)
	{
		int nMaxPerSlot = SkillInfo::GetInstance()->GetBundleItemMaxPerSlot(pItem->nItemID, pCharacterData),
			//nLastPos = pCharacterData->mItemSlot[nTI].size() == 0 ? 1 : pCharacterData->mItemSlot[nTI].rbegin()->first,
			nPOS = 1,
			nOnTrading = 0,
			nRemaining = 0,
			nSlotInc = 0,
			nTotalInc = 0,
			nNumber = ((GW_ItemSlotBundle*)pItem)->nNumber; //�n�[�J��쪺���~�ƶq


		//while (nPOS <= nLastPos)
		for(auto& pos : itemSlot)
		{
			nPOS = pos.first;
			if (nPOS <= 0)
				continue;
			if (nPOS >= 10000)
				break;

			auto pItemInSlot = pCharacterData->GetItem(nTI, nPOS);
			
			//���q�I�]���ۦP�����~
			if (pItemInSlot != nullptr && pItemInSlot->nItemID == pItem->nItemID)
			{
				//�T�{������٥i�H��h�֭ӬۦP���~
				nRemaining = (nMaxPerSlot - ((GW_ItemSlotBundle*)pItemInSlot)->nNumber);
				
				//�����w���A�~���U�@��
				if (nRemaining <= 0)
					continue;
				nOnTrading = pCharacterData->mItemTrading[nTI][nPOS];
				nSlotInc = nNumber > nRemaining ? nRemaining : (nNumber); //�o���i�H�W�[�h��
				//printf("Add To Bag %d, nNumber = %d, nRemaining = %d, nMaxPerSlot = %d\n", nPOS, nNumber, nRemaining, nMaxPerSlot);
				if (nSlotInc - nOnTrading > 0)
				{
					((GW_ItemSlotBundle*)pItemInSlot)->nNumber += (nSlotInc - nOnTrading);
					InsertChangeLog(aChangeLog, 1, nTI, nPOS, pItemInSlot, 0, ((GW_ItemSlotBundle*)pItemInSlot)->nNumber);
				}
				else
				{
					//�٤��T�wmItemTrading�Oԣ
				}
				nNumber -= nSlotInc;
				nTotalInc += nSlotInc;
				if (nNumber <= 0)
					break;
			}

		}

		//���L�ۦP���~�A��s����촡�J
		while (nNumber > 0)
		{
			printf("InventoryManipulator::RawAddItem nItemID = %d nNumber = %d nMaxPerSlot = %d Test = %d\n", pItem->nItemID, nNumber, nMaxPerSlot, (int)(ItemInfo::GetInstance()->GetBundleItem(pItem->nItemID) == nullptr));
			nPOS = pCharacterData->FindEmptySlotPosition(nTI);

			//�i�����~�å�������J�I�]���C
			if (nPOS <= 0)
			{
				*nIncRet = nTotalInc;
				return false;
			}
			/*
			�p�GpItem�Ѿl���ƶq�S���W�LnMaxPerSlot�A���pItem��islot���C
			�p�G�W�L�̤j�ƶq�A�h���ͤ@��pClone�A�ƶq��nMaxPerSlot�A�åB�NpItem�Ѿl�ƶq��hnSlotInc (nMaxPerSlot)�C
			*/
			auto pClone = nNumber > nMaxPerSlot ? pItem->MakeClone() : pItem;
			nSlotInc = nNumber > nMaxPerSlot ? nMaxPerSlot : (nNumber); //�o���i�H�W�[�h�� ?
			((GW_ItemSlotBundle*)pClone)->nNumber = nSlotInc;
			itemSlot[nPOS] = pClone;
			pClone->nPOS = nPOS;

			InsertChangeLog(aChangeLog, 0, nTI, nPOS, pItem, 0, 0);
			nNumber -= nSlotInc;
			nTotalInc += nSlotInc;
		}
		*nIncRet = nTotalInc;
	}
	return true;
}

void InventoryManipulator::InsertChangeLog(std::vector<ChangeLog>& aChangeLog, int nChange, int nTI, int nPOS, GW_ItemSlotBase * pi, int nPOS2, int nNumber)
{
	ChangeLog newLog;
	newLog.nChange = nChange;
	newLog.nTI = nTI;
	newLog.nPOS = nPOS;
	newLog.nPOS2 = nPOS2;
	newLog.nNumber = nNumber;
	newLog.pItem = pi;
	aChangeLog.push_back(newLog);
}

void InventoryManipulator::MakeInventoryOperation(OutPacket * oPacket, int bOnExclResult, std::vector<InventoryManipulator::ChangeLog>& aChangeLog)
{
	oPacket->Encode2(EPacketFlags::SERVER_PACKET::LP_InventoryOperation);
	oPacket->Encode1(bOnExclResult);
	oPacket->Encode1((char)aChangeLog.size());
	oPacket->Encode1(0);
	for (auto& change : aChangeLog)
	{
		//printf("Encoding Inventory Operation\n");
		oPacket->Encode1((char)change.nChange);
		oPacket->Encode1((char)change.nTI);
		oPacket->Encode2((short)change.nPOS);
		if (change.nChange)
		{
			if (change.nChange == 1)
				oPacket->Encode2((short)change.nNumber);
			if (change.nChange == 2)
				oPacket->Encode2((short)change.nPOS2);
			if (change.nChange == 3 && change.nPOS < 0)
				oPacket->Encode1(1);
		}
		else 
		{
			change.pItem->RawEncode(oPacket);
			oPacket->Encode4(0); // what's this?
		}
	}
	//printf("Encoding Inventory Operation Done\n");
	oPacket->Encode4(0); // what's this?
}

bool InventoryManipulator::RawRemoveItem(GA_Character * pCharacterData, int nTI, int nPOS, int nCount, std::vector<ChangeLog>& aChangeLog, int * nDecRet, GW_ItemSlotBase ** ppItemRemoved)
{
	auto pItem = pCharacterData->GetItem(nTI, nPOS);
	GW_ItemSlotBase* pClone = ppItemRemoved ? pItem->MakeClone() : nullptr;
	printf("Raw Remove Item pClone == null ? %d\n", (int)(pClone == nullptr));
	if (pItem != nullptr)
	{
		int nRemaining = 0;
		//
		if (pItem->IsTreatSingly() && nTI == 1)
			pCharacterData->RemoveItem(nTI, nPOS);
		else if (nCount >= 1)
		{
			GW_ItemSlotBundle* pBundle = (GW_ItemSlotBundle*)pItem;
			int nInSlotCount = pBundle->nNumber;
			if (nCount > nInSlotCount)
				nCount = nInSlotCount;
			pBundle->nNumber -= nCount;
			nRemaining = pBundle->nNumber;
			if (nRemaining <= 0)
				pCharacterData->RemoveItem(nTI, nPOS);
		}

		if (ppItemRemoved) 
		{
			*ppItemRemoved = pClone;
		}
		if (ppItemRemoved && nCount >= 1 && nTI != 1)
			((GW_ItemSlotBundle*)*ppItemRemoved)->nNumber = nCount;

		*nDecRet = nCount;
		if(nRemaining > 0)
			InsertChangeLog(aChangeLog, 1, nTI, nPOS, pItem, 0, nRemaining);
		else
			InsertChangeLog(aChangeLog, 3, nTI, nPOS, pItem, 0, *nDecRet);
	}
	return true;
}
