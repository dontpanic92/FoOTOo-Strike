#include "AGESkeletonAnimationImporter.h"

using namespace AGE;

SkeletonAnimation* AGESkeletonAnimationImporter::LoadFromeFile(Skeleton* skeleton, const char* filename)
{
	FILE* filp;
	fopen_s(&filp, filename, "rb");

	if (!filp)
		return NULL;


	AM1BoneFileHeader fileHeader;
	fread(&fileHeader, 1, sizeof(fileHeader), filp);

	SkeletonAnimation* animation = new SkeletonAnimation(skeleton, fileHeader.FrameNum);

	for (int i = 0; i < fileHeader.FrameNum; i++) {
		Skeleton::BoneTransform* boneTransform = new Skeleton::BoneTransform[fileHeader.BoneNum];

		float m[16], t;
		for (int j = 0; j < fileHeader.BoneNum; j++) {
			fread(&t, sizeof(t), 1, filp);
			fread(m, sizeof(m), 1, filp);
			memcpy(boneTransform[j].Matrix, m, sizeof(m));
		}
		animation->SetFrame(i, t, boneTransform);
	}


	fclose(filp);

	return animation;
}