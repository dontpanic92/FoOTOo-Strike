#ifndef __AGE_SCENE_HEADER__
#define __AGE_SCENE_HEADER__
//#include <memory>
#include <vector>
#include "Mesh.h"
#include "LinearMath.h"
#include "Renderable.h"
#include "Camera.h"
#include "Light.h"
#include "Def.h"
//using std::shared_ptr;
using std::vector;

namespace AGE
{

	class AGE_EXPORT SceneNode
	{
	public:
		SceneNode();
		~SceneNode();

		void Attach(Renderable* renderable);
		void Attach(SceneNode* node);
		void Attach(Attachable* otherObjects);
		void Detach(Renderable* renderable); 
		void Detach(SceneNode* node);
		void Detach(Attachable* otherObjects);

		SceneNode* GetParent() { return mParent; }
		const vector<SceneNode*>& GetChildren() { return mNodes; }
		const vector<Renderable*>& GetRenderables() { return mRenderables; }

		void Render(const Matrix4x4f& parentMatrix, const Matrix4x4f & viewMatrix);
		void UpdateAndCulling(const Matrix4x4f& parentMatrix);

		Transform* GetTransform() { return &mTransform; }
		const Matrix4x4f& GetWorldTransform() { return mWorldTransform; }
		//Matrix4x4f CalcWorldTransformMatrix();

	private:
		void SetParent(SceneNode* parent) { mParent = parent; }

		vector<SceneNode*> mNodes;
		vector<Renderable*> mRenderables;
		vector<Attachable*> mOtherObjects;

		SceneNode* mParent;

		Transform mTransform;

		Matrix4x4f mWorldTransform;
	};

	class AGE_EXPORT Scene
	{
	public:
		Scene();
		~Scene();

		void StartUp();
		SceneNode* GetRoot() { return &mRoot; }
		Renderable* LoadMesh();

		void Render();
		void UpdateAndCulling();

		void AttachCameraOnNode(SceneNode* node);

		Camera* GetCurrentCamera() { return &mCamera; }

		SceneNode* CreateSceneNode(SceneNode* parent = NULL);

		Light* CreateLight(SceneNode* parent = NULL);

		vector<Light*>& GetLights() { return mLights; }

		//Attachable* GetAttachable() { return mAttachable[0]; }
	private:
		SceneNode mRoot;
		vector<Light*> mLights;

		Camera mCamera;

	};
}

#endif