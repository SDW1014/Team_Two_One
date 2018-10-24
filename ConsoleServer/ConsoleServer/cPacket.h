#pragma once

//	Send로 보낼 메시지와 Recv 로 받을 메시지를 가공하기 위한 클래스 입니다.
//	클래스 명칭 :	MHKLibrary::cPacket
//	제작기간	:	2018년 10월 24일 ~ 2018년 10월 24일
//	제작자		:	김민혁
//	E-mail 문의 :	rlaalsgur05@naver.com
//	Version		:	0.1


namespace MHKLibrary
{
	enum PACKET_TYPE
	{
		PACKET_NULL = 0,
		PACKET_TEXT,
		PACKET_CUSTOM,
				// TODO : 필요한 패킷을 추가하세요.
		PACKET_MAXSIZE = 256
	};

	struct DataBuffer
	{
		byte data[BUFFER_SIZE];
		size_t lenght;

		DataBuffer(LPCSTR text) : lenght(strlen(text))
		{
			strcpy_s((LPSTR)data, BUFFER_SIZE, text);
		}
	};

	class cPacket
	{
	private:
		std::queue<DataBuffer> _queuePacketBuffers;
		std::queue<DataBuffer> _queueDataBuffers;
		INT _nTextCurrent;

	public:
		LPSTR FrontPacket(void) { return (LPSTR)_queuePacketBuffers.front().data; }
		LPSTR FrontText(void) { return (LPSTR)_queueDataBuffers.front().data; }

		bool IsPacket(void);
		bool IsData(void);

		bool TextToData(LPCSTR text);
		bool PacketToData(PACKET_TYPE type, LPVOID pSource, INT sourecSize);

		void PopPacket(void);
		void PopData(void);

		void DataToPacket(LPVOID data);

		cPacket(void);
		~cPacket(void);
	};
}

