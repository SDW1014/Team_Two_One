#pragma once


namespace MHKLibrary
{
	//void Play(void);
	//void Resume(void);
	//void Resume(int frame);
	//void Pause(void);
	//void Pause(int frame);
	//void Stop(void);
	// bool _isPlay, _isLoop;
	struct Rect
	{
		int left, top, width, heigth;
	};

	class Image;
	class Animation
	{
	private:
		int _nCount, _nMax;
		Rect* _pSources;
		Image** _ppImages;

		HRESULT _ArrayFrame(int centerX, int centerY, Image* pImage, int frameX, int frameY, int count, va_list ap);

	public:
		HRESULT Init(int count);
		HRESULT Init_DefaultFrame(Image* pImage, int frameX, int frameY);
		HRESULT Init_DefaultFrame(int centerX, int centerY, Image* pImage, int frameX, int frameY);
		HRESULT Init_ArrayFrame(Image* pImage, int frameX, int frameY, int count, ...);
		HRESULT Init_ArrayFrame(int centerX, int centerY, Image* pImage, int frameX, int frameY, int count, ...);
		HRESULT Init_CoordinateFrame(Image* pImage, int frameX, int frameY, int start, int end);
		HRESULT Init_CoordinateFrame(int centerX, int centerY, Image* pImage, int frameX, int frameY, int start, int end);
		void Release(void);
		void Render(HDC hdc, int x, int y);
		void Render(HDC hdc, int count, int x, int y);
		
		// 이미지를 참조만 합니다.
		HRESULT AddImage(int count, Image* pImage);
		HRESULT AddImage(int count, int centerX, int centerY, Image* pImage);
		HRESULT AddImage(int count, Image* pImage, int x, int y, int width, int heigth);
		HRESULT AddImage(int count, int centerX, int centerY, Image* pImage, int x, int y, int width, int heigth);

		void SetCount(int count);
		void SetCount(float progress);
		void SetCountReverse(float progress);

		Animation(void);
		~Animation(void);
	};
}

