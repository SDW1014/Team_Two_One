#pragma once

namespace MHKLibrary
{
	class NetworkWSA
	{
	private:
		static WSADATA _wsaData;
		static unsigned int _instanceCount;

	public:
		virtual void InitInstance(void) = 0;
		virtual void ExitInstance(void) = 0;

		NetworkWSA(void);
		~NetworkWSA(void);
	};
}