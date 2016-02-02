#ifndef __FOOTOOSTRIKE_SIMPLELEVEL_HEADER__
#define __FOOTOOSTRIKE_SIMPLELEVEL_HEADER__

#include <AGE.h>

using namespace AGE;

class SimpleLevel : public Level
{
public:
	bool StartUp() override;
	void ShutDown() override;

	Scene* GetScene() override { return mScene; }

	bool Update(float time) override;


	~SimpleLevel(){ ShutDown(); }

private:
	Scene* mScene = 0;
	SceneNode* ActorNode = 0;
	StaticSceneObject* ground = 0;
};


#endif
