#ifndef __MODELVIEWLELEVEL_HEADER__
#define __MODELVIEWLELEVEL_HEADER__

#include <AGE.h>

using namespace AGE;

class ModelViewLevel : public Level
{
public:
	bool StartUp() override;
	void ShutDown() override;

	Scene* GetScene() override { return mScene; }

	bool Update(float time) override;

	//bool Load(const char* path);

	~ModelViewLevel() { ShutDown(); }

private:
	Scene* mScene = 0;
	SceneNode* mSceneNode = 0;
	SceneObject* mSceneObject = 0;
};


#endif
