#include "stdafx.h"
#include "Action.h"
#include "TimeManagent.h"

namespace MHKLibrary
{
	Action::Action(void)
		: _isPlay(false)
		, _isLoop(false)
		, _isPause(false)
		, _fElapsedTime(0.0f)
		, _fTotalTime(0.0f)
		, _fTravelTime(0.0f)
	{
	}

	Action::~Action(void)
	{
	}

	//이 함수를 매 프레임마다 실행되어서 새로고침 해야된다.
	void Action::Update(void)
	{
		if (_isPlay && (!_isPause))			//재생중이고 정지상태가 아닐때
		{
			//시간 새로고침
			_fElapsedTime = _fElapsedTime + TIME_MANAGENT->GetElapsedTime();
			_fTravelTime = _fElapsedTime / _fTotalTime;

			if (1.0f <= _fTravelTime)		// 재생이 끝났을 경우
			{
				if (_isLoop) Play(_fElapsedTime - _fTotalTime);
				else Stop();
			}
		}
	}

	void Action::Play(void)
	{
		Play(0.0f);
	}

	void Action::Play(float travelTime)
	{
		_fElapsedTime = 0.0f;
		_fTravelTime = (1.0f > travelTime ? travelTime : 0.0f);
		_isPlay = true;
		_isPause = false;
	}

	void Action::Stop(void)
	{
		_fElapsedTime = 0.0f;
		_fTravelTime = 0.0f;
		_isPlay = false;
	}

	void Action::Pause(void)
	{
		_isPause = true;
	}

	void Action::Resume(void)
	{
		_isPause = false;
	}

	// _fElapsedTime를 _fTravelTime 맞게 조절하고 _fTravelTime 수정한다.
	void Action::SetTravelTime(float travelTime)
	{
		_fElapsedTime = _fTotalTime * travelTime;
		_fTravelTime = travelTime;
	}
}
