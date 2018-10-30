#pragma once
#include "UI.h"

namespace MHKLibrary
{
	class InputField : public UI
	{
	public:
		virtual HRESULT Init(void) = 0;
		virtual void Release(void) = 0;
		virtual void Update(void) = 0;
		virtual void Render(void) = 0;

		InputField(void);
		virtual ~InputField(void);
	};
}

