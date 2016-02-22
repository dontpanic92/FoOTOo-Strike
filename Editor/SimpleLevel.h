#ifndef __FOOTOOSTRIKE_SIMPLELEVEL_HEADER__
#define __FOOTOOSTRIKE_SIMPLELEVEL_HEADER__

#include <AGE.h>

enum class PrimitiveType
{
	Plane,
	IcoSphere
};

class RenderWidget;
class SimpleLevel : public AGE::Level
{
public:
	SimpleLevel(RenderWidget* widget);

	bool StartUp() override;
	void ShutDown() override;

	AGE::Scene* GetScene() override { return mScene; }

	bool Update(float time) override;

	void RotateCamera(int deltaX, int deltaY);
	void AdjustDistance(int adjust);

	void AddPrimitive(PrimitiveType type);

	~SimpleLevel(){ ShutDown(); }

private:
	AGE::Scene* mScene = 0;

	AGE::SceneNode* mPlaneObject = nullptr;
	AGE::Renderable* mPlane = nullptr;
	RenderWidget* mWidget = nullptr;
};


#endif
