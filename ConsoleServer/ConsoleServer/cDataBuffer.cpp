#include "pch.h"
#include "cDataBuffer.h"

namespace MHKLibrary
{
	cDataBuffer::cDataBuffer(void)
		: _nTextCurrent(BUFFER_SIZE)
	{
	}

	cDataBuffer::~cDataBuffer(void)
	{
	}

	// 버퍼에서 데이터를 확인합니다.
	bool cDataBuffer::IsData(void)
	{
		return !_queueDataBuffers.empty();
	}

	// 버퍼에서 패킷을 확인합니다.
	bool cDataBuffer::IsPacket(void)
	{
		return !_queuePacketBuffers.empty();
	}

	// Text를 Packet로 모아 _queuePacketBuffers에 저장합니다.
	bool cDataBuffer::TextToPacket(LPCSTR text)
	{
		return DataToPacket(MHKLibrary::PACKET_TEXT, (LPVOID)text, strlen(text));
	}

	// Data을 Packet로 모아 _queuePacketBuffers에 저장합니다.
	bool cDataBuffer::DataToPacket(PACKET_TYPE type, LPVOID pSource, int sourecSize)
	{
		if (MHKLibrary::PACKET_NULL == type) return false;

		// 공간 할당
		if (BUFFER_SIZE <= (_nTextCurrent + sourecSize + (PACKET_HADE * 3)))
		{
			_queuePacketBuffers.push(Packet(MHKLibrary::PACKET_PACKET, ""));
			_nTextCurrent = 0;
		}

		// 데이터 파싱
		_queuePacketBuffers.back().data[_nTextCurrent] = (PACKET_TYPE)type;
		_queuePacketBuffers.back().data[_nTextCurrent + PACKET_HADE] = sourecSize;
		memcpy(&_queuePacketBuffers.back().data[_nTextCurrent + (PACKET_HADE * 2)], pSource, sourecSize);
		_nTextCurrent = _nTextCurrent + sourecSize + (PACKET_HADE * 2);
		_queuePacketBuffers.back().data[_nTextCurrent] = MHKLibrary::PACKET_NULL;
		_queuePacketBuffers.back().lenght = _nTextCurrent;

		return true;
	}

	// 데이터를 버퍼에서 꺼냅니다.
	void cDataBuffer::PopData(void)
	{
		_queueDataBuffers.pop();
	}

	// 패킷을 버퍼에서 꺼냅니다.
	void cDataBuffer::PopPacket(void)
	{
		_queuePacketBuffers.pop();
		if (_queuePacketBuffers.empty()) _nTextCurrent = BUFFER_SIZE;
	}

	// recv 받은 Packet를 여러 Data으로 분활하여 _queueDataBuffers 저장합니다. 
	void cDataBuffer::PacketToData(LPVOID packet)
	{
		for (size_t imove, i = 0; i < BUFFER_SIZE; i += imove)
		{
			imove = (PACKET_TYPE)(((LPSTR)packet)[i]);
			if (MHKLibrary::PACKET_NULL == imove) return;
			_queueDataBuffers.push(Packet((PACKET_TYPE)imove, ""));
			imove = (int)((LPSTR)packet)[i + PACKET_HADE];
			strncpy_s((LPSTR)_queueDataBuffers.back().data, BUFFER_SIZE, &((LPSTR)packet)[i + (PACKET_HADE * 2)], imove);
			_queueDataBuffers.back().lenght = imove;
			imove += (PACKET_HADE * 2);
		}
	}
}

