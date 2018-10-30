#pragma once

namespace MHKLibrary
{
	template <class T>
	class Singleton
	{
	private:
		static T _instance;

	public:
		static T* GetInstance(void);

	protected:
		Singleton(void) {}
		~Singleton(void) {}
	};

	template<class T>
	inline T * Singleton<T>::GetInstance(void)
	{
		static T _instance;
		return &_instance;
	}
}