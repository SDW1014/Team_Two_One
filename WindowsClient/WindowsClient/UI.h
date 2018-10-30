#pragma once
#include "GameNode.h"

namespace MHKLibrary
{
	class UI : public GameNode
	{
	private:
		RECT _rcCollision;

	public:
		virtual HRESULT Init(void) = 0;
		virtual void Release(void) = 0;
		virtual void Update(void) = 0;
		virtual void Render(void) = 0;

		bool IsCollision(POINT pt);
		bool IsCollision(RECT rc);
		void SetCollision(int left, int top, int right, int bottom);

		UI(void);
		virtual ~UI(void);
	};
}

