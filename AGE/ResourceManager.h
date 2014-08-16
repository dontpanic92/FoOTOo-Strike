#ifndef __AGE_RESOURCEMANAGER_HEADER__
#define __AGE_RESOURCEMANAGER_HEADER__

#include "Singleton.h"
#include "Mesh.h"

namespace AGE
{
	class ResourceManager : public Singleton<ResourceManager>
	{
	public:


		Mesh* LoadMesh();

	private:
		ResourceManager(){}
		friend class Singleton<ResourceManager>;
	};
}

#endif
