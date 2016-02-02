#ifndef __AGE_STATICSCENEOBJECT_HEADER__
#define __AGE_STATICSCENEOBJECT_HEADER__

#include "Renderable.h"
#include "AGEMeshImporter.h"
#include "PhysicsEngine.h"
#include "SceneObject.h"

namespace AGE
{
	class AGE_EXPORT StaticSceneObject : public SceneObject
	{
	public:
		StaticSceneObject(const char* rp)
		{

			AGEMeshImporter importer;
			Renderable* r = importer.LoadFromFile(rp, true);
			this->SetRenderable(r);
			InitPhysics();
		}

	protected:
		void InitPhysics();
	};
}

#endif
