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

	// ���ۿ��� �����͸� Ȯ���մϴ�.
	bool cDataBuffer::IsData(void)
	{
		return !_queueDataBuffers.empty();
	}

	// ���ۿ��� ��Ŷ�� Ȯ���մϴ�.
	bool cDataBuffer::IsPacket(void)
	{
		return !_queuePacketBuffers.empty();
	}

	// Text�� Packet�� ��� _queuePacketBuffers�� �����մϴ�.
	bool cDataBuffer::TextToPacket(LPCSTR text)
	{
		return DataToPacket(MHKLibrary::PACKET_TEXT, (LPVOID)text, strlen(text));
	}

	// Data�� Packet�� ��� _queuePacketBuffers�� �����մϴ�.
	bool cDataBuffer::DataToPacket(PACKET_TYPE type, LPVOID pSource, int sourecSize)
	{
		if (MHKLibrary::PACKET_NULL == type) return false;

		// ���� �Ҵ�
		if (BUFFER_SIZE <= (_nTextCurrent + sourecSize + (PACKET_HADE * 3)))
		{
			_queuePacketBuffers.push(Packet(MHKLibrary::PACKET_PACKET, ""));
			_nTextCurrent = 0;
		}

		// ������ �Ľ�
		_queuePacketBuffers.back().data[_nTextCurrent] = (PACKET_TYPE)type;
		_queuePacketBuffers.back().data[_nTextCurrent + PACKET_HADE] = sourecSize;
		memcpy(&_queuePacketBuffers.back().data[_nTextCurrent + (PACKET_HADE * 2)], pSource, sourecSize);
		_nTextCurrent = _nTextCurrent + sourecSize + (PACKET_HADE * 2);
		_queuePacketBuffers.back().data[_nTextCurrent] = MHKLibrary::PACKET_NULL;
		_queuePacketBuffers.back().lenght = _nTextCurrent;

		return true;
	}

	// �����͸� ���ۿ��� �����ϴ�.
	void cDataBuffer::PopData(void)
	{
		_queueDataBuffers.pop();
	}

	// ��Ŷ�� ���ۿ��� �����ϴ�.
	void cDataBuffer::PopPacket(void)
	{
		_queuePacketBuffers.pop();
		if (_queuePacketBuffers.empty()) _nTextCurrent = BUFFER_SIZE;
	}

	// recv ���� Packet�� ���� Data���� ��Ȱ�Ͽ� _queueDataBuffers �����մϴ�. 
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

