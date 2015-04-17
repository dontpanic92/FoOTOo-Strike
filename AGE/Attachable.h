#ifndef __AGE_ATTACHABLE_HEADER__
#define __AGE_ATTACHABLE_HEADER__

#include "LinearMath.h"
#include "Def.h"

namespace AGE
{
	class SceneNode;
	class AGE_EXPORT Attachable
	{
	public:
		Attachable() :mParent(0) {}

		virtual ~Attachable() {};
		Transform* GetTransform() { return &mTransform; }

		void SetParent(SceneNode* parent);
		SceneNode* GetParent() { return mParent; }

		void UpdateWorldMatrix(const Matrix4x4f& parentMatrix)
		{
			//mWorldMatrix = mTransform.GetTransformMatrix() * parentMatrix;
			mWorldTransform = mTransform;
			mWorldTransform.Multiply(parentMatrix);
		}

		Matrix4x4f GetWorldMatrix() const
		{
			return mWorldTransform.GetTransformMatrix();
		}

		const Transform& GetWorldTransform() const
		{
			return mWorldTransform;
		}

	protected:
		Transform mTransform;
		//Matrix4x4f mWorldMatrix;
		Transform mWorldTransform;
		SceneNode* mParent;

	};

}

#endif