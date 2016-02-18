#ifndef __AGE_SCENE_HEADER__
#define __AGE_SCENE_HEADER__
//#include <memory>
#include <vector>
#include "Mesh.h"
#include "LinearMath.h"
#include "Renderable.h"
#include "Camera.h"
#include "Light.h"
#include "SkyBox.h"
#include "Def.h"
#include "SceneNode.h"
using std::vector;

namespace AGE
{

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

		SkyBox* GetSkyBox() { return mSkyBox; }
		void AddSkyBox() { if(!mSkyBox) mSkyBox = new SkyBox(); }

		void UpdateCameraAspectRatio();

		//Attachable* GetAttachable() { return mAttachable[0]; }
	private:
		SceneNode mRoot;
		vector<Light*> mLights;

		Camera mCamera;
		SkyBox* mSkyBox = nullptr;
	};
}

#endif