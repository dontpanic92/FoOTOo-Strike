#ifndef __AGE_STATICSCENEOBJECT_HEADER__
#define __AGE_STATICSCENEOBJECT_HEADER__

#include "Renderable.h"
#include "AGEMeshImporter.h"
#include "PhysicsEngine.h"
#include "RenderableSceneObject.h"

namespace AGE
{
	class AGE_EXPORT StaticSceneObject : public RenderableSceneObject
	{
	public:
		StaticSceneObject(const char* rp)
		{

			AGEMeshImporter importer;
			Renderable* r = importer.LoadFromFile(rp, true);
			mRenderable = r;
			r->SetParent(this);
			InitPhysics();
		}

	protected:
		void InitPhysics();
	};
}

#endif
