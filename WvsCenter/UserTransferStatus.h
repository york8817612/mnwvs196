#pragma once
#include <vector>

class InPacket;
class OutPacket;

/*
�ΨӫO�dUser�ഫ�W�D�Ϊ̶i�J�ӫ����ɭԡA�һݭn�Ȧs����T
*/
class UserTransferStatus
{
	int m_nChannelID;
	struct TransferTemporaryStatus
	{
		int nSkillID, nSLV, tTime;

		void Encode(OutPacket *oPacket) const;
		void Decode(InPacket *iPacket);
	};
	std::vector<TransferTemporaryStatus> m_aTS;

public:
	UserTransferStatus();
	~UserTransferStatus();

	void Decode(InPacket* iPacket);
	void Encode(OutPacket* oPacket) const;
};

