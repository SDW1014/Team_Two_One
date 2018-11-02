#include "stdafx.h"
#include "Image.h"
#include "Animation.h"

namespace MHKLibrary
{
	Animation::Animation(void)
		: _ppImages(nullptr)
		, _pSources(nullptr)
		, _nMax(0)
		, _nCount(0)
	{
	}

	Animation::~Animation(void)
	{
	}

	HRESULT Animation::_ArrayFrame(int centerX, int centerY, Image * pImage, int frameX, int frameY, int count, va_list ap)
	{
		if ((!pImage) || FAILED(Init(count))) return E_FAIL;

		int frameSizeX = pImage->GetWidth() / frameX;
		int frameSizeY = pImage->GetHeight() / frameY;
		for (size_t frame, i = 0; i < count; i++)
		{
			frame = va_arg(ap, int);
			if (SUCCEEDED(AddImage(i, centerX, centerY, pImage, frameSizeX * (frame % frameX), frameSizeY * (frame / frameX), frameSizeX, frameSizeY))) continue;
			va_end(ap);
			return E_FAIL;
		}
		return S_OK;
	}


	HRESULT Animation::Init(int max)
	{
		if (0 >= max) E_FAIL;

		Release();
		_pSources = new Rect[max];
		_ppImages = new Image*[max];

		if (!(_pSources && _ppImages)) return E_FAIL;
		for (size_t i = 0; i < max; i++) _ppImages[i] = nullptr;
		_nMax = max - 1;
		return S_OK;
	}

	HRESULT Animation::Init_DefaultFrame(Image * pImage, int frameX, int frameY)
	{
		if ((!pImage) || FAILED(Init(frameX * frameY))) return E_FAIL;

		int frameSizeX = pImage->GetWidth() / frameX;
		int frameSizeY = pImage->GetHeight() / frameY;
		for (size_t y = 0; y < frameY; y++)
		{
			for (size_t x = 0; x < frameX; x++)
			{
				if (SUCCEEDED(AddImage(x + y * frameX, pImage, frameSizeX * x, frameSizeY * y, frameSizeX, frameSizeY))) continue;
				return E_FAIL;
			}
		}
		return S_OK;
	}

	HRESULT Animation::Init_DefaultFrame(int centerX, int centerY, Image * pImage, int frameX, int frameY)
	{
		if ((!pImage) || FAILED(Init(frameX * frameY))) return E_FAIL;

		int frameSizeX = pImage->GetWidth() / frameX;
		int frameSizeY = pImage->GetHeight() / frameY;
		for (size_t y = 0; y < frameY; y++)
		{
			for (size_t x = 0; x < frameX; x++)
			{
				if (SUCCEEDED(AddImage(x + y * frameX, centerX, centerY, pImage, frameSizeX * x, frameSizeY * y, frameSizeX, frameSizeY))) continue;
				return E_FAIL;
			}
		}
		return S_OK;
	}

	HRESULT Animation::Init_ArrayFrame(Image* pImage, int frameX, int frameY, int count, ...)
	{
		va_list ap;
		va_start(ap, count);
		if (FAILED(_ArrayFrame(0, 0, pImage, frameX, frameY, count, ap)))
		{
			va_end(ap);
			return E_FAIL;
		}
		va_end(ap);
		return S_OK;
	}

	HRESULT Animation::Init_ArrayFrame(int centerX, int centerY, Image * pImage, int frameX, int frameY, int count, ...)
	{
		va_list ap;
		va_start(ap, count);
		if (FAILED(_ArrayFrame(centerX, centerY, pImage, frameX, frameY, count, ap)))
		{
			va_end(ap);
			return E_FAIL;
		}
		va_end(ap);
		return S_OK;
	}

	HRESULT Animation::Init_CoordinateFrame(Image * pImage, int frameX, int frameY, int start, int end)
	{
		return Init_CoordinateFrame(0, 0, pImage, frameX, frameY, start, end);
	}

	HRESULT Animation::Init_CoordinateFrame(int centerX, int centerY, Image * pImage, int frameX, int frameY, int start, int end)
	{
		if (!pImage) return E_FAIL;
		if (SUCCEEDED(Init(end - start + 1)))
		{
			int frameSizeX = pImage->GetWidth() / frameX;
			int frameSizeY = pImage->GetHeight() / frameY;
			for (size_t count = 0, frame = start; frame <= end; count++, frame++)
			{
				if (SUCCEEDED(AddImage(count, centerX, centerY, pImage, frameSizeX * (frame % frameX), frameSizeY * (frame / frameX), frameSizeX, frameSizeY))) continue;
				return E_FAIL;
			}
		}
		else
		{
			if (FAILED(Init(start - end + 1))) return E_FAIL;

			int frameSizeX = pImage->GetWidth() / frameX;
			int frameSizeY = pImage->GetHeight() / frameY;
			for (size_t count = 0, frame = start; frame >= end; count++, frame--)
			{
				if (SUCCEEDED(AddImage(count, centerX, centerY, pImage, frameSizeX * (frame % frameX), frameSizeY * (frame / frameX), frameSizeX, frameSizeY))) continue;
				return E_FAIL;
			}
		}
		return S_OK;
	}

	void Animation::Release(void)
	{
		SAFE_DELETE_ARRAY(_ppImages);
		SAFE_DELETE(_pSources);
		_nMax = 0;
	}

	void Animation::Render(HDC hdc, int x, int y)
	{
		Render(hdc, _nCount, x, y);
	}

	void Animation::Render(HDC hdc, int count, int x, int y)
	{
		if (0 > count || _nMax < count || (!_ppImages) || (!_ppImages[count])) return;
		_ppImages[count]->Render(hdc, x, y, _pSources[count].left, _pSources[count].top, _pSources[count].width, _pSources[count].heigth);
	}

	HRESULT Animation::AddImage(int count, Image * pImage)
	{
		return AddImage(count, pImage, 0, 0, pImage->GetWidth(), pImage->GetHeight());
	}

	HRESULT Animation::AddImage(int count, int centerX, int centerY, Image * pImage)
	{
		return AddImage(count, centerX, centerY, pImage, 0, 0, pImage->GetWidth(), pImage->GetHeight());
	}

	HRESULT Animation::AddImage(int count, Image * pImage, int x, int y, int width, int heigth)
	{
		return AddImage(count, 0, 0, pImage, x, y, width, heigth);
	}

	HRESULT Animation::AddImage(int count, int centerX, int centerY, Image * pImage, int x, int y, int width, int heigth)
	{
		if ((!pImage) || (0 > count) || (_nMax <= count)) return E_FAIL;
		_pSources[count] = { x - centerX, y - centerX, width, heigth };
		_ppImages[count] = pImage;
		return S_OK;
	}

	void Animation::SetCount(int count) { _nCount = count; }

	void Animation::AnimationLerp(float travelTime)
	{
		_nCount = ((1.0f > travelTime) ? int(_nMax * travelTime) : _nMax);
	}

	void Animation::AnimationLerpReverse(float travelTime)
	{
		_nCount = ((1.0f > travelTime) ? _nMax - int(_nMax * travelTime) : 0);
	}
}
