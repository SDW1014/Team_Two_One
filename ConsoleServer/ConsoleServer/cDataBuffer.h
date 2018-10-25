#pragma once

//	Send로 보낼 메시지와 Recv 로 받을 메시지를 가공하기 위한 클래스 입니다.
//	클래스 명칭 :	MHKLibrary::cPacket
//	제작기간	:	2018년 10월 24일 ~ 2018년 10월 25일
//	제작자		:	김민혁
//	E-mail 문의 :	rlaalsgur05@naver.com
//	Version		:	0.2


namespace MHKLibrary
{
	enum PACKET_TYPE
	{
		PACKET_NULL = 0,
		PACKET_PACKET,
		PACKET_TEXT,
		PACKET_DATA,
				// TODO : 필요한 패킷을 추가하세요.
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

