#include "pch.h"
#include "cNetwork.h"

namespace MHKLibrary
{
	WSADATA cNetwork::_wsaData;
	unsigned long cNetwork::_nonBlocking = TRUE;
	INT cNetwork::_nInstanceCount = 0;

	// 네트워크 정보를 PF_INET, SOCK_STREAM, IPPROTO_TCP 로 설정합니다.
	cNetwork::cNetwork(void)
		: cNetwork(PF_INET, SOCK_STREAM, IPPROTO_TCP)
	{
	}

	// 네트워크 정보를 설정합니다.
	// 첫 실행시 _CreateInstance() 를통해 WSAStartup() 함수를 호출합니다.
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

	// 네트워크 정보를 해제합니다.
	// 마지막 실행시  _ReleaseInstance() 를 통해 WSACleanup() 함수를 호출합니다.
	cNetwork::~cNetwork(void)
	{
		_ReleaseInstance();
	}

	// 호출횟수가 처음일경우 WSAStartup()를 호출하고 호출횟수를 증가한다.
	// 반환값은 WSAStartup()의 에러코드.
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

	// 호출횟수를 감소하고 마지막일경우 WSACleanup()를 호출한다.
	void cNetwork::_ReleaseInstance(void)
	{
		cNetwork::_nInstanceCount--;
		if (cNetwork::_nInstanceCount <= 0) return;
		WSACleanup();
	}

	// _pConnectExit를 호출하고 소켓을 닫습니다.
	inline void cNetwork::_CloseConnectExit(SOCKET socket)
	{
		if (_pConnectExit) _pConnectExit(this, *_it);
		shutdown(*_it, SD_BOTH);
		closesocket(*_it);
	}

	// 등록된 Accept 전용소켓을 shutdown 하고 close 합니다.
	// maybe-later : 쓰레드를 사용하려면 크리티컬 섹션을 만들어야합니다.
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

	// 소켓을 생성하고 bind, listen, non-blocking 처리하고 Accept 전용소켓으로 등록합니다.
	// 이미 등록된 소켓이 있다면 shutdown 하고 close 합니다.
	// todo : PF_INET만 지원합니다.
	// maybe-later : PF_INET6 정상적으로 작동하는지 확인하지 못했습니다.
	// maybe-later : 쓰레드를 사용하려면 크리티컬 섹션을 만들어야합니다.
	// maybe-later : acceptSocket 이 close 될때 acceptSocket로 연결된 ConnectSocket 이 자동으로 close 되는지 확인안했습니다.
	bool cNetwork::AcceptSocket(INT port)
	{

		SOCKET sock = socket(_family, _socketType, _protocol);
		if (INVALID_SOCKET == sock) return false;

		sockaddr address;
		if (!GetAddressInfo(&address)) return false;
		if (!SetAdderssPort(&address, port)) return false;
		return AcceptSocket(sock, &address);
		//return (GetAddressInfo(&address) && SetAdderssPort(&address, port) && AcceptSocket(sock, &address)); 이렇게 해두될거 같지않음?
	}

	// 소켓을 생성하고 bind, listen, non-blocking 처리하고 Accept 전용소켓으로 등록합니다.
	// 이미 등록된 소켓이 있다면 shutdown 하고 close 합니다. 
	// todo : PF_INET만 지원합니다.
	// maybe-later : PF_INET6 정상적으로 작동하는지 확인하지 못했습니다.
	// maybe-later : 쓰레드를 사용하려면 크리티컬 섹션을 만들어야합니다.
	// maybe-later : acceptSocket 이 close 될때 acceptSocket로 연결된 ConnectSocket 이 자동으로 close 되는지 확인안함
	bool cNetwork::AcceptSocket(LPCSTR addressString, INT port)
	{
		SOCKET sock = socket(_family, _socketType, _protocol);
		if (INVALID_SOCKET == sock) return false;

		sockaddr address;
		if (!SetAdderss(&address, addressString, port)) return false;
		return AcceptSocket(sock, &address);
		//return (SetAdderss(&address, addressString, port) && AcceptSocket(sock, &address)); 이렇게 해두될거 같지않음?
	}

