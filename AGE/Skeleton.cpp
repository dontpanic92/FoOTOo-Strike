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

Skeleton::Skeleton(uint vertexNum, uint boneNum)
{
	mVertexBoneBind = new VertexBoneBind[vertexNum];
	mVertexTransform = new Matrix4x4f[vertexNum];

	mVertexNum = vertexNum;
	mBoneNum = boneNum;

	mPlaying = false;
	mCurrentFrame = 0;
}

Skeleton::~Skeleton()
{
	for (uint i = 0; i < mAllBones.size(); i++) {
		delete mAllBones[i];
	}
	for (std::map<std::string, SkeletonAnimation*>::iterator it = mAnimations.begin(); it != mAnimations.end(); it++) {
		delete it->second;
	}

	delete[] mVertexBoneBind;
	delete[] mVertexTransform;
}

Bone* Skeleton::FindBone(int ID)
{
	for (uint i = 0; i < mAllBones.size(); i++) {
		if (mAllBones[i]->GetID() == ID)
			return mAllBones[i];
	}
	return NULL;
}

Bone* Skeleton::AddBone(int ID, int parentID)
{

	//Bone* bone = FindBone(ID);
	//if (bone == NULL) {
	Bone* bone = new Bone(ID);
	bone->SetInternalID(mAllBones.size());
	mAllBones.push_back(bone);
	//}
	bone->SetParentID(parentID);

	return bone;
}

void Skeleton::BuildTree()
{
	for (uint i = 0; i < mAllBones.size(); i++) {
		int id = mAllBones[i]->GetID();
		if (id == -1)
			continue;
		Bone* parentBone = FindBone(mAllBones[i]->GetID());
		mAllBones[i]->SetParent(parentBone);
		parentBone->AddChild(mAllBones[i]);
	}
}

void Skeleton::SetBoneBindPoseMatrixInv(int ID, float matrix[16])
{
	Bone* bone = FindBone(ID);
	bone->SetBindPoseMatrixInv(matrix);
}

/*void Skeleton::BindVertex(int vertex, int boneID[4], float weight[4])
{
	for (int i = 0; i < 4; i++) {
		mVertexBoneBind[vertex].Bones[i] = FindBone(boneID[i]);
		mVertexBoneBind[vertex].weight[i] = weight[i];
	}
}
*/

void Skeleton::StartPlay(const char* name, bool loop)
{
	mPlaying = false;
	mCurrentAnimation = mAnimations.find(name);
	if (mCurrentAnimation == mAnimations.end())
		return;
	mTime = 0;
	mCurrentFrame = 0;
	mPlaying = true;
	mLoop = loop;
}

void Skeleton::Update(float time)
{
	if (!mPlaying)
		return;
	//printf("%f %f %f\n", time, mStartTime, mCurrentAnimation->second->GetFrame(mCurrentFrame)->Time);
	mTime += time;

	SkeletonAnimation* currentAnim = mCurrentAnimation->second;

	if (mTime < currentAnim->GetFrame(mCurrentFrame)->Time)
		return;
	for (uint i = 0; i < mRenderable->GetNumberOfRenderObjects(); i++) {
		Mesh* mesh = mRenderable->GetRenderObject(i)->Mesh;
		const Mesh::SkeletonData* skeletonData = mesh->GetSkeletonData();
		Mesh::Vertex* vertex = mesh->GetVertexData();
		const Mesh::Vertex* vertexBindPose = mesh->GetVertexBindPose();
		for (uint j = 0; j < mesh->GetNumberOfVertex(); j++) {
			//if (mVertexBoneBind[i].Bones[0] == 0)
			//	continue;
			//if (mVertexBoneBind[i].Bones[0]->GetID() != 2)
			//continue;
			//uint id = mVertexBoneBind[i].Bones[0]->GetInternalID();
			//float weight = mVertexBoneBind[i].weight[0];
			//if (mVertexBoneBind[i].Bones[0] == 0)
			//continue;
			Bone* bone = FindBone(skeletonData[j].BoneID[0]);
			if (!bone)
				continue;
			uint id = bone->GetInternalID();
			float weigt = skeletonData[j].weight[0];
			Matrix4x4f trans = bone->GetBindPoseMatrixInv() * currentAnim->GetFrame(mCurrentFrame)->Transforms[id].Matrix;
			Vector3f v = vertexBindPose[j].Position;
			v = v * trans;
			memcpy(vertex[j].Position, v, sizeof(float) * 3);
			//v = vertexBindPose[j].Normal;
			//v = v * trans;
			//memcpy(vertex[j].Normal, v, sizeof(float) * 3);
		}

		mRenderable->GetRenderObject(i)->Update();
		//mVertexTransform[i] = mVertexBoneBind[i].Bones[0]->GetBindPoseMatrixInv() * mAnimations["default"]->GetFrame(mCurrentFrame)->Transforms[id].Matrix;
	}

	//mRenderable->UpdateSkinnedVertex();
	float t2 = currentAnim->GetFrame(currentAnim->GetFrameNum() - 1)->Time;
	while (mTime > t2)
		mTime -= t2;

	mCurrentFrame = (mCurrentFrame + 1);
	if (mCurrentFrame >= currentAnim->GetFrameNum()) {
		if (mLoop) {
			mCurrentFrame %= currentAnim->GetFrameNum();
		} else {
			mPlaying = false;
		}
	}
}


SkeletonAnimation::SkeletonAnimation(Skeleton* skeleton, uint frameNum)
{
	mFrameNum = frameNum;
	mFrames = new Frame[frameNum];
	mSkeleton = skeleton;
}

SkeletonAnimation::~SkeletonAnimation()
{
	delete[] mFrames;
}

void SkeletonAnimation::SetFrame(uint frame, float time, Skeleton::BoneTransform* transform)
{
	mFrames[frame].Transforms = transform;
	mFrames[frame].Time = time;
}

