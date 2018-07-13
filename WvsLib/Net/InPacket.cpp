#include "InPacket.h"
#include <iostream>
#include "..\Logger\WvsLogger.h"

InPacket::InPacket(unsigned char* buff, unsigned short size)
	: aBuff(buff),
	  nPacketSize(size), nReadPos(0)
{
}

InPacket::~InPacket()
{
}

char InPacket::Decode1()
{
	if(nReadPos >= nPacketSize || nReadPos + sizeof(char) > nPacketSize)
	{
		std::cout << "InPacket::Decode1 ���~ : �s�����`�C" << std::endl;
		return 0;
	}
	char ret = *(char*)(aBuff + nReadPos);
	nReadPos += sizeof(char);
	return ret;
}

short InPacket::Decode2()
{
	if (nReadPos >= nPacketSize || nReadPos + sizeof(short) > nPacketSize)
	{
		std::cout << "InPacket::Decode2 ���~ : �s�����`�C" << std::endl;
		return 0;
	}
	short ret = *(short*)(aBuff + nReadPos);
	nReadPos += sizeof(short);
	return ret;
}

int InPacket::Decode4()
{
	if (nReadPos >= nPacketSize || nReadPos + sizeof(int) > nPacketSize)
	{
		std::cout << "InPacket::Decode4 ���~ : �s�����`�C" << std::endl;
		return 0;
	}
	int ret = *(int*)(aBuff + nReadPos);
	nReadPos += sizeof(int);
	return ret;
}

long long int InPacket::Decode8()
{
	if (nReadPos >= nPacketSize || nReadPos + sizeof(long long int) > nPacketSize)
	{
		std::cout << "InPacket::Decode8 ���~ :�@�s�����`�C" << std::endl;
		return 0;
	}
	long long int ret = *(long long int*)(aBuff + nReadPos);
	nReadPos += sizeof(long long int);
	return ret;
}

std::string InPacket::DecodeStr()
{
	if (nReadPos >= nPacketSize || nReadPos + sizeof(short) > nPacketSize)
	{
		std::cout << "InPacket::DecodeStr ���~ : �s�����`�C (�o�ͩ�ѪR�r����׮�)" << std::endl;
		return 0;
	}
	short size = Decode2();
	if (nReadPos >= nPacketSize || nReadPos + size > nPacketSize)
	{
		std::cout << "InPacket::DecodeStr ���~ : �s�����`�C (�o�ͩ�ѪR�r���)" << std::endl;
		return "null";
	}
	std::string ret((char*)aBuff + nReadPos, size);
	nReadPos += size;
	return ret;
}

void InPacket::DecodeBuffer(unsigned char* dst, int size)
{
	if (nReadPos >= nPacketSize || nReadPos + size > nPacketSize)
	{
		std::cout << "InPacket::DecodeBuffer ���~ : �s�����`�C" << std::endl;
		return;
	}
	if(dst)
		memcpy(dst, aBuff + nReadPos, size);
	nReadPos += size;
}

void InPacket::Print()
{
	//setbuf(stdout, NULL);
	//std::cout << "InPacket�ʥ]���e�G";
	for (int i = 0; i < nPacketSize; ++i)
		//std::cout << std::hex << (int)aBuff[i] << " ";
		WvsLogger::LogFormat("0x%02X ", (int)aBuff[i]);
	WvsLogger::LogRaw("\n");
	//std::cout << std::endl;
	//std::flush(std::cout);
}

unsigned char* InPacket::GetPacket() const
{
	return aBuff;
}

unsigned short InPacket::GetPacketSize() const
{
	return nPacketSize;
}

unsigned short InPacket::RemainedCount() const
{
	if (nPacketSize <= nReadPos)
		return 0;
	return nPacketSize - nReadPos;
}

void InPacket::RestorePacket()
{
	nReadPos = 0;
}