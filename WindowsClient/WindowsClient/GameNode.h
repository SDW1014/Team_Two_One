#pragma once

namespace MHKLibrary
{
	class GameNode
	{
	private:

	public:
		virtual HRESULT Init(void) = 0;
		virtual void Release(void) = 0;
		virtual void Update(void) = 0;
		virtual void Render(void) = 0;

		GameNode(void) {};
		virtual ~GameNode(void) {};
	};
}