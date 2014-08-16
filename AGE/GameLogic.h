#ifndef __AGE_GAMELOGIC_HEADER__
#define __AGE_GAMELOGIC_HEADER__

namespace AGE
{
	class GameLogic
	{
	public:
		virtual ~GameLogic(){}

		virtual bool Update(float time) = 0;
	};

	class GameLogicImp : public GameLogic
	{
	public:
		bool Update(float time);
	};
}

#endif