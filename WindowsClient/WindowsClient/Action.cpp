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

	//�� �Լ��� �� �����Ӹ��� ����Ǿ ���ΰ�ħ �ؾߵȴ�.
	void Action::Update(void)
	{
		if (_isPlay && (!_isPause))			//������̰� �������°� �ƴҶ�
		{
			//�ð� ���ΰ�ħ
			_fElapsedTime = _fElapsedTime + TIME_MANAGENT->GetElapsedTime();
			_fTravelTime = _fElapsedTime / _fTotalTime;

			if (1.0f <= _fTravelTime)		// ����� ������ ���
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

	// _fElapsedTime�� _fTravelTime �°� �����ϰ� _fTravelTime �����Ѵ�.
	void Action::SetTravelTime(float travelTime)
	{
		_fElapsedTime = _fTotalTime * travelTime;
		_fTravelTime = travelTime;
	}
}
