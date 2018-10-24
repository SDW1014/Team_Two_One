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

	// ���ۿ��� ��Ŷ�� Ȯ���մϴ�.
	bool cPacket::IsPacket(void)
	{
		return !_queuePacketBuffers.empty();
	}

	// ���ۿ��� �����͸� Ȯ���մϴ�.
	bool cPacket::IsData(void)
	{
		return !_queueDataBuffers.empty();
	}

	// Text�� Data�� ��� _queueDataBuffers�� �����մϴ�.
	bool cPacket::TextToData(LPCSTR text)
	{
		return PacketToData(MHKLibrary::PACKET_TEXT, (LPVOID)text, strlen(text));
	}

	// Packet�� Data�� ��� _queueDataBuffers�� �����մϴ�.
	bool cPacket::PacketToData(PACKET_TYPE type, LPVOID pSource, INT sourecSize)
	{
		if (PACKET_MAXSIZE <= sourecSize || MHKLibrary::PACKET_NULL == type) return false;

		// ���� �Ҵ�
		if (BUFFER_SIZE <= (_nTextCurrent + sourecSize + 3))
		{
			_queueDataBuffers.push("");
			_nTextCurrent = 0;
		}

		// ������ �Ľ�
		_queueDataBuffers.back().data[_nTextCurrent] = type;
		_queueDataBuffers.back().data[_nTextCurrent + 1] = (unsigned char)sourecSize;
		memcpy(&_queueDataBuffers.back().data[_nTextCurrent + 2], pSource, sourecSize);
		_nTextCurrent = _nTextCurrent + sourecSize + 2;
		_queueDataBuffers.back().data[_nTextCurrent] = '\0';
		_queueDataBuffers.back().lenght = _nTextCurrent;

		return true;
	}

	// ��Ŷ�� ���ۿ��� �����ϴ�.
	void cPacket::PopPacket(void)
	{
		_queuePacketBuffers.pop();
	}

	// �����͸� ���ۿ��� �����ϴ�.
	void cPacket::PopData(void)
	{
		_queueDataBuffers.pop();
		if (_queueDataBuffers.empty()) _nTextCurrent = BUFFER_SIZE;
	}

	// recv ���� Data�� ���� Packet���� ��Ȱ�Ͽ� _queuePacketBuffers �����մϴ�. 
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

