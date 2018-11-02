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
		Image*			_pImg;
		Action*			_pAction;
		Animation*		_pAnimation;
		TCHAR			_szText[128];
	public:
		virtual HRESULT Init(void);
		virtual void Release(void);
		virtual void Update(void);
		virtual void Render(void);

		TestScene(void);
		virtual ~TestScene(void);
	};
}