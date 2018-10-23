#include "pch.h"
#include "cNetwork.h"

namespace MHKLibrary
{
	WSADATA cNetwork::_wsaData;
	unsigned long cNetwork::_nonBlocking = TRUE;
	INT cNetwork::_nInstanceCount = 0;

	// ��Ʈ��ũ ������ PF_INET, SOCK_STREAM, IPPROTO_TCP �� �����մϴ�.
	cNetwork::cNetwork(void)
		: cNetwork(PF_INET, SOCK_STREAM, IPPROTO_TCP)
	{
	}

	// ��Ʈ��ũ ������ �����մϴ�.
	// ù ����� _CreateInstance() ������ WSAStartup() �Լ��� ȣ���մϴ�.
	cNetwork::cNetwork(INT family, INT socketType, INT protocol)
		: _family(family)
		, _socketType(socketType)
		, _protocol(protocol)
		, _acceptSocket(INVALID_SOCKET)
		, _pConnectEnter(nullptr)
		, _pConnectExit(nullptr)
		, _recvFlags(0)
	{
		if (_CreateInstance())
		{

		}
	}

	// ��Ʈ��ũ ������ �����մϴ�.
	// ������ �����  _ReleaseInstance() �� ���� WSACleanup() �Լ��� ȣ���մϴ�.
	cNetwork::~cNetwork(void)
	{
		_ReleaseInstance();
	}

	// ȣ��Ƚ���� ó���ϰ�� WSAStartup()�� ȣ���ϰ� ȣ��Ƚ���� �����Ѵ�.
	// ��ȯ���� WSAStartup()�� �����ڵ�.
	INT cNetwork::_CreateInstance(void)
	{
		if (cNetwork::_nInstanceCount > 0)
		{
			cNetwork::_nInstanceCount++;
			return 0;
		}

		cNetwork::_nInstanceCount = 1;
		return WSAStartup(WSA_VERSION, &cNetwork::_wsaData);
	}

	// ȣ��Ƚ���� �����ϰ� �������ϰ�� WSACleanup()�� ȣ���Ѵ�.
	void cNetwork::_ReleaseInstance(void)
	{
		cNetwork::_nInstanceCount--;
		if (cNetwork::_nInstanceCount <= 0) return;
		WSACleanup();
	}

	// _pConnectExit�� ȣ���ϰ� ������ �ݽ��ϴ�.
	inline void cNetwork::_CloseConnectExit(SOCKET socket)
	{
		if (_pConnectExit) _pConnectExit(this, *_it);
		shutdown(*_it, SD_BOTH);
		closesocket(*_it);
	}

	// ��ϵ� Accept ��������� shutdown �ϰ� close �մϴ�.
	// maybe-later : �����带 ����Ϸ��� ũ��Ƽ�� ������ �������մϴ�.
	void cNetwork::AcceptClose(void)
	{
		///EnterCriticalSection(&_cs);
		if (INVALID_SOCKET != _acceptSocket)
		{
			shutdown(_acceptSocket, SD_BOTH);
			closesocket(_acceptSocket);
		}
		_acceptSocket = INVALID_SOCKET;
		///LeaveCriticalSection(&_cs);
	}

	// ������ �����ϰ� bind, listen, non-blocking ó���ϰ� Accept ����������� ����մϴ�.
	// �̹� ��ϵ� ������ �ִٸ� shutdown �ϰ� close �մϴ�.
	// todo : PF_INET�� �����մϴ�.
	// maybe-later : PF_INET6 ���������� �۵��ϴ��� Ȯ������ ���߽��ϴ�.
	// maybe-later : �����带 ����Ϸ��� ũ��Ƽ�� ������ �������մϴ�.
	// maybe-later : acceptSocket �� close �ɶ� acceptSocket�� ����� ConnectSocket �� �ڵ����� close �Ǵ��� Ȯ�ξ��߽��ϴ�.
	bool cNetwork::AcceptSocket(INT port)
	{

		SOCKET sock = socket(_family, _socketType, _protocol);
		if (INVALID_SOCKET == sock) return false;

		sockaddr address;
		if (!GetAddressInfo(&address)) return false;
		if (!SetAdderssPort(&address, port)) return false;
		return AcceptSocket(sock, &address);
		//return (GetAddressInfo(&address) && SetAdderssPort(&address, port) && AcceptSocket(sock, &address)); �̷��� �صεɰ� ��������?
	}

