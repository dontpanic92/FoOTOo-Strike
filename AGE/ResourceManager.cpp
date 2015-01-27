#include "ResourceManager.h"

#include <Windows.h>

using namespace AGE;
using namespace std;


ResourceManager::~ResourceManager(){
	for (MaterialMap::iterator it = mMaterials.begin(); it != mMaterials.end(); it++) {
		delete it->second;
	}

	for (ShaderProgramMap::iterator it = mShaderPrograms.begin(); it != mShaderPrograms.end(); it++){
		delete it->second;
	}
}

Mesh* ResourceManager::LoadMesh(){
	return NULL;//new Mesh();
}

Material* ResourceManager::LoadMaterial(const char* name){
	MaterialMap::iterator it;
	if ((it = mMaterials.find(name)) != mMaterials.end()) {
		return it->second;
	} else {

		Material* material = new Material();
		Texture2D* texture2 = new Texture2D();
		char path[256];
		strcpy(path, "../Resources/Textures/");
		strcat(path, name);

		//char longpath[256];

		//GetLongPathNameA(path, longpath, sizeof(longpath));

		printf("%s\n", path);
		texture2->Load(path, Texture2D::AUTO);
		material->SetTexture(texture2);
		mMaterials.insert(make_pair(name, material));
		return material;
	}
}

ShaderProgram* ResourceManager::LoadShader(const char* name){
	ShaderProgramMap::iterator it;
	if ((it = mShaderPrograms.find(name)) != mShaderPrograms.end()){
		return it->second;
	}
	else {
		ShaderProgram* shader = new ShaderProgram();
		char path1[256], path2[256];
		strcpy(path1, "../Resources/Shaders/GLSL/");
		strcat(path1, name);
		strcat(path1, ".vshader");

		strcpy(path2, "../Resources/Shaders/GLSL/");
		strcat(path2, name);
		strcat(path2, ".fshader");

		shader->LoadAndCompile(path1, path2);

		mShaderPrograms.insert(make_pair(name, shader));

		return shader;
	}
}
