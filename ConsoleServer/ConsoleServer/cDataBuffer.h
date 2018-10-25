#pragma once

//	Send�� ���� �޽����� Recv �� ���� �޽����� �����ϱ� ���� Ŭ���� �Դϴ�.
//	Ŭ���� ��Ī :	MHKLibrary::cPacket
//	���۱Ⱓ	:	2018�� 10�� 24�� ~ 2018�� 10�� 25��
//	������		:	�����
//	E-mail ���� :	rlaalsgur05@naver.com
//	Version		:	0.2


namespace MHKLibrary
{
	enum PACKET_TYPE
	{
		PACKET_NULL = 0,
		PACKET_PACKET,
		PACKET_TEXT,
		PACKET_DATA,
				// TODO : �ʿ��� ��Ŷ�� �߰��ϼ���.
		PACKET_MAXSIZE = 0xffffffff
	};

	struct Packet
	{
		int type, lenght;
		byte data[BUFFER_SIZE];

		Packet(PACKET_TYPE type, LPCSTR text) : type(type), lenght(strlen(text))
		{
			strcpy_s((LPSTR)data, BUFFER_SIZE, text);
		}
	};

	class cDataBuffer
	{
	private:
		std::queue<Packet> _queueDataBuffers;
		std::queue<Packet> _queuePacketBuffers;
		size_t _nTextCurrent;

	public:
		LPSTR FrontData(void) { return (LPSTR)_queueDataBuffers.front().data; }
		LPSTR FrontPacket(void) { return (LPSTR)_queuePacketBuffers.front().data; }

		bool IsData(void);
		bool IsPacket(void);

		bool TextToPacket(LPCSTR text);
		bool DataToPacket(PACKET_TYPE type, LPVOID pSource, int sourecSize);

		void PopData(void);
		void PopPacket(void);

		void PacketToData(LPVOID packet);

		cDataBuffer(void);
		~cDataBuffer(void);
	};
}

