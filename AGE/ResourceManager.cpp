#include <Windows.h>
#include "ResourceManager.h"
#include "RenderEngine.h"


using namespace AGE;
using namespace std;

const wchar_t* ResourceManager::RootDirectory = L"../Resources/";

ResourceManager::~ResourceManager()
{
	for (MaterialMap::iterator it = mMaterials.begin(); it != mMaterials.end(); it++) {
		delete it->second;
	}

	for (ShaderProgramMap::iterator it = mShaderPrograms.begin(); it != mShaderPrograms.end(); it++) {
		delete it->second;
	}
}

Mesh* ResourceManager::LoadMesh()
{
	return NULL;//new Mesh();
}

Material* ResourceManager::LoadMaterial(const char* name)
{
	MaterialMap::iterator it;
	if ((it = mMaterials.find(name)) != mMaterials.end()) {
		return it->second;
	} else {

		Material* material = new Material();
		char path[256];
		strcpy(path, "../Resources/Textures/");
		strcat(path, name);

		//char longpath[256];

		//GetLongPathNameA(path, longpath, sizeof(longpath));

		printf("%s\n", path);

		Texture2D* texture2 = RenderEngine::GetInstance()->CreateTextrue2D(path);
		material->SetTexture(texture2);
		mMaterials.insert(make_pair(name, material));
		return material;
	}
}

Shader* ResourceManager::LoadShader(const char* name)
{
	ShaderProgramMap::iterator it;
	if ((it = mShaderPrograms.find(name)) != mShaderPrograms.end()) {
		return it->second;
	} else {
		/*Shader* shader = new Shader();
		char path1[256], path2[256];
		strcpy(path1, "../Resources/Shaders/GLSL/");
		strcat(path1, name);
		strcat(path1, ".vshader");

		strcpy(path2, "../Resources/Shaders/GLSL/");
		strcat(path2, name);
		strcat(path2, ".fshader");

		shader->LoadAndCompile(path1, path2);

		*/
		Shader* shader = RenderEngine::GetInstance()->CreateShader(name);
		mShaderPrograms.insert(make_pair(name, shader));
		return shader;
	}
}
