#include "stdafx.h"
#include "TimeManagent.h"

#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")


namespace MHKLibrary
{
	HRESULT TimeManagent::Init(void)
	{
		if (QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency))
		{
			_isHardware = true;
			QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);
			_timeScale = 1.0f / _periodFrequency;
		}
		else
		{
			_isHardware = false;
			_lastTime = timeGetTime();
			_timeScale = 0.001f;
		}

		_frameRate = 0;
		_frameCount = 0;
		_worldTime = 0;
		_momentTime = 0.0f;
		_lockFPS = 60.0f;

		return S_OK;
	}
	void TimeManagent::Release(void)
	{
	}
	void TimeManagent::Update(void)
	{
		if (_isHardware) QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
		else _curTime = timeGetTime();

		_timeElapsed = (_curTime - _lastTime) * _timeScale;

		if (_lockFPS > 0.0f) while (_timeElapsed < (1.0f / _lockFPS))
		{
			if (_isHardware) QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
			else _curTime = timeGetTime();

			_timeElapsed = (_curTime - _lastTime) * _timeScale;
		}

		_lastTime = _curTime;
		_frameCount++;
		_momentTime += _timeElapsed;
		_worldTime;

		if (_momentTime > 1.0f)
		{
			_frameRate = _frameCount;
			_frameCount = 0;
			_momentTime -= 1.0f;
			_worldTime++;
		}
	}

	TimeManagent::TimeManagent(void)
	{
	}

	TimeManagent::~TimeManagent(void)
	{
	}
}

