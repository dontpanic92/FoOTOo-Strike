#include "SceneNode.h"
#include "PhysicsEngine.h"
#include "Renderable.h"
#include "RenderQueue.h"
using namespace AGE;

SceneNode::SceneNode(SceneNode* parent)
{
	mParent = parent;
}

void SceneNode::Attach(SceneNode* object)
{
	if (object->GetParent())
		object->GetParent()->Detach(object);
	object->SetParent(this);
	mChildren.push_back(object);
}

void SceneNode::Detach(SceneNode* object)
{
	for (auto it = mChildren.begin(); it != mChildren.end();) {
		if (*it == object) {
			(*it)->SetParent(nullptr);
			it = mChildren.erase(it);
		} else {
			it++;
		}
	}
}

Renderable* SceneNode::SetRenderable(Renderable* r)
{
	Renderable* old = mRenderable;
	mRenderable = r;
	r->SetParent(this);

	if (old == r) {
		return nullptr;
	} else if (old) {
		old->SetParent(nullptr);
	}
	return old;
}

void SceneNode::UpdateAndCulling(const Matrix4x4f& parentMatrix)
{
	mWorldTransform = mTransform.GetTransformMatrix() * parentMatrix;
	for each(auto obj in mChildren)
	{
		obj->UpdateWorldMatrix(mWorldTransform.GetTransformMatrix());
		//printf("scenenode: %p, attachable: %p\n", this, attachable);

		if (Renderable* r = obj->GetRenderable())
			RenderQueue::GetInstance()->PushRenderable(r);
	}

	for each(auto node in mChildren)
	{
		node->UpdateAndCulling(mWorldTransform.GetTransformMatrix());
	}
}