#ifndef __AGE_AGEMESHIMPORTER_HEADER__
#define __AGE_AGEMESHIMPORTER_HEADER__

#include "Renderable.h"
#include "Scene.h"
#include "Def.h"

namespace AGE
{
	class AGE_EXPORT AGEMeshImporter
	{
	public:

		struct AM1FileHeader
		{
			char	Magic[4];
			int		MaterialNum;
			int		BoneNum;
			int		VertexNum;
		};

		struct AM1MaterialHeader
		{
			char	TextureName[64];
			int		VertexNum;
		};

		struct AM1Bone
		{
			int BoneID;
			int ParentID;
			float BindPoseMatrixInv[16];
		};

		Renderable* LoadFromFile(const char* filename, bool is_static);
	};
}

#endif
