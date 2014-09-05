#ifndef __AGE_SCENE_HEADER__
#define __AGE_SCENE_HEADER__
//#include <memory>
#include <vector>
#include "Mesh.h"
#include "Math.h"
#include "Renderable.h"
#include "Camera.h"

//using std::shared_ptr;
using std::vector;

namespace AGE
{
	class SceneNode
	{
	public:
		SceneNode();
		~SceneNode();

		SceneNode* CreateNode();
		void Attach(Attachable* attach);
		void Attach(SceneNode* node);
		void SetParent(SceneNode* parent) { mParent = parent; }
		SceneNode* GetParent() { return mParent; }
		const vector<SceneNode*>& GetChildren() { return mNodes; }
		const vector<Attachable*>& GetAttachable() { return mAttachable; }

		void Render(const Matrix4x4f& parentMatrix);

		Transform* GetTramsform(){ return &mTransform; }
	private:
		vector<SceneNode*> mNodes;
		vector<Attachable*> mAttachable;

		SceneNode* mParent;

		Transform mTransform;
	};

	class Scene
	{
	public:
		Scene();
		~Scene();

		SceneNode* GetRoot() { return &mRoot; }
		Renderable* LoadMesh();

		void Render();

		void AttachCameraOnNode(SceneNode* node);

		Camera* GetCurrentCamera(){ return &mCamera; }
		Attachable* GetAttachable() { return mAttachable[0]; }
	private:
		SceneNode mRoot;
		vector<Attachable*> mAttachable;

		Camera mCamera;

	};
}

#endif