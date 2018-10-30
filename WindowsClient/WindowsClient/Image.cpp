#include "stdafx.h"
#include "Image.h"

namespace MHKLibrary
{
	Image::Image(void)
		: _hMemDC(NULL)
		, _hBitmap(NULL)
		, _transColor(RGB(0, 0, 0))
		, _isTrans (FALSE)
	{
		ZeroMemory(&_bitmap, sizeof(BITMAP));
	}

	Image::~Image(void)
	{
	}

	//DC에 넣은 기존 bitmap을 제거하고 새로운 bitmap을 넣습니다.
	HRESULT Image::_ChangeBitmap(HBITMAP hBitmap)
	{
		if (!hBitmap) return E_FAIL;
		DeleteBitmap();
		GetObject(hBitmap, sizeof(BITMAP), &_bitmap);
		_hBitmap = (HBITMAP)SelectObject(_hMemDC, hBitmap);
		_isTrans = FALSE;
		return S_OK;
	}

	// DC를 생성하고 비트맵을 생성합니다.
	HRESULT Image::Init(int width, int height)
	{
		HDC hdc = GetDC(_hWnd);
		if (!_hMemDC) _hMemDC = CreateCompatibleDC(hdc);
		HBITMAP hBitmap = _hMemDC ? CreateCompatibleBitmap(hdc, width, height) : NULL;
		ReleaseDC(_hWnd, hdc);
		return _ChangeBitmap(hBitmap);
	}

	// DC를 생성하고 비트맵을 생성하고 색상으로 채웁니다.
	HRESULT Image::Init(int width, int height, COLORREF color)
	{
		if (FAILED(Init(width, height))) return E_FAIL;
		ColorNess(color);
		return S_OK;
	}

	// DC를 생성하고 비트맵을 불러옵니다.
	HRESULT Image::Init(const TCHAR*  szBitmapName)
	{
		if (!_hMemDC)
		{
			HDC hdc = GetDC(_hWnd);
			_hMemDC = CreateCompatibleDC(hdc);
			ReleaseDC(_hWnd, hdc);
		}

		return _ChangeBitmap(_hMemDC ? ((HBITMAP) LoadImage(_hInst, szBitmapName, IMAGE_BITMAP,
			0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE)) : NULL);
	}

	//bitmap을 제거하고 DC를 제거합니다.
	void Image::Release(void)
	{
		if (!_hMemDC) return;

		DeleteBitmap();

		DeleteDC(_hMemDC);
		_hMemDC = NULL;
	}

	void Image::AlphaRender(HDC hSubDC, BYTE alpha, HDC hDestDC)
	{
		AlphaRender(hSubDC, alpha, hDestDC, 0, 0, 0, 0, GetWidth(), GetHeight());
	}
	
	void Image::AlphaRender(HDC hSubDC, BYTE alpha, HDC hDestDC, int destX, int destY)
	{
		AlphaRender(hSubDC, alpha, hDestDC, destX, destY, 0, 0, GetWidth(), GetHeight());
	}
	
	void Image::AlphaRender(HDC hSubDC, BYTE alpha, HDC hDestDC, int sourX, int sourY, int sourWidth, int sourHeight)
	{
		AlphaRender(hSubDC, alpha, hDestDC, 0, 0, sourX, sourY, sourWidth, sourHeight);
	}
	
	void Image::AlphaRender(HDC hSubDC, BYTE alpha, HDC hDestDC, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
	{
		BLENDFUNCTION blendFunction;
		blendFunction.SourceConstantAlpha = alpha;
	
		if (_isTrans && (!hSubDC))
		{
			BitBlt(hSubDC, 0, 0, sourWidth, sourHeight, hDestDC, destX, destY, SRCCOPY);
		
			GdiTransparentBlt(hSubDC, 0, 0, sourWidth, sourHeight, _hMemDC, sourX, sourY, sourWidth, sourHeight, _transColor);
		
			AlphaBlend(hDestDC, destX, destY, sourWidth, sourHeight, hSubDC, 0, 0, sourWidth, sourHeight, blendFunction);
		}
		else
		{
			AlphaBlend(hDestDC, destX, destY, sourWidth, sourHeight, _hMemDC, sourX, sourY, sourWidth, sourHeight, blendFunction);
		}
	}

	void Image::DynamicRender(HDC hdc, int destX, int destY, int destWidth, int destHeight)
	{
		if (_isTrans)
		{
			GdiTransparentBlt(hdc, destX, destY, destWidth, destHeight, _hMemDC, 0, 0, GetWidth(), GetHeight(), _transColor);
		}
		else
		{
			StretchBlt(hdc, destX, destY, destWidth, destHeight, _hMemDC, 0, 0, GetWidth(), GetHeight(), SRCCOPY);
		}
	}

	void Image::Render(HDC hdc)
	{
		Render(hdc, 0, 0, 0, 0, GetWidth(), GetHeight());
	}

	void Image::Render(HDC hdc, int destX, int destY)
	{
		Render(hdc, destX, destY, 0, 0, GetWidth(), GetHeight());
	}

	void Image::Render(HDC hdc, int sourX, int sourY, int sourWidth, int sourHeight)
	{
		Render(hdc, 0, 0, sourX, sourY, sourWidth, sourHeight);
	}

	void Image::Render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
	{
		if (_isTrans)
		{
			GdiTransparentBlt(hdc, destX, destY, sourWidth,
				sourHeight, _hMemDC, sourX, sourY,
				sourWidth, sourHeight, _transColor);
		}
		else
		{
			BitBlt(hdc, destX, destY, sourWidth, sourHeight,
				_hMemDC, sourX, sourY, SRCCOPY);
		}
	}

	void Image::ColorNess(COLORREF color)
	{
		RECT rc = { 0, 0, GetWidth(), GetHeight() };
		HBRUSH hBrush = CreateSolidBrush(color);
		FillRect(_hMemDC, &rc, hBrush);
		DeleteObject((HBRUSH)hBrush);
	}

	//DC에 들어간 bitmap을 제거합니다.
	void Image::DeleteBitmap(void)
	{
		if (!_hBitmap) return;

		_hBitmap = (HBITMAP)SelectObject(_hMemDC, _hBitmap);
		DeleteObject(_hBitmap);
	}

	HRESULT Image::SaveBitmap(LPCSTR szFileName)
	{
		if (!_hMemDC) return E_FAIL;

		BITMAPINFOHEADER bmih;
		ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));
		bmih.biSize = sizeof(BITMAPINFOHEADER);
		bmih.biWidth = GetWidth();
		bmih.biHeight = GetHeight();
		bmih.biPlanes = _bitmap.bmPlanes;
		bmih.biBitCount = _bitmap.bmBitsPixel;
		bmih.biCompression = BI_RGB;
		bmih.biSizeImage = _bitmap.bmWidthBytes * _bitmap.bmHeight;

		_hBitmap = (HBITMAP)SelectObject(_hMemDC, _hBitmap);
		HDC hdc = GetDC(_hWnd);
		LPBYTE lpbits = new BYTE[bmih.biSizeImage];
		if (!lpbits)
		{
			ReleaseDC(_hWnd, hdc);
			_hBitmap = (HBITMAP)SelectObject(_hMemDC, _hBitmap);
			return E_FAIL;
		}
		GetDIBits(hdc, _hBitmap, 0, _bitmap.bmHeight, lpbits, (LPBITMAPINFO)&bmih, DIB_RGB_COLORS);
		ReleaseDC(_hWnd, hdc);
		_hBitmap = (HBITMAP)SelectObject(_hMemDC, _hBitmap);

		HANDLE hFile = CreateFile(szFileName, GENERIC_WRITE, NULL, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			delete[] lpbits;
			return E_FAIL;
		}

		BITMAPFILEHEADER bmfh;
		ZeroMemory(&bmfh, sizeof(BITMAPFILEHEADER));
		bmfh.bfType = 'MB';
		bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		bmfh.bfSize = bmfh.bfOffBits + bmih.biSizeImage;

		DWORD dwWritten = 0;
		WriteFile(hFile, &bmfh, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
		WriteFile(hFile, &bmih, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
		WriteFile(hFile, lpbits, bmih.biSizeImage, &dwWritten, NULL);
		CloseHandle(hFile);

		delete[] lpbits;

		return S_OK;
	}

	//색상을 제외할지 여부를 설정합니다.
	void Image::TransColor(BOOL isTran)
	{
		_isTrans = isTran;
	}

	//제외할 색상을 설정합니다.
	void Image::TransColor(COLORREF transColor)
	{
		_transColor = transColor;
		_isTrans = TRUE;
	}
}
