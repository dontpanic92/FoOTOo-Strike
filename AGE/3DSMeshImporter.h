#ifndef __AGE_3DSMESHIMPORTER_HEADER__
#define __AGE_3DSMESHIMPORTER_HEADER__

#include "../3rd/lib3ds/include/lib3ds.h"

#include "Scene.h"

namespace AGE
{
	class AGE3DSMeshImporter
	{
	public:
		
		Scene* LoadSceneFromFile();

	private:
		void ProcessNode(Lib3dsFile* file, Lib3dsNode* node, Scene* scene);

		void ProcessMesh(Lib3dsFile* file, Lib3dsMesh* mesh, Scene* scene);

		//unsigned int m_TotalFaces;
	};
}

#endif