#ifndef __AGE_AGEMESHIMPORTER_HEADER__
#define __AGE_AGEMESHIMPORTER_HEADER__

#include "Renderable.h"
#include "Scene.h"
namespace AGE
{
	class AGEMeshImporter
	{
	public:

		struct AM1FileHeader
		{
			char	Magic[4];
			int		MaterialNum;
		};

		struct AM1MaterialHeader
		{
			char	TextureName[64];
			int		VertexNum;
		};

		SceneNode* LoadFromFile(const char* filename);
	};
}

#endif
