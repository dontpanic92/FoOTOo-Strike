#ifndef __AGE_TIMER_HEADER__
#define __AGE_TIMER_HEADER__

#include <Windows.h>
#include "Singleton.h"
namespace AGE
{
	class Timer : public Singleton < Timer >
	{
	public:
		Timer() { SetThreadAffinityMask(GetCurrentThread(), 1); }

		void StartUp()
		{
			QueryPerformanceFrequency(&mFrequency);
			QueryPerformanceCounter(&mStartCount);
		}

		float GetTotalMilliSeconds()
		{
			LARGE_INTEGER count;
			QueryPerformanceCounter(&count);
			return float((count.QuadPart - mStartCount.QuadPart) * 1000 / float(mFrequency.QuadPart));
		}

	private:
		LARGE_INTEGER mFrequency;
		LARGE_INTEGER mStartCount;
	};
}

#endif