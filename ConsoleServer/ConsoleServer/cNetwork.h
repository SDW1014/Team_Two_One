#pragma once

//	해당 클래스는 아래의 예제와 같이 원속(Winsock2.h)을 편리하게 사용하려고 만들었습니다.
//	클래스 명칭 :	MHKLibrary::cNetwork
//	제작기간	:	2018년 10월 21일 ~ 2018년 10월 22일
//	제작자		:	김민혁
//	E-mail 문의 :	rlaalsgur05@naver.com
//	Version		:	0.1

namespace MHKLibrary
{
#define WSA_VERSION		MAKEWORD(2, 2)
#define BUFFER_SIZE		512

	class cNetwork;
	typedef void(*DelegateFunctionSocket)(cNetwork* pNetwork, SOCKET sock);

	//누가 무슨 내용을 보냈는지 저장한다.
	struct Message
	{
		SOCKET nSocket;
		std::string sText;

		Message(void) : Message(INVALID_SOCKET, "") {}
		Message(SOCKET sock, LPCSTR szText) : nSocket(sock) , sText(szText) {}
	};

	class cNetwork
	{
	private:
		static WSADATA _wsaData;											// 원속 초기화 변수
		static ULONG _nonBlocking;											// 소켓 논-블로킹 변수
		static INT _nInstanceCount;											// 클래스 인스턴스 카운터

		std::queue<Message> _queueRecvMessages;								// 받은 메시지 버퍼
		std::set<SOCKET> _setConnectSockets;								// 연결되있는 TCP 소켓
		INT _family, _socketType, _protocol, _recvFlags;					// 네트워크 프로토콜 정보
		SOCKET _acceptSocket;												// 연결요청 대기중인 소켓
		DelegateFunctionSocket _pConnectEnter, _pConnectExit;				// 소켓이 연결되때나 해제될때 실행된다.

		std::set<SOCKET>::iterator _it;										// 반복자
		SOCKET _socket;														// 생성된 소켓
		sockaddr _address;													// 연결된 주소
		INT _nSize, _nLength;												// 크기와 길이
		char _szBuffer[BUFFER_SIZE];										// 데이터를 받을 버퍼

		inline INT _CreateInstance(void);									// 해당 클래스 생성 체크
		inline void _ReleaseInstance(void);									// 해당 클래스 소멸 체크

		inline void _CloseConnectExit(SOCKET socket);						// _pConnectExit를 호출하고 소켓을 닫습니다.

	public:

		void AcceptClose(void);
		bool AcceptSocket(INT port);
		bool AcceptSocket(LPCSTR addressString, INT port);
		bool AcceptSocket(SOCKET socket, PSOCKADDR pSocketAddress);

		// TODO : send를 직접보내면 안될고 버퍼에 저장하도록 수정해야 함
		void AllSend(LPCSTR szText);
		void AllSend(LPCSTR szText, INT flags);

		void AllCloseConnectSocket(void);
		bool CloseConnectSocket(SOCKET socket);

		void Confurm(void);
		void ConfurmAccept(void);
		void ConfirmRecv(void);

		bool GetAddressInfo(OUT PSOCKADDR pSocketAddress);
		bool GetAddressString(OUT LPSTR pDest, INT destsize);

		int GetConnectSocketCount(void) { return _setConnectSockets.size(); }

		void GetRecvMessage(Message* pMessage);

		bool InsertConnectSocket(LPCSTR addressString, INT port);
		bool InsertConnectSocket(SOCKET socket, PSOCKADDR pSocketAddress);

		bool IsRecvMessage(void);

		bool SetAdderss(OUT PSOCKADDR pSocketAddress, LPCSTR szAddress, INT nPort);
		bool SetAdderssPort(OUT PSOCKADDR pSocketAddress, INT nPort);

		void SetDelegateConnectEnter(DelegateFunctionSocket funtion) { _pConnectEnter = funtion; }
		void SetDelegateConnectExit(DelegateFunctionSocket funtion) { _pConnectExit = funtion; }

		cNetwork(void);
		cNetwork(INT family, INT socktype, INT protocol);
		virtual ~cNetwork(void);
	};
}

//	// ex. IPv4 tcp 서버
//	void main()
//	{
//		MHKLibrary::cNetwork TCPIpv4Server;								// 네트워크 클래스 사용
//		TCPIpv4Server.AcceptSocket("127.0.0.1", 8600);					// 다른 클라이언트 연결요청 준비
//		while (true)
//		{
//			TCPIpv4Server.Confurm();									// 요청확인 및 메시지 받기
//			while (TCPIpv4Server.IsRecvMessage())						// 받은 메시지를 확인하고 연결된 모든소켓에 보냅니다.
//			{
//				TCPIpv4Server.AllSend(TCPIpv4Server.GetRecvMessage().sText.c_str());
//			}
//		}
//		TCPIpv4Server.AllCloseConnectSocket();							// 연결해제
//	}
//
//	// ex. IPv4 tcp 클라이언트(Recv)
//	void main()
//	{
//		MHKLibrary::cNetwork TCPIpv4Client;								// 네트워크 클래스 사용
//		TCPIpv4Client.InsertConnectSocket("127.0.0.1", 8600);			// 서버 또는 다른 PC와 연결
//		while (true)
//		{
//			TCPIpv4Client.ConfirmRecv();								// 메시지 확인
//			while (TCPIpv4Client.IsRecvMessage())
//			{
//				std::cout << TCPIpv4Client.GetRecvMessage().sText << std::endl;
//			}
//		}
//		TCPIpv4Client.AllCloseConnectSocket();							// 연결해제
//	}
//
//	// ex. IPv4 tcp 클라이언트(Send)
//	void main()
//	{
//		MHKLibrary::cNetwork TCPIpv4Client;								// 네트워크 클래스 사용
//		TCPIpv4Client.InsertConnectSocket("127.0.0.1", 8600);			// 서버 또는 다른 PC와 연결
//		TCPIpv4Client.AllSend("보내고 싶은 내용");						// 메시지 보내기
//		TCPIpv4Client.AllCloseConnectSocket();							// 연결해제
//	}
