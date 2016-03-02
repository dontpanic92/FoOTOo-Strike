#ifndef __EDITOR_LEVEL_HEADER__
#define __EDITOR_LEVEL_HEADER__

#include <AGE.h>
#include "EScene.h"

enum class PrimitiveType
{
	Plane,
	IcoSphere
};

class RenderWidget;
class EditorLevel : public AGE::Level
{
public:
	EditorLevel(RenderWidget* widget);
	~EditorLevel() { ShutDown(); }

	bool StartUp() override;
	void ShutDown() override;

	void SetScene(EScene* scene);

	EScene* GetScene() override { return mScene; }

	bool Update(float time) override;

	void RotateCamera(int deltaX, int deltaY);
	void AdjustDistance(int adjust);

	void AddPrimitive(PrimitiveType type);
	void AddSceneNode(AGE::Renderable* r);
	void AddPhysicsNode(AGE::Renderable* r);

private:
	EScene* mScene = nullptr;

	AGE::SceneNode* mPlaneObject = nullptr;
	AGE::Renderable* mPlane = nullptr;
	RenderWidget* mWidget = nullptr;
};

inline EditorLevel* GetEditorLevel()
{
	return AGE::GetLevel<EditorLevel>();
}

#endif
