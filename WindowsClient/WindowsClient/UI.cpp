#include "stdafx.h"
#include "UI.h"

namespace MHKLibrary
{
	UI::UI(void)
	{
	}

	UI::~UI(void)
	{
	}


	bool UI::IsCollision(LPPOINT lppt)
	{
		return PtInRect(&_rcCollision, *lppt);
	}
	bool UI::IsCollision(LPRECT lprc)
	{
		return (!EqualRect(&_rcCollision, lprc));
	}
	void UI::SetCollision(int left, int top, int right, int bottom)
	{
	}
}
