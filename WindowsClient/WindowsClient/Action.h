#pragma once


namespace MHKLibrary
{
	class Action
	{
	private:
		float	_fElapsedTime;
		float	_fTotalTime;
		float	_fTravelTime;
		bool	_isPlay;
		bool	_isPause;
		bool	_isLoop;

	public:
		virtual void Update(void);
		
		float GetTotalTime(void) { return _fTotalTime; }
		float GetTravelTime(void) { return _fTravelTime; }
		bool IsPlay(void) { return _isPlay; }
		bool IsPause(void) { return _isPause; }
		bool IsLoop(void) { return _isLoop; }

		void Play(void);
		void Play(float travelTime);
		void Stop(void);
		void Pause(void);
		void Resume(void);

		void SetLoop(bool isLoop) { _isLoop = isLoop; }
		void SetTotalTime(float totaltime) { _fTotalTime = totaltime; }
		void SetTravelTime(float travelTime);

		Action(void);
		~Action(void);
	};
}

