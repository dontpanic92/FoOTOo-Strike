#ifndef __AGE_STATICSCENEOBJECT_HEADER__
#define __AGE_STATICSCENEOBJECT_HEADER__

#include "Renderable.h"
#include "AGEMeshImporter.h"
#include "PhysicsEngine.h"
#include "SceneNode.h"

namespace AGE
{
	class AGE_EXPORT PhysicsNode : public SceneNode
	{
	public:
		PhysicsNode(Renderable* r);

	protected:
		static void NewtonSetTransformCallback(const NewtonBody* const body, const dFloat* const matrix, int threadIndex);
		void InitPhysics();

		NewtonBody* mBody = nullptr;
	};
}

#endif
