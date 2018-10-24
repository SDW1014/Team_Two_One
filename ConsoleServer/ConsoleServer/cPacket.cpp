#include "pch.h"
#include "cPacket.h"

namespace MHKLibrary
{
	cPacket::cPacket(void)
		: _nTextCurrent(BUFFER_SIZE)
	{
	}

	cPacket::~cPacket(void)
	{
	}

	// 버퍼에서 패킷을 확인합니다.
	bool cPacket::IsPacket(void)
	{
		return !_queuePacketBuffers.empty();
	}

	// 버퍼에서 데이터를 확인합니다.
	bool cPacket::IsData(void)
	{
		return !_queueDataBuffers.empty();
	}

	// Text를 Data로 모아 _queueDataBuffers에 저장합니다.
	bool cPacket::TextToData(LPCSTR text)
	{
		return PacketToData(MHKLibrary::PACKET_TEXT, (LPVOID)text, strlen(text));
	}

	// Packet을 Data로 모아 _queueDataBuffers에 저장합니다.
	bool cPacket::PacketToData(PACKET_TYPE type, LPVOID pSource, INT sourecSize)
	{
		if (PACKET_MAXSIZE <= sourecSize || MHKLibrary::PACKET_NULL == type) return false;

		// 공간 할당
		if (BUFFER_SIZE <= (_nTextCurrent + sourecSize + 3))
		{
			_queueDataBuffers.push("");
			_nTextCurrent = 0;
		}

		// 데이터 파싱
		_queueDataBuffers.back().data[_nTextCurrent] = type;
		_queueDataBuffers.back().data[_nTextCurrent + 1] = (unsigned char)sourecSize;
		memcpy(&_queueDataBuffers.back().data[_nTextCurrent + 2], pSource, sourecSize);
		_nTextCurrent = _nTextCurrent + sourecSize + 2;
		_queueDataBuffers.back().data[_nTextCurrent] = '\0';
		_queueDataBuffers.back().lenght = _nTextCurrent;

		return true;
	}

	// 패킷을 버퍼에서 꺼냅니다.
	void cPacket::PopPacket(void)
	{
		_queuePacketBuffers.pop();
	}

	// 데이터를 버퍼에서 꺼냅니다.
	void cPacket::PopData(void)
	{
		_queueDataBuffers.pop();
		if (_queueDataBuffers.empty()) _nTextCurrent = BUFFER_SIZE;
	}

	// recv 받은 Data를 여러 Packet으로 분활하여 _queuePacketBuffers 저장합니다. 
	void cPacket::DataToPacket(LPVOID data)
	{
		for (size_t imove, i = 0; i < BUFFER_SIZE; i += imove)
		{
			if (NULL == ((LPSTR)data)[i]) return;
			_queuePacketBuffers.push("");
			imove = (unsigned char)((LPSTR)data)[i + 1] + 2;
			strncpy_s((LPSTR)_queuePacketBuffers.back().data, BUFFER_SIZE, &((LPSTR)data)[i], imove);
			_queuePacketBuffers.back().lenght = imove;
		}
	}
}

