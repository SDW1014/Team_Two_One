#include "pch.h"
#include "NetworkPoll.h"


namespace MHKLibrary
{

	NetworkPoll::NetworkPoll(void)
		: _nListenLength(0)
		, _nServiceLength(0)
		, _nPollLength(0)
		, _isPollDied(true)
	{
	}

	NetworkPoll::~NetworkPoll(void)
	{
	}


	void NetworkPoll::InitInstance(void)
	{
	}

	void NetworkPoll::ExitInstance(void)
	{
		for (SOCKET fd : _listenSockets)
		{
			shutdown(fd, SD_BOTH);
			closesocket(fd);
		}
		_serviceSockets.clear();

		for (SOCKET fd : _serviceSockets)
		{
			closesocket(fd);
		}
		_serviceSockets.clear();
		_pollfds.clear();
		_isPollDied = true;
	}

	int /*CALLBACK*/ NetworkLive(LPVOID lpvoid)
	{
		NetworkPoll* network = (NetworkPoll*)lpvoid;
		network->_isNetworkAlive = true;
		while(network->_isNetworkAlive)
		{
			if (network->Pollfds(500)) break;
		}
	}

	inline void NetworkPoll::Makefds(void)
	{

		int index = 0;
		_nListenLength = _listenSockets.size();
		_nServiceLength = _serviceSockets.size();
		_nPollLength = _nListenLength + _nServiceLength;
		_pollfds.resize(_nPollLength);

		for (SOCKET fd : _listenSockets)
		{
			_pollfds[index].fd = fd;
			_pollfds[index].events = POLLRDNORM;
			index++;
		}

		for (SOCKET fd : _serviceSockets)
		{
			_pollfds[index].fd = fd;
			_pollfds[index].events = POLLRDNORM;
			index++;
		}

		_isPollDied = false;
	}

	inline int NetworkPoll::Pollfds(int timeout)
	{
		SOCKET sock;
		sockaddr sock_addr;
		int addrlen = sizeof(sockaddr);

		if (_isPollDied) Makefds();
		int nResult = WSAPoll(&_pollfds[0], _listenSockets.size(), timeout);
		if (NULL >= nResult) return nResult;

		//Service
		for (size_t i = _nListenLength; i < _nPollLength; i++)
		{
			if (_pollfds[i].revents & POLLRDNORM)
			{
			}

			if (_pollfds[i].revents & POLLHUP)
			{
				_isPollDied = true;
			}
		}

		//Listen
		for (size_t i = 0; i < _nListenLength; i++)
		{
			if (_pollfds[i].revents & POLLRDNORM)
			{
				addrlen = sizeof(sockaddr);
				sock = accept(_pollfds[i].fd, &sock_addr, &addrlen);
				if (INVALID_SOCKET != sock) continue;
				_serviceSockets.push_back(sock);
				_isPollDied = true;
			}
		}
	}
}

