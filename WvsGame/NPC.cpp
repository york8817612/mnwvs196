#include "Npc.h"
#include "User.h"
#include "..\WvsLib\Net\InPacket.h"
#include "..\WvsLib\Net\OutPacket.h"
#include "..\WvsLib\Net\PacketFlags\EPacketFlags.h"
#include "MovePath.h"

Npc::Npc()
{
}

Npc::~Npc()
{
}

void Npc::OnUpdateLimitedInfo(User * pUser, InPacket * iPacket)
{
	int nRemained = iPacket->RemainedCount();
	OutPacket oPacket;
	oPacket.Encode2(EPacketFlags::SERVER_PACKET::LP_NpcMove);
	//printf("[LifePool::OnNpcPacket][OnUpdateLimitedInfo]Remained = %d\n", nRemained);
	
	oPacket.Encode4(GetFieldObjectID());
	oPacket.Encode2(iPacket->Decode2());
	oPacket.Encode4(iPacket->Decode4());
	if (nRemained > 6) {
		MovePath movePath;
		movePath.Decode(iPacket);
		movePath.Encode(&oPacket);
	}

	pUser->SendPacket(&oPacket);
}

void Npc::SendChangeControllerPacket(User * pUser)
{
	OutPacket oPacket;
	oPacket.Encode2(EPacketFlags::SERVER_PACKET::LP_NpcChangeController);
	oPacket.Encode1(1);
	oPacket.Encode4(GetFieldObjectID());
	oPacket.Encode4(GetTemplateID());
	EncodeInitData(&oPacket);
	pUser->SendPacket(&oPacket);
}

void Npc::MakeEnterFieldPacket(OutPacket *oPacket)
{
	oPacket->Encode2(EPacketFlags::SERVER_PACKET::LP_NpcEnterField); //CNpcPool::OnUserEnterField
	oPacket->Encode4(GetFieldObjectID());
	oPacket->Encode4(GetTemplateID());
	EncodeInitData(oPacket);
}

void Npc::MakeLeaveFieldPacket(OutPacket * oPacket)
{
}

void Npc::EncodeInitData(OutPacket *oPacket)
{
	oPacket->Encode2(GetPosX());
	oPacket->Encode2(GetCy());
	oPacket->Encode1(0);
	oPacket->Encode1(~GetF());
	oPacket->Encode2(GetFh());
	oPacket->Encode2(GetRx0());
	oPacket->Encode2(GetRx1());
	oPacket->Encode1(!IsHidden());
	oPacket->Encode4(0);
	oPacket->Encode1(0);
	oPacket->Encode4(-1);
	oPacket->Encode4(0);

	oPacket->Encode4(0);
	oPacket->Encode4(0);

	oPacket->EncodeStr("");
	oPacket->Encode1(0);
}