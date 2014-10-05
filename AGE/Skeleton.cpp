#include "Skeleton.h"
#include "Renderable.h"

#include <map>
using namespace AGE;


/*void SkeletonAnimation::SetFrame(uint frame, const Vector3f& translation, const Quaternion& rotation) {
	mFrames[frame].Rotation = rotation;
	mFrames[frame].Translation = translation;
	mFrames[frame].BuildMatrix();
}


void Bone::SetBindPoseTranslation(float x, float y, float z) {
	mMatrix[3][0] = x;
	mMatrix[3][1] = y;
	mMatrix[3][2] = z;
}

void Bone::SetBindPoseRotation(float x, float y, float z, float w) {
	Quaternion q(x, y, z, w);
	Matrix4x4f m = q.ToRotationMatrix();

	m[3][0] = mMatrix[3][0];
	m[3][1] = mMatrix[3][1];
	m[3][2] = mMatrix[3][2];

	mMatrix = m;
}*/

Skeleton::Skeleton(uint vertexNum, uint boneNum){
	mVertexBoneBind = new VertexBoneBind[vertexNum];
	mVertexTransform = new Matrix4x4f[vertexNum];

	mVertexNum = vertexNum;
	mBoneNum = boneNum;

	mPlaying = false;
	mCurrentFrame = 0;
}

Skeleton::~Skeleton() {
	for (uint i = 0; i < mAllBones.size(); i++) {
		delete mAllBones[i];
	}
	for (std::map<std::string, SkeletonAnimation*>::iterator it = mAnimations.begin(); it != mAnimations.end(); it++) {
		delete it->second;
	}

	delete[] mVertexBoneBind;
	delete[] mVertexTransform;
}

Bone* Skeleton::FindBone(int ID) {
	for (uint i = 0; i < mAllBones.size(); i++) {
		if (mAllBones[i]->GetID() == ID)
			return mAllBones[i];
	}
	return NULL;
}

Bone* Skeleton::AddBone(int ID, int parentID) {

	//Bone* bone = FindBone(ID);
	//if (bone == NULL) {
	Bone* bone = new Bone(ID);
	bone->SetInternalID(mAllBones.size());
	mAllBones.push_back(bone);
	//}
	bone->SetParentID(parentID);

	return bone;
}

void Skeleton::BuildTree() {
	for (uint i = 0; i < mAllBones.size(); i++) {
		int id = mAllBones[i]->GetID();
		if (id == -1)
			continue;
		Bone* parentBone = FindBone(mAllBones[i]->GetID());
		mAllBones[i]->SetParent(parentBone);
		parentBone->AddChild(mAllBones[i]);
	}
}

void Skeleton::SetBoneBindPoseMatrixInv(int ID, float matrix[16]) {
	Bone* bone = FindBone(ID);
	bone->SetBindPoseMatrixInv(matrix);
}

void Skeleton::BindVertex(int vertex, int boneID[4], float weight[4]) {
	for (int i = 0; i < 4; i++) {
		mVertexBoneBind[vertex].Bones[i] = FindBone(boneID[i]);
		mVertexBoneBind[vertex].weight[i] = weight[i];
	}
}

void Skeleton::Update() {
	for (uint i = 0; i < mVertexNum; i++) {
		if (mVertexBoneBind[i].Bones[0] == 0)
			continue;
		//if (mVertexBoneBind[i].Bones[0]->GetID() != 2)
			//continue;
		uint id = mVertexBoneBind[i].Bones[0]->GetInternalID();
		float weight = mVertexBoneBind[i].weight[0];

		
		mVertexTransform[i] = mVertexBoneBind[i].Bones[0]->GetBindPoseMatrixInv() * mAnimations["default"]->GetFrame(mCurrentFrame)->Transforms[id].Matrix;
	}

	mRenderable->UpdateSkinnedVertex();

	mCurrentFrame = (mCurrentFrame + 1) % mAnimations["default"]->GetFrameNum();
}


SkeletonAnimation::SkeletonAnimation(Skeleton* skeleton, uint frameNum) {
	mFrameNum = frameNum;
	mFrames = new Frame[frameNum];
	mSkeleton = skeleton;
}

SkeletonAnimation::~SkeletonAnimation() {
	delete[] mFrames;
}

void SkeletonAnimation::SetFrame(uint frame, Skeleton::BoneTransform* transform) {
	mFrames[frame].Transforms = transform;
}

