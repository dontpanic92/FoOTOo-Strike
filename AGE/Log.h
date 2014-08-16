#ifndef __AGE_LOG_HEADER__
#define __AGE_LOG_HEADER__

#include "Singleton.h"

namespace AGE
{
	class Log : public Singleton<Log>
	{
	public:
		static void Error(const char* fmt);
	};
}


#endif
