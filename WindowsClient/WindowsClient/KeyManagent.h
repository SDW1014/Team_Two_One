#pragma once
#include "Singleton.h"

namespace MHKLibrary
{
#define KEY_MANAGENT		KeyManagent::GetInstance()
#define KEYMAX 256

	class KeyManagent : public Singleton<KeyManagent>
	{
	private:
		std::bitset<KEYMAX> _isOnceKeys;
		std::bitset<KEYMAX> _isStayKeys;

		bool _GetOnceKey(int key) { return _isOnceKeys[key]; }
		bool _GetStayKey(int key) { return _isStayKeys[key]; }
		void _SetOnceKey(int key, bool state) { _isOnceKeys.set(key, state); }
		void _SetStayKey(int key, bool state) { _isStayKeys.set(key, state); }

	public:
		HRESULT Init(void);

		bool IsOnceKeyDown(int key);
		bool IsOnceKeyUp(int key);
		bool IsStayKeyDown(int key);
		bool IsToggleKey(int key);

		void OnceKeyCheck(int key);

		KeyManagent(void);
		~KeyManagent(void);
	};

}

