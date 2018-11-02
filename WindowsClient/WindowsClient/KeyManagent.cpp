#include "stdafx.h"
#include "KeyManagent.h"


namespace MHKLibrary
{
	KeyManagent::KeyManagent(void)
	{
	}

	KeyManagent::~KeyManagent(void)
	{
	}


	HRESULT KeyManagent::Init(void)
	{
		_isOnceKeys.reset();
		_isStayKeys.reset();

		return S_OK;
	}

	void KeyManagent::OnceKeyCheck(int key)
	{
		if (GetAsyncKeyState(key))
		{
			if (_GetStayKey(key))
			{
				_SetOnceKey(key, false);
			}
			else
			{
				_SetStayKey(key, true);
				_SetOnceKey(key, true);
			}
		}
		else
		{
			if (_GetStayKey(key))
			{
				_SetStayKey(key, false);
				_SetOnceKey(key, true);
			}
			else
			{
				_SetOnceKey(key, false);
			}
		}
	}

	bool KeyManagent::IsOnceKeyDown(int key)
	{
		return _GetStayKey(key) ? _GetOnceKey(key) : false;
	}

	bool KeyManagent::IsOnceKeyUp(int key)
	{
		return _GetStayKey(key) ? false : _GetOnceKey(key);
	}

	bool KeyManagent::IsStayKeyDown(int key)
	{
		if (GetKeyState(key) & 0x8000) return true;
		return false;
	}

	bool KeyManagent::IsToggleKey(int key)
	{
		if (GetKeyState(key) & 0x0001) return true;
		return false;
	}

}