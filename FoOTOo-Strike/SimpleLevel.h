#ifndef __FOOTOOSTRIKE_SIMPLELEVEL_HEADER__
#define __FOOTOOSTRIKE_SIMPLELEVEL_HEADER__

#include <AGE.h>
#include "Actor.h"

using namespace AGE;

class SimpleLevel : public Level, public OIS::MouseListener, public OIS::KeyListener
{
public:
	bool StartUp() override;
	void ShutDown() override;

	Scene* GetScene() override { return mScene; }

	bool Update(float time) override;

	void ProcessMouse(const OIS::MouseState &state);

	void InitPhysics(Renderable *r1);

	~SimpleLevel(){ ShutDown(); }

	bool keyPressed(const KeyEvent &arg) { return false; }
	bool keyReleased(const KeyEvent &arg) { return false; }

	bool mouseMoved(const MouseEvent &arg) { return true; }
	bool mousePressed(const MouseEvent &arg, MouseButtonID id) { return true; }
	bool mouseReleased(const MouseEvent &arg, MouseButtonID id) { return true; }
private:
	Scene* mScene = 0;

	SceneNode* CameraNode = 0;

	Actor * actor = 0;

	AIActor* leet = 0;
};


#endif
