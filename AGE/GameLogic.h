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


	using namespace OIS;
	class Renderable;
	class GameLogicImp : public GameLogic, public OIS::MouseListener, public OIS::KeyListener
	{
	public:
		void StartUp();
		void InitPhysics(Renderable *r1);

		bool Update(float time);
		void ProcessMouse(const MouseState &state);

		bool keyPressed(const KeyEvent &arg) { return false; }
		bool keyReleased(const KeyEvent &arg) { return false; }

		bool mouseMoved(const MouseEvent &arg);
		bool mousePressed(const MouseEvent &arg, MouseButtonID id) { return true; }
		bool mouseReleased(const MouseEvent &arg, MouseButtonID id) { return true; }
	};
}

#endif