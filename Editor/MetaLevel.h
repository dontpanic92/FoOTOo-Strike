#ifndef __EDITOR_LEVEL_HEADER__
#define __EDITOR_LEVEL_HEADER__

#include <AGE.h>

enum class PrimitiveType
{
	Plane,
	IcoSphere
};

class RenderWidget;
class EScene;
class MetaLevel : public AGE::Level
{
public:
	MetaLevel(RenderWidget* widget);
	~MetaLevel() { ShutDown(); }

	bool StartUp() override;
	void ShutDown() override;

	void LoadScene(EScene* scene);
	void UnloadScene();

	AGE::Scene* GetScene() override { return mScene; }

	bool Update(float time) override;

	void RotateCamera(int deltaX, int deltaY);
	void AdjustDistance(int adjust);

	void AddPrimitive(PrimitiveType type);

private:
	AGE::Scene* mScene = 0;

	AGE::SceneNode* mPlaneObject = nullptr;
	AGE::Renderable* mPlane = nullptr;
	RenderWidget* mWidget = nullptr;
};

inline MetaLevel* GetMetaLevel()
{
	return AGE::GetLevel<MetaLevel>();
}

#endif
