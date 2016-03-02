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
	class ScenePrivate;
	class AGE_EXPORT Scene
	{
	public:
		Scene(std::wstring name = L"");
		~Scene();

		void Load(const char* sceneFilePath = nullptr);
		//bool Save(const char* path);
		SceneNode* GetRoot();

		void UpdateAndCulling();

		void AttachCameraOnNode(SceneNode* node);

		Camera* GetCurrentCamera();

		SceneNode* CreateSceneNode(SceneNode* parent = NULL);

		Light* CreateLight(SceneNode* parent = NULL);

		vector<Light*>& GetLights();

		SkyBox* GetSkyBox();
		void AddSkyBox();

		void UpdateCameraAspectRatio();

	protected:
		Scene(ScenePrivate* d);
		ScenePrivate* dptr();

	private:
		ScenePrivate* d;
	};
}

#endif