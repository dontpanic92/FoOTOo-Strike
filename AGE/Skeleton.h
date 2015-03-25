#ifndef __AGE_SKELETON_HEADER__
#define __AGE_SKELETON_HEADER__
#include "Math.h"
#include <vector>
#include <map>

namespace AGE
{

	class Bone
	{
	public:

		Bone(int boneID)/* : mAnimation(0)*/ { mBoneID = boneID; mParentID = -1; }

		~Bone() { /*delete mAnimation;*/ }

		void AddChild(Bone* child) { mChildren.push_back(child); }

		void SetParent(Bone* parent) { mParent = parent; }

		void SetParentID(int parentID) { mParentID = parentID; }

		void SetInternalID(int internalID) { mInternalID = internalID; }

		int GetInternalID() { return mInternalID; }

		Bone* GetParent() { return mParent; }

		int GetID() { return mBoneID; }

		int GetParentID() { return mParentID; }

		Matrix4x4f GetTransformMatrix() { return mMatrix; }

		uint GetChildrenNum() { return mChildren.size(); }

		Bone* GetChild(uint id) { return mChildren[id]; }

		//void SetBindPoseTranslation(float x, float y, float z);

		//void SetBindPoseRotation(float x, float y, float z, float w);

		void SetBindPoseMatrixInv(const Matrix4x4f& matrix) { mBindPoseMatrixInv = matrix; }

		const Matrix4x4f& GetBindPoseMatrixInv() { return mBindPoseMatrixInv; }

		//void AddAnimation(BoneAnimation* animation) { mAnimation = animation; }

		//BoneAnimation* GetAnimation() { return mAnimation; }

	private:

		//BoneID is the Node ID in 3dsmax
		int mBoneID;
		int mParentID;

		//InternalID is the ID in current Skeleton's mAllBones
		int mInternalID;
		Bone* mParent;

		std::vector<Bone*> mChildren;
		//BoneAnimation* mAnimation;

		Matrix4x4f mBindPoseMatrixInv;
		Matrix4x4f mMatrix;
	};


	class Renderable;
	class SkeletonAnimation;
	class Skeleton
	{
	public:

		struct BoneTransform
		{
			//Vector3f Translation;
			//Quaternion Rotation;
			Matrix4x4f Matrix;

			BoneTransform() {}
			BoneTransform(float trans[3], float quat[4]) { BuildMatrix(trans, quat); }

			void BuildMatrix(float trans[3], float quat[4])
			{
				Quaternion q(quat);
				Matrix = q.ToRotationMatrix();
				Matrix[3][0] = trans[0];
				Matrix[3][1] = trans[1];
				Matrix[3][2] = trans[2];
			}
		};

		Skeleton(uint vertexNum, uint boneNum);
		~Skeleton();

		Bone* AddBone(int ID, int parentID);

		void SetBoneBindPoseMatrixInv(int ID, float matrix[16]);

		//void BindVertex(int vertex, int boneID[4], float weight[4]);

		void BuildTree();

		void SetRenderable(Renderable* renderable) { mRenderable = renderable; }

		void AddAnimation(const char* name, SkeletonAnimation* animation) { mAnimations.insert(std::make_pair(name, animation)); }

		const Matrix4x4f* GetVertexTransform() { return mVertexTransform; }

		void StartPlay(const char* name);

		void Update(float time);

		bool IsPlaying() { return mPlaying; }

	private:
		Bone* FindBone(int ID);

		std::vector<Bone*> mAllBones;
		typedef std::map<std::string, SkeletonAnimation*> AnimationMapType;
		AnimationMapType mAnimations;
		AnimationMapType::iterator mCurrentAnimation;
		float mTime;

		struct VertexBoneBind
		{
			Bone* Bones[4];
			float weight[4];
		}*mVertexBoneBind;

		Matrix4x4f* mVertexTransform;
		Renderable* mRenderable;

		uint mVertexNum;
		uint mBoneNum;

		uint mCurrentFrame;
		bool mPlaying;
	};

	class SkeletonAnimation
	{
	public:

		struct Frame
		{
			Skeleton::BoneTransform* Transforms;
			float Time;

			Frame() : Transforms(0) {}
			~Frame() { delete[] Transforms; }
		};

		SkeletonAnimation(Skeleton* skeleton, uint frameNum);
		~SkeletonAnimation();

		uint GetFrameNum() { return mFrameNum; }

		Frame* GetFrame(uint frame) { return &mFrames[frame]; }

		void SetFrame(uint frame, float time, Skeleton::BoneTransform* transforms);
	private:
		Frame* mFrames;
		uint mFrameNum;
		Skeleton* mSkeleton;
	};
}

#endif