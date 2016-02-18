#ifndef __AGE_SceneNode_HEADER__
#define __AGE_SceneNode_HEADER__

#include <vector>
#include "Def.h"
#include "LinearMath.h"


namespace AGE
{

	class Renderable;
	class AGE_EXPORT SceneNode
	{
	public:
		SceneNode(SceneNode* parent = nullptr);

		void Attach(SceneNode* object);
		void Detach(SceneNode* object);

		Renderable* GetRenderable() { return mRenderable; }
		Renderable* SetRenderable(Renderable* r);

		virtual ~SceneNode() {}

		Transform* GetTransform() { return &mTransform; }
		SceneNode* GetParent() { return mParent; }
		const std::vector<SceneNode*>& GetChildren() { return mChildren; }

		Matrix4x4f GetWorldMatrix() const
		{
			return mWorldTransform.GetTransformMatrix();
		}

		const Transform& GetWorldTransform() const
		{
			return mWorldTransform;
		}

		void SetName(const char* name)
		{
			mNodeName = name;
		}

		const char* GetName()
		{
			return mNodeName.c_str();
		}

	protected:
		void SetParent(SceneNode* parent) { mParent = parent; }

		void UpdateAndCulling(const Matrix4x4f& parentMatrix);

		void UpdateWorldMatrix(const Matrix4x4f& parentMatrix)
		{
			mWorldTransform = mTransform;
			mWorldTransform.Multiply(parentMatrix);
		}

		Transform mTransform;
		Transform mWorldTransform;
		SceneNode* mParent = nullptr;
		Renderable* mRenderable = nullptr;

		std::vector<SceneNode*> mChildren;

		std::string mNodeName;

		friend class Scene;
	};
}

#endif
