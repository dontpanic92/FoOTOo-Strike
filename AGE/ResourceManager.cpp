#include "ResourceManager.h"

#include <Windows.h>

using namespace AGE;
using namespace std;


ResourceManager::~ResourceManager(){
	for (Texture2DMap::iterator it = mTextures.begin(); it != mTextures.end(); it++){
		delete it->second;
	}

	for (ShaderProgramMap::iterator it = mShaderPrograms.begin(); it != mShaderPrograms.end(); it++){
		delete it->second;
	}
}

Mesh* ResourceManager::LoadMesh(){
	return NULL;//new Mesh();
}

Texture2D* ResourceManager::LoadTexture2D(const char* name){
	Texture2DMap::iterator it;
	if ((it = mTextures.find(name)) != mTextures.end()){
		return it->second;
	} else {

		Texture2D* texture2 = new Texture2D();
		char path[256];
		strcpy(path, "../Resources/Textures/");
		strcat(path, name);

		//char longpath[256];

		//GetLongPathNameA(path, longpath, sizeof(longpath));

		printf("%s\n", path);
		texture2->Load(path, Texture2D::AUTO);

		mTextures.insert(make_pair(name, texture2));
		return texture2;
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
