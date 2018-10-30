#include "stdafx.h"
#include "TimeManagent.h"
#include "TestScene.h"

#include "Image.h"

namespace MHKLibrary
{
	HRESULT TestScene::Init(void)
	{
		if (!_pImg) _pImg = new Image;
		//_pImg->Init(100, 100, RGB(255, 255, 255));
		_pImg->Init("Resources/Images/태극기.bmp");
		_pImg->SaveBitmap("Resources/Images/복사본.bmp");
		return S_OK;
	}

	void TestScene::Release(void)
	{
		SAFE_RELEASE_DELETE(_pImg);
	}

	void TestScene::Update(void)
	{
		wsprintf(_szText, "WorldTime : %d sec", TIME_MANAGENT->GetWorldTime());
	}

	void TestScene::Render(void)
	{
		if (_pImg) _pImg->Render(GetMemDC(), 20, 20);

		SetTextColor(GetMemDC(), RGB(255, 0, 255));
		TextOut(GetMemDC(), 0, 0, _szText, strlen(_szText));
	}

	TestScene::TestScene(void)
		: _pImg(nullptr)
		, _szText("")
	{
	}


	TestScene::~TestScene(void)
	{
	}
}
