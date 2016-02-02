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
		Renderable* GetRenderable() { return mRenderable; }
		Renderable* SetRenderable(Renderable* r);

		virtual ~SceneObject() {}

		Transform* GetTransform() { return &mTransform; }

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
		void SetParent(SceneNode* parent) { mParent = parent; }

		Transform mTransform;
		Transform mWorldTransform;
		SceneNode* mParent = nullptr;
		Renderable* mRenderable = nullptr;

		friend class SceneNode;
		friend class Scene;
	};
}

#endif
