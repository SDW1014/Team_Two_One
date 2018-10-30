#pragma once
#include "Scene.h"

namespace MHKLibrary
{
	class Image;
	class TestScene : public Scene
	{
	private:
		Image* _pImg;
		TCHAR _szText[128];
	public:
		virtual HRESULT Init(void);
		virtual void Release(void);
		virtual void Update(void);
		virtual void Render(void);

		TestScene(void);
		virtual ~TestScene(void);
	};
}