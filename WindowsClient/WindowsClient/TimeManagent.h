#pragma once
#include "Singleton.h"

namespace MHKLibrary
{
#define TIME_MANAGENT		TimeManagent::GetInstance()

	class TimeManagent : public Singleton<TimeManagent>
	{
	private:
		bool	_isHardware;
		float	_timeElapsed;
		float	_timeScale;

		__int64	_curTime, _lastTime;
		__int64 _periodFrequency;

		unsigned long _frameRate;
		unsigned long _frameCount;
		unsigned long _worldTime;
		float _momentTime;
		float _lockFPS;

	public:
		HRESULT Init(void);
		void Release(void);
		void Update(void);

		void SetLockFPS(const float lockFPS) { _lockFPS = lockFPS; }
		inline const float GetElapsedTime(void) { return _timeElapsed; }
		inline const unsigned long GetFPS(void) { return _frameRate; }
		inline const unsigned long GetWorldTime(void) { return _worldTime; }

		TimeManagent(void);
		~TimeManagent(void);
	};
}

