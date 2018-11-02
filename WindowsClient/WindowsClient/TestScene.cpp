#include "stdafx.h"
#include "TimeManagent.h"
#include "TestScene.h"

#include "Image.h"
#include "Action.h"
#include "Animation.h"

namespace MHKLibrary
{
	HRESULT TestScene::Init(void)
	{
		_pImg = new Image;
		_pImg->Init("Resources/Images/Ultralisk.bmp");
		_pImg->TransColor(RGB(0, 0, 0));
		_pAnimation = new Animation;
		_pAnimation->Init_ArrayFrame(_pImg, 16, 16, 8, 0, 16, 32, 48, 64, 80, 96, 112);

		_pAction = new Action;
		_pAction->SetTotalTime(0.5f);
		_pAction->SetLoop(true);
		_pAction->Play();

		return S_OK;
	}

	void TestScene::Release(void)
	{
		SAFE_DELETE(_pAction);
		SAFE_RELEASE_DELETE(_pAnimation);
		SAFE_RELEASE_DELETE(_pImg);
	}

	void TestScene::Update(void)
	{
		if (_pAction)
		{
			_pAction->Update();
			if (_pAnimation)
			{
				_pAnimation->AnimationLerp(_pAction->GetTravelTime());
			}

		}
		wsprintf(_szText, "WorldTime : %d sec", TIME_MANAGENT->GetWorldTime());
	}

	void TestScene::Render(void)
	{
		//if (_pImg) _pImg->Render(GetMemDC(), 20, 20);
		if (_pAnimation) _pAnimation->Render(GetMemDC(), 40, 40);

		SetTextColor(GetMemDC(), RGB(255, 0, 255));
		TextOut(GetMemDC(), 0, 0, _szText, strlen(_szText));
	}

	TestScene::TestScene(void)
		: _szText("")
		, _pImg(nullptr)
		, _pAction(nullptr)
		, _pAnimation(nullptr)
	{
	}


	TestScene::~TestScene(void)
	{
	}
}
