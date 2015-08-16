#ifndef __AGE_SCENEOBJECT_HEADER__
#define __AGE_SCENEOBJECT_HEADER__

#include "Def.h"
#include "LinearMath.h"


namespace AGE
{

	class SceneNode;
	class Renderable;
	class AGE_EXPORT SceneObject
	{
	public:
		virtual Renderable* GetRenderable() { return 0; }
		virtual ~SceneObject() {}

		Transform* GetTransform() { return &mTransform; }

		void SetParent(SceneNode* parent) { mParent = parent; }
		SceneNode* GetParent() { return mParent; }

		void UpdateWorldMatrix(const Matrix4x4f& parentMatrix)
		{
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
		Transform mWorldTransform;
		SceneNode* mParent = 0;
	};
}

#endif