	// 소켓을 bind, listen, non-blocking 처리하고 Accept 전용소켓으로 등록합니다.
	// 이미 등록된 소켓이 있다면 shutdown 하고 close 합니다. 
	// maybe-later : 쓰레드를 사용하려면 크리티컬 섹션을 만들어야합니다.
	bool cNetwork::AcceptSocket(SOCKET socket, PSOCKADDR pSocketAddress)
	{
		// 여기도 && 연산해두될거 같은데?
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

	//연결된 소켓모두에게 send 메시지를 보낸다.
	void cNetwork::AllSend(LPCSTR szText)
	{
		AllSend(szText, _recvFlags);
	}

	//연결된 소켓모두에게 send 메시지를 보낸다.
	// maybe-later : 쓰레드를 사용하려면 크리티컬 섹션을 만들어야합니다.
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

	//연결된 소켓 전체를 shutdown 하고 close 하고 목록에서 제외시킵니다.
	// maybe-later : 쓰레드를 사용하려면 크리티컬 섹션을 만들어야합니다.
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

	//연결된 소켓중 해당 소켓을 shutdown 하고 close 하고 목록에서 제외시킵니다.
	// maybe-later : 쓰레드를 사용하려면 크리티컬 섹션을 만들어야합니다.
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

	// 상시 확인이 필요한 함수를 한번에 모아놓은 함수입니다.
	void cNetwork::Confurm(void)
	{
		ConfurmAccept();
		ConfirmRecv();
	}

	// 요청이 있는지 확인하고 있으면 연결합니다.
	// maybe-later : 쓰레드를 사용하려면 크리티컬 섹션을 만들어야합니다.
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

	// 메시지가 있는지 확인하고 있으면 기록합니다.
	// maybe-later : 쓰레드를 사용하려면 크리티컬 섹션을 만들어야합니다.
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
				case WSAEWOULDBLOCK: continue;	//현재 받을 메시지가 없습니다.
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

	// 호스트이름으로 주소정보를 가져옵니다.
	// maybe-later : 쓰레드를 사용하려면 크리티컬 섹션을 만들어야합니다.
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

	// 호스트이름으로 주소문자열을 가져옵니다.
	// todo : PF_INET 만 지원합니다.
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

	// 받은 메시지중에 가장최근에 받은 메시지를 꺼내옵니다.
	void cNetwork::GetRecvMessage(Message* pMessage)
	{
		(*pMessage) = _queueRecvMessages.front();
		_queueRecvMessages.pop();
	}

	// 직접 연결된 소켓을 추가합니다.
	bool cNetwork::InsertConnectSocket(LPCSTR addressString, INT port)
	{
		SOCKET sock = socket(_family, _socketType, _protocol);
		if (INVALID_SOCKET == sock) return false;

		sockaddr address;
		if (!SetAdderss(&address, addressString, port)) return false;


		return InsertConnectSocket(sock, &address);

		//return (SetAdderss(&address, addressString, port) && InsertConnectSocket(sock, &address)); 이렇게 해두될거 같지않음?
	}

	//직접 연결된 소켓을 추가합니다.
	// maybe-later : 쓰레드를 사용하려면 크리티컬 섹션을 만들어야합니다.
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

	//받은 메시지가 있는지 확인한다.
	// maybe-later : 쓰레드를 사용하려면 크리티컬 섹션을 만들어야합니다.
	bool cNetwork::IsRecvMessage(void)
	{
		///EnterCriticalSection(&_cs);
		if (!_queueRecvMessages.empty()) return true;
		///LeaveCriticalSection(&_cs);
		return false;
	}

	// 초기화하고 주소정보를 설정합니다.
	// todo : PF_INET 만 지원합니다.
	// maybe-later : PF_INET6 정상적으로 작동하는지 확인하지 못했습니다.
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

	// 주소정보의 포트번호를 변경합니다.
	// todo : PF_INET 만 지원합니다.
	// maybe-later : PF_INET6 정상적으로 작동하는지 확인하지 못했습니다.
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
