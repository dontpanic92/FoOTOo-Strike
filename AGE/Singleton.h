#ifndef __AGE_SINGLETON_HEADER__
#define __AGE_SINGLETON_HEADER__

namespace AGE
{
	class NonCopyable
	{
	protected:
		NonCopyable(){}
	private:
		NonCopyable & operator=(const NonCopyable&){};
		NonCopyable(const NonCopyable&){};
	};

	template<class T>
	class Singleton : private NonCopyable
	{
	public:
		virtual ~Singleton(){}
		static T* GetInstance() {
			static T sInstance;
			return &sInstance;
		}

	protected:
		Singleton(){};
	};
}

#endif
