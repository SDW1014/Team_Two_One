#include "pch.h"
#include "NetworkWSA.h"

//bool &&(bool first, bool second) { if(first) return first; return second; }
//bool ||(bool first, bool second) { if(first) return second; return first; }

namespace MHKLibrary
{
	WSADATA NetworkWSA::_wsaData;
	unsigned int NetworkWSA::_instanceCount = 0;

	NetworkWSA::NetworkWSA(void)
	{
		if (_instanceCount || (!WSAStartup(WSA_VERSION, &_wsaData))) _instanceCount++;
	}

	NetworkWSA::~NetworkWSA(void)
	{
		_instanceCount--;
		if (!_instanceCount) WSACleanup();
	}


}