	// ������ �����ϰ� bind, listen, non-blocking ó���ϰ� Accept ����������� ����մϴ�.
	// �̹� ��ϵ� ������ �ִٸ� shutdown �ϰ� close �մϴ�. 
	// todo : PF_INET�� �����մϴ�.
	// maybe-later : PF_INET6 ���������� �۵��ϴ��� Ȯ������ ���߽��ϴ�.
	// maybe-later : �����带 ����Ϸ��� ũ��Ƽ�� ������ �������մϴ�.
	// maybe-later : acceptSocket �� close �ɶ� acceptSocket�� ����� ConnectSocket �� �ڵ����� close �Ǵ��� Ȯ�ξ���
	bool cNetwork::AcceptSocket(LPCSTR addressString, INT port)
	{
		SOCKET sock = socket(_family, _socketType, _protocol);
		if (INVALID_SOCKET == sock) return false;

		sockaddr address;
		if (!SetAdderss(&address, addressString, port)) return false;
		return AcceptSocket(sock, &address);
		//return (SetAdderss(&address, addressString, port) && AcceptSocket(sock, &address)); �̷��� �صεɰ� ��������?
	}

	// ������ bind, listen, non-blocking ó���ϰ� Accept ����������� ����մϴ�.
	// �̹� ��ϵ� ������ �ִٸ� shutdown �ϰ� close �մϴ�. 
	// maybe-later : �����带 ����Ϸ��� ũ��Ƽ�� ������ �������մϴ�.
	bool cNetwork::AcceptSocket(SOCKET socket, PSOCKADDR pSocketAddress)
	{
		// ���⵵ && �����صεɰ� ������?
		if (SOCKET_ERROR == bind(socket, pSocketAddress, sizeof(sockaddr)))
		{
			closesocket(socket);
			return false;
		}

		if (SOCKET_ERROR == listen(socket, 5))
		{
			closesocket(socket);
			return false;
		}

		if (SOCKET_ERROR == ioctlsocket(socket, FIONBIO, &cNetwork::_nonBlocking))
		{
			closesocket(socket);
			return false;
		}

		AcceptClose();
		///EnterCriticalSection(&_cs);
		_acceptSocket = socket;
		///LeaveCriticalSection(&_cs);
		return true;
	}

	//����� ���ϸ�ο��� send �޽����� ������.
	void cNetwork::AllSend(LPCSTR szText)
	{
		AllSend(szText, _recvFlags);
	}

	//����� ���ϸ�ο��� send �޽����� ������.
	// maybe-later : �����带 ����Ϸ��� ũ��Ƽ�� ������ �������մϴ�.
	void cNetwork::AllSend(LPCSTR szText, INT flags)
	{
		///EnterCriticalSection(&_cs);
		_nLength = strlen(szText) + 1;
		for (_it = _setConnectSockets.begin(); _it != _setConnectSockets.end(); _it++)
		{
			_nSize = send(*_it, szText, _nLength, flags);
			if (_nSize)
			{
				if (SOCKET_ERROR != _nSize) continue;

				switch (GetLastError())
				{
				default:
					break;
				}
			}

			_CloseConnectExit(*_it);
			_it = _setConnectSockets.erase(_it);
		}
		///LeaveCriticalSection(&_cs);
	}

	//����� ���� ��ü�� shutdown �ϰ� close �ϰ� ��Ͽ��� ���ܽ�ŵ�ϴ�.
	// maybe-later : �����带 ����Ϸ��� ũ��Ƽ�� ������ �������մϴ�.
	void cNetwork::AllCloseConnectSocket(void)
	{

		///EnterCriticalSection(&_cs);
		for (_it = _setConnectSockets.begin(); _it != _setConnectSockets.end(); _it++)
		{
			_CloseConnectExit(*_it);
		}
		_setConnectSockets.clear();
		///LeaveCriticalSection(&_cs);
	}

