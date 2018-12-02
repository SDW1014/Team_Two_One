#pragma once
#include "NetworkWSA.h"

namespace MHKLibrary
{
	// TODO : 아직 작업이 완료되지 않았습니다.
	class NetworkPoll : public NetworkWSA
	{
	private:
		std::vector<SOCKET> _listenSockets;
		std::vector<SOCKET> _serviceSockets;
		std::vector<WSAPOLLFD> _pollfds;
		unsigned int _nListenLength, _nServiceLength, _nPollLength;
		bool _isPollDied, _isNetworkAlive;

	public:
		virtual void InitInstance(void) override;
		virtual void ExitInstance(void) override;

		friend int /*CALLBACK*/ NetworkLive(LPVOID lpvoid);
		//void ListenLive(void);

	private:
		inline void Makefds(void);
		inline int Pollfds(int timeout);

	public:
		NetworkPoll(void);
		~NetworkPoll(void);
	};
}

