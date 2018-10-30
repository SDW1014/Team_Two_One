#pragma once
#include "Singleton.h"

namespace MHKLibrary
{
	enum IMAGE_ID
	{
		IMAGE_NULL = 0,
		IMAGE_TEST,
	};

	class Image;
	class AnimationManagent : public Singleton<AnimationManagent>
	{
	private:
		typedef std::map<IMAGE_ID, Image*> mapImageList;


	public:
		AnimationManagent(void);
		~AnimationManagent(void);
	};
}