	//����� ������ �ش� ������ shutdown �ϰ� close �ϰ� ��Ͽ��� ���ܽ�ŵ�ϴ�.
	// maybe-later : �����带 ����Ϸ��� ũ��Ƽ�� ������ �������մϴ�.
	bool cNetwork::CloseConnectSocket(SOCKET socket)
	{
		if (INVALID_SOCKET == socket) return false;

		///EnterCriticalSection(&_cs);
		if (_setConnectSockets.erase(socket))
		{
			///LeaveCriticalSection(&_cs);
			_CloseConnectExit(socket);
			return true;
		}
		///LeaveCriticalSection(&_cs);
		return false;
	}

	// ��� Ȯ���� �ʿ��� �Լ��� �ѹ��� ��Ƴ��� �Լ��Դϴ�.
	void cNetwork::Confurm(void)
	{
		ConfurmAccept();
		ConfirmRecv();
	}

	// ��û�� �ִ��� Ȯ���ϰ� ������ �����մϴ�.
	// maybe-later : �����带 ����Ϸ��� ũ��Ƽ�� ������ �������մϴ�.
	void cNetwork::ConfurmAccept(void)
	{
		///EnterCriticalSection(&_cs);
		if (INVALID_SOCKET != _acceptSocket)
		{
			_nLength = sizeof(sockaddr);
			_socket = accept(_acceptSocket, (PSOCKADDR)&_address, &_nLength);
			if (INVALID_SOCKET == _socket)
			{
				///LeaveCriticalSection(&_cs);
				return;
			}
			_setConnectSockets.insert(_socket);
			if (_pConnectEnter) _pConnectEnter(this, _socket);
		}
		///LeaveCriticalSection(&_cs);
	}

	// �޽����� �ִ��� Ȯ���ϰ� ������ ����մϴ�.
	// maybe-later : �����带 ����Ϸ��� ũ��Ƽ�� ������ �������մϴ�.
	void cNetwork::ConfirmRecv(void)
	{
		///EnterCriticalSection(&_cs);
		for (_it = _setConnectSockets.begin(); _it != _setConnectSockets.end(); _it++)
		{
			_nSize = recv(*_it, _szBuffer, BUFFER_SIZE, _recvFlags);
			if (_nSize)
			{
				if (SOCKET_ERROR != _nSize)
				{
					_queueRecvMessages.push(Message(*_it, _szBuffer));

					continue;
				}

				switch (GetLastError())
				{
				case WSAEWOULDBLOCK: continue;	//���� ���� �޽����� �����ϴ�.
				default:
					break;
				}
			}

			_CloseConnectExit(*_it);
			_it = _setConnectSockets.erase(_it);
			if (_it == _setConnectSockets.end()) break;
		}
		///LeaveCriticalSection(&_cs);
	}

	// ȣ��Ʈ�̸����� �ּ������� �����ɴϴ�.
	// maybe-later : �����带 ����Ϸ��� ũ��Ƽ�� ������ �������մϴ�.
	bool cNetwork::GetAddressInfo(OUT PSOCKADDR pSocketAddress)
	{
		///EnterCriticalSection(&_cs);
		if (SOCKET_ERROR == gethostname(_szBuffer, BUFFER_SIZE)) return false;

		addrinfo tints, *res = nullptr;
		char* szRemotePort = nullptr;
		ZeroMemory(&tints, sizeof(addrinfo));
		tints.ai_family = _family;
		tints.ai_socktype = _socketType;
		tints.ai_protocol = _protocol;

		if (getaddrinfo(_szBuffer, szRemotePort, &tints, &res) || (!res)) return false;
		///LeaveCriticalSection(&_cs);

		memcpy(pSocketAddress, res->ai_addr, sizeof(SOCKADDR));
		freeaddrinfo(res);
		return true;
	}

	// ȣ��Ʈ�̸����� �ּҹ��ڿ��� �����ɴϴ�.
	// todo : PF_INET �� �����մϴ�.
	bool cNetwork::GetAddressString(OUT LPSTR pDest, INT destsize)
	{
		SOCKADDR sockAddr;
		if (GetAddressInfo(&sockAddr)) return false;

		switch (_family)
		{
		case AF_INET:
			sprintf_s(pDest, destsize, "%d.%d.%d.%d",
				(unsigned char)sockAddr.sa_data[2],
				(unsigned char)sockAddr.sa_data[3],
				(unsigned char)sockAddr.sa_data[4],
				(unsigned char)sockAddr.sa_data[5]);
			return true;

		default:
			return false;
		}

		return false;
	}

	// ���� �޽����߿� �����ֱٿ� ���� �޽����� �����ɴϴ�.
	void cNetwork::GetRecvMessage(Message* pMessage)
	{
		(*pMessage) = _queueRecvMessages.front();
		_queueRecvMessages.pop();
	}

	// ���� ����� ������ �߰��մϴ�.
	bool cNetwork::InsertConnectSocket(LPCSTR addressString, INT port)
	{
		SOCKET sock = socket(_family, _socketType, _protocol);
		if (INVALID_SOCKET == sock) return false;

		sockaddr address;
		if (!SetAdderss(&address, addressString, port)) return false;


		return InsertConnectSocket(sock, &address);

		//return (SetAdderss(&address, addressString, port) && InsertConnectSocket(sock, &address)); �̷��� �صεɰ� ��������?
	}

	//���� ����� ������ �߰��մϴ�.
	// maybe-later : �����带 ����Ϸ��� ũ��Ƽ�� ������ �������մϴ�.
	bool cNetwork::InsertConnectSocket(SOCKET socket, PSOCKADDR pSocketAddress)
	{
		if (SOCKET_ERROR == connect(socket, pSocketAddress, sizeof(sockaddr)))
		{
			return false;
		}

		///EnterCriticalSection(&_cs);
		_setConnectSockets.insert(socket);
		///LeaveCriticalSection(&_cs);
		return true;
	}

	//���� �޽����� �ִ��� Ȯ���Ѵ�.
	// maybe-later : �����带 ����Ϸ��� ũ��Ƽ�� ������ �������մϴ�.
	bool cNetwork::IsRecvMessage(void)
	{
		///EnterCriticalSection(&_cs);
		if (!_queueRecvMessages.empty()) return true;
		///LeaveCriticalSection(&_cs);
		return false;
	}

	// �ʱ�ȭ�ϰ� �ּ������� �����մϴ�.
	// todo : PF_INET �� �����մϴ�.
	// maybe-later : PF_INET6 ���������� �۵��ϴ��� Ȯ������ ���߽��ϴ�.
	bool cNetwork::SetAdderss(OUT PSOCKADDR pSocketAddress, LPCSTR szAddress, INT nPort)
	{
		ZeroMemory(pSocketAddress, sizeof(sockaddr));

		switch (_family)
		{
		case PF_INET:
		{
			sockaddr_in& iPv4 = *((PSOCKADDR_IN)pSocketAddress);
			iPv4.sin_family = _family;
			iPv4.sin_port = htons(nPort);
			inet_pton(_family, szAddress, &iPv4.sin_addr);
			return true;
		}
		case PF_INET6:
		{
			sockaddr_in6& iPv6 = *((PSOCKADDR_IN6)pSocketAddress);
			iPv6.sin6_family = _family;
			iPv6.sin6_port = htons(nPort);
			inet_pton(_family, szAddress, &iPv6.sin6_addr);
			return true;
		}
		default:
			return false;
		}

		return false;
	}

	// �ּ������� ��Ʈ��ȣ�� �����մϴ�.
	// todo : PF_INET �� �����մϴ�.
	// maybe-later : PF_INET6 ���������� �۵��ϴ��� Ȯ������ ���߽��ϴ�.
	bool cNetwork::SetAdderssPort(OUT PSOCKADDR pSocketAddress, INT nPort)
	{
		switch (_family)
		{
		case PF_INET:
		{
			sockaddr_in& iPv4 = *((PSOCKADDR_IN)pSocketAddress);
			iPv4.sin_family = _family;
			iPv4.sin_port = htons(nPort);
			return true;
		}
		case PF_INET6:
		{
			sockaddr_in6& iPv6 = *((PSOCKADDR_IN6)pSocketAddress);
			iPv6.sin6_family = _family;
			iPv6.sin6_port = htons(nPort);
			return true;
		}
		default:
			return false;
		}

		return false;
	}
}
