#include "SceneObject.h"
#include "PhysicsEngine.h"
#include "Renderable.h"
using namespace AGE;

Renderable* SceneObject::SetRenderable(Renderable* r)
{
	Renderable* old = mRenderable;
	mRenderable = r;
	r->SetParent(this);

	if (old == r) {
		return nullptr;
	} else if (old) {
		old->SetParent(nullptr);
	}
	return old;
}
