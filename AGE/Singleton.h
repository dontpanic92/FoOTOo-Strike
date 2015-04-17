#ifndef __AGE_SINGLETON_HEADER__
#define __AGE_SINGLETON_HEADER__
#include "Def.h"

namespace AGE
{
	class AGE_EXPORT NonCopyable
	{
	protected:
		NonCopyable() {}
	private:
		NonCopyable & operator=(const NonCopyable&) = delete;
		NonCopyable(const NonCopyable&) = delete;
	};

	template<class T>
	class Singleton : private NonCopyable
	{
	public:
		virtual ~Singleton() {}
		static T* GetInstance()
		{
			static T sInstance;
			return &sInstance;
		}

	protected:
		Singleton() {};
	};
}

#endif
