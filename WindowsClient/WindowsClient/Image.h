#pragma once

namespace MHKLibrary
{
	class Image
	{
	private:
		HDC _hMemDC;

		BITMAP _bitmap;
		HBITMAP _hBitmap;

		COLORREF _transColor;
		BOOL _isTrans;

		HRESULT _ChangeBitmap(HBITMAP hBitmap);

	public:
		HRESULT Init(int width, int height);
		HRESULT Init(int width, int height, COLORREF color);
		HRESULT Init(const TCHAR* szBitmapName);
		void Release(void);

		//AlphaRender
		void AlphaRender(HDC hSubDC, BYTE alpha, HDC hDestDC);
		void AlphaRender(HDC hSubDC, BYTE alpha, HDC hDestDC, int destX, int destY);
		void AlphaRender(HDC hSubDC, BYTE alpha, HDC hDestDC, int sourX, int sourY, int sourWidth, int sourHeight);
		void AlphaRender(HDC hSubDC, BYTE alpha, HDC hDestDC, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);

		//CenterRadianRender
		void CenterRadianRender(int centerX, int centerY, float radian, HDC hdc);
		void CenterRadianRender(int centerX, int centerY, float radian, HDC hdc, int destX, int destY);
		void CenterRadianRender(int centerX, int centerY, float radian, HDC hdc, int sourX, int sourY, int sourWidth, int sourHeight);
		void CenterRadianRender(int centerX, int centerY, float radian, HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);

		//DynamicRender
		void DynamicRender(HDC hdc, int destX, int destY, int destWidth, int destHeight);
		
		//LoopRender
		void LoopRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY);

		//RadianRender
		void RadianRender(float radian, HDC hdc);
		void RadianRender(float radian, HDC hdc, int destX, int destY);
		void RadianRender(float radian, HDC hdc, int sourX, int sourY, int sourWidth, int sourHeight);
		void RadianRender(float radian, HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);

		//Render
		void Render(HDC hdc);
		void Render(HDC hdc, int destX, int destY);
		void Render(HDC hdc, int sourX, int sourY, int sourWidth, int sourHeight);
		void Render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);


		void ColorNess(COLORREF color);

		void DeleteBitmap(void);

		HDC* GetPHDC(void) { return &_hMemDC; }
		HDC GetMemDC(void) { return _hMemDC; }
		int GetHeight(void) { return _bitmap.bmHeight; }
		int GetWidth(void) { return _bitmap.bmWidth; }

		bool IsBitmap(void) { return _hBitmap; }

		HRESULT SaveBitmap(LPCSTR szFileName);

		void TransColor(BOOL isTran);
		void TransColor(COLORREF transColor);

		Image(void);
		~Image(void);
	};
}

