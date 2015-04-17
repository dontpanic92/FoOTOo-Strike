#ifndef __AGE_AGESKELETONIMPORTER_HEADER__
#define __AGE_AGESKELETONIMPORTER_HEADER__
#include "Skeleton.h"
#include "Def.h"

namespace AGE
{
	class AGE_EXPORT AGESkeletonAnimationImporter
	{
	public:
		struct AM1BoneFileHeader
		{
			char Magic[8];
			int BoneNum;
			int FrameNum;
		};

		struct AM1Frame
		{
			float Rotation[4];
			float Translation[3];
		};

		SkeletonAnimation* LoadFromeFile(Skeleton* skeleton, const char* filename);
	};
}

#endif
