#pragma once

//	Send�� ���� �޽����� Recv �� ���� �޽����� �����ϱ� ���� Ŭ���� �Դϴ�.
//	Ŭ���� ��Ī :	MHKLibrary::cPacket
//	���۱Ⱓ	:	2018�� 10�� 24�� ~ 2018�� 10�� 24��
//	������		:	�����
//	E-mail ���� :	rlaalsgur05@naver.com
//	Version		:	0.1


namespace MHKLibrary
{
	enum PACKET_TYPE
	{
		PACKET_NULL = 0,
		PACKET_TEXT,
		PACKET_CUSTOM,
				// TODO : �ʿ��� ��Ŷ�� �߰��ϼ���.
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

