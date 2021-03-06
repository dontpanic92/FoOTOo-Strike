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

	bool Load(const char* path);

	void RotateCamera(int deltaX, int deltaY);

	void AdjustDistance(int adjust);

	~ModelViewLevel() { ShutDown(); }

private:
	Scene* mScene = nullptr;
	SceneObject* mSceneObject = nullptr;
	SceneObject* mPlaneObject = nullptr;
	Renderable* mPlane = nullptr;

	float mDistance = 10;
};


#endif
