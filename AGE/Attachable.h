#ifndef __AGE_ATTACHABLE_HEADER__
#define __AGE_ATTACHABLE_HEADER__

#include "Math.h"

namespace AGE
{
	class SceneNode;
	class Attachable
	{
	public:
		Attachable() :mParent(0) {}

		virtual ~Attachable(){};
		Transform* GetTramsform(){ return &mTransform; }

		void SetParent(SceneNode* parent) { mParent = parent; }
		SceneNode* GetParent() { return mParent; }

		void UpdateWorldMatrix(const Matrix4x4f& parentMatrix)
		{ 
			mWorldMatrix = mTransform.GetTransformMatrix() * parentMatrix;
		}

		const Matrix4x4f& GetWorldMatrix()
		{
			return mWorldMatrix;
		}

	protected:
		Transform mTransform;
		Matrix4x4f mWorldMatrix;
		SceneNode* mParent;

	};

}

#endif