#pragma once

//	�ش� Ŭ������ �Ʒ��� ������ ���� ����(Winsock2.h)�� ���ϰ� ����Ϸ��� ��������ϴ�.
//	Ŭ���� ��Ī :	MHKLibrary::cNetwork
//	���۱Ⱓ	:	2018�� 10�� 21�� ~ 2018�� 10�� 22��
//	������		:	�����
//	E-mail ���� :	rlaalsgur05@naver.com
//	Version		:	0.1

namespace MHKLibrary
{
#define WSA_VERSION		MAKEWORD(2, 2)
#define BUFFER_SIZE		512

	class cNetwork;
	typedef void(*DelegateFunctionSocket)(cNetwork* pNetwork, SOCKET sock);

	//���� ���� ������ ���´��� �����Ѵ�.
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
		static WSADATA _wsaData;											// ���� �ʱ�ȭ ����
		static ULONG _nonBlocking;											// ���� ��-���ŷ ����
		static INT _nInstanceCount;											// Ŭ���� �ν��Ͻ� ī����

		std::queue<Message> _queueRecvMessages;								// ���� �޽��� ����
		std::set<SOCKET> _setConnectSockets;								// ������ִ� TCP ����
		INT _family, _socketType, _protocol, _recvFlags;					// ��Ʈ��ũ �������� ����
		SOCKET _acceptSocket;												// �����û ������� ����
		DelegateFunctionSocket _pConnectEnter, _pConnectExit;				// ������ ����Ƕ��� �����ɶ� ����ȴ�.

		std::set<SOCKET>::iterator _it;										// �ݺ���
		SOCKET _socket;														// ������ ����
		sockaddr _address;													// ����� �ּ�
		INT _nSize, _nLength;												// ũ��� ����
		char _szBuffer[BUFFER_SIZE];										// �����͸� ���� ����

		inline INT _CreateInstance(void);									// �ش� Ŭ���� ���� üũ
		inline void _ReleaseInstance(void);									// �ش� Ŭ���� �Ҹ� üũ

		inline void _CloseConnectExit(SOCKET socket);						// _pConnectExit�� ȣ���ϰ� ������ �ݽ��ϴ�.

	public:

		void AcceptClose(void);
		bool AcceptSocket(INT port);
		bool AcceptSocket(LPCSTR addressString, INT port);
		bool AcceptSocket(SOCKET socket, PSOCKADDR pSocketAddress);

		// TODO : send�� ���������� �ȵɰ� ���ۿ� �����ϵ��� �����ؾ� ��
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

//	// ex. IPv4 tcp ����
//	void main()
//	{
//		MHKLibrary::cNetwork TCPIpv4Server;								// ��Ʈ��ũ Ŭ���� ���
//		TCPIpv4Server.AcceptSocket("127.0.0.1", 8600);					// �ٸ� Ŭ���̾�Ʈ �����û �غ�
//		while (true)
//		{
//			TCPIpv4Server.Confurm();									// ��ûȮ�� �� �޽��� �ޱ�
//			while (TCPIpv4Server.IsRecvMessage())						// ���� �޽����� Ȯ���ϰ� ����� �����Ͽ� �����ϴ�.
//			{
//				TCPIpv4Server.AllSend(TCPIpv4Server.GetRecvMessage().sText.c_str());
//			}
//		}
//		TCPIpv4Server.AllCloseConnectSocket();							// ��������
//	}
//
//	// ex. IPv4 tcp Ŭ���̾�Ʈ(Recv)
//	void main()
//	{
//		MHKLibrary::cNetwork TCPIpv4Client;								// ��Ʈ��ũ Ŭ���� ���
//		TCPIpv4Client.InsertConnectSocket("127.0.0.1", 8600);			// ���� �Ǵ� �ٸ� PC�� ����
//		while (true)
//		{
//			TCPIpv4Client.ConfirmRecv();								// �޽��� Ȯ��
//			while (TCPIpv4Client.IsRecvMessage())
//			{
//				std::cout << TCPIpv4Client.GetRecvMessage().sText << std::endl;
//			}
//		}
//		TCPIpv4Client.AllCloseConnectSocket();							// ��������
//	}
//
//	// ex. IPv4 tcp Ŭ���̾�Ʈ(Send)
//	void main()
//	{
//		MHKLibrary::cNetwork TCPIpv4Client;								// ��Ʈ��ũ Ŭ���� ���
//		TCPIpv4Client.InsertConnectSocket("127.0.0.1", 8600);			// ���� �Ǵ� �ٸ� PC�� ����
//		TCPIpv4Client.AllSend("������ ���� ����");						// �޽��� ������
//		TCPIpv4Client.AllCloseConnectSocket();							// ��������
//	}
