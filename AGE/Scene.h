#ifndef __AGE_SCENE_HEADER__
#define __AGE_SCENE_HEADER__
//#include <memory>
#include <vector>
#include "Mesh.h"
#include "Math.h"

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
		const vector<SceneNode*>& GetChildren() { return mChildrenNodes; }
		const vector<Attachable*>& GetAttachable() { return mChildrenAttachable; }
	private:
		vector<SceneNode*> mChildrenNodes;
		vector<Attachable*> mChildrenAttachable;
	};

	class Scene
	{
	public:
		Scene();
		~Scene();

		SceneNode* GetRoot() { return &mRoot; }
		Mesh* LoadMesh();
	private:
		SceneNode mRoot;
		vector<Attachable*> mAllAttachable;
	};
}

#endif