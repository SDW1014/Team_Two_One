#pragma once
#include "Scene.h"

namespace MHKLibrary
{
	class Image;
	class Action;
	class Animation;
	class TestScene : public Scene
	{
	private:
		TCHAR			_szText1[128];
		TCHAR			_szText2[128];
		Image*			_pImg;
		Action*			_pAction;
		Animation*		_pAnimation;
		int _keyDownCount, _keyUpCount;
		bool _isStayKey, _isToggleKey;

	public:
		virtual HRESULT Init(void);
		virtual void Release(void);
		virtual void Update(void);
		virtual void Render(void);

		TestScene(void);
		virtual ~TestScene(void);
	};
}