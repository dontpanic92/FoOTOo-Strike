#ifndef __AGE_GAMELOGIC_HEADER__
#define __AGE_GAMELOGIC_HEADER__

#include "InputEngine.h"

namespace AGE
{

	class Skeleton;
	class GameLogic
	{
	public:
		virtual ~GameLogic() {}

		virtual void StartUp() = 0;
		virtual bool Update(float time) = 0;
	};
}

#endif