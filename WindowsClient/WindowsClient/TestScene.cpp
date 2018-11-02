#include "stdafx.h"
#include "TestScene.h"

#include "Image.h"
#include "Action.h"
#include "Animation.h"

namespace MHKLibrary
{

	TestScene::TestScene(void)
		: _szText1("")
		, _pImg(nullptr)
		, _pAction(nullptr)
		, _pAnimation(nullptr)
		, _keyDownCount(0)
		, _keyUpCount(0)
		, _isStayKey(false)
		, _isToggleKey(false)
	{
	}

	TestScene::~TestScene(void)
	{
	}


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
		wsprintf(_szText1, "WorldTime : %d sec", TIME_MANAGENT->GetWorldTime());
		KEY_MANAGENT->OnceKeyCheck(VK_SPACE);
		if (KEY_MANAGENT->IsOnceKeyDown(VK_SPACE)) _keyDownCount++;
		if (KEY_MANAGENT->IsOnceKeyUp(VK_SPACE)) _keyUpCount++;
		if (KEY_MANAGENT->IsStayKeyDown(VK_SPACE)) _isStayKey = true;
		else _isStayKey = false;
		if (KEY_MANAGENT->IsToggleKey(VK_SPACE)) _isToggleKey = true;
		else _isToggleKey = false;

		wsprintf(_szText2, "DownCount : %d, UpCount : %d, StayKey : %s, ToggleKey : %s", _keyDownCount, _keyUpCount, _isStayKey ? "True": "False", _isToggleKey ? "True" : "False");
	}

	void TestScene::Render(void)
	{
		//if (_pImg) _pImg->Render(GetMemDC(), 20, 20);
		if (_pAnimation) _pAnimation->Render(GetMemDC(), 40, 40);

		SetTextColor(GetMemDC(), RGB(255, 255, 0));
		TextOut(GetMemDC(), 0, 0, _szText1, strlen(_szText1));

		SetTextColor(GetMemDC(), RGB(255, 255, 255));
		TextOut(GetMemDC(), 20, 150, _szText2, strlen(_szText2));
	}
}
