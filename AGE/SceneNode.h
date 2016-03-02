#ifndef __AGE_SCENENODE_HEADER__
#define __AGE_SCENENODE_HEADER__

#include <vector>
#include <map>
#include <memory>
#include "Def.h"
#include "LinearMath.h"


namespace AGE
{

	class Renderable;
	class AGE_EXPORT SceneNode
	{
	public:
		SceneNode();
		SceneNode(Renderable* r);

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


		void SetUserData(int id, std::shared_ptr<void> userdata);
		std::shared_ptr<void> GetUserData(int id);

		template <class T>
		std::shared_ptr<T> GetUserData(int id)
		{
			return std::static_pointer_cast<T>(GetUserData(id));
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
		std::map<int, std::shared_ptr<void>> mUserData;

		std::string mNodeName;

		friend class Scene;
	};
}

#endif
