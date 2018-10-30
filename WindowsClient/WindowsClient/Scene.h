#pragma once
#include "GameNode.h"

namespace MHKLibrary
{
	class Scene : public GameNode
	{
	private:
		HDC* _phdc;

	public:
		virtual HRESULT Init(void) = 0;
		virtual void Release(void) = 0;
		virtual void Update(void) = 0;
		virtual void Render(void) = 0;


		HDC GetMemDC(void) { return *_phdc; }
		void SetPHDC(HDC* phdc) { _phdc = phdc; }

		Scene(void) {};
		virtual ~Scene(void) {};
	};
}
