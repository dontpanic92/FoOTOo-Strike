#ifndef __AGE_GAMELOGICIMP_HEADER__
#define __AGE_GAMELOGICIMP_HEADER__

#include <AGE.h>

using namespace OIS;
using namespace AGE;
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

#endif