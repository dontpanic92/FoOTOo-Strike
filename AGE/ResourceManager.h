#ifndef __AGE_RESOURCEMANAGER_HEADER__
#define __AGE_RESOURCEMANAGER_HEADER__

#include "Singleton.h"
#include "Mesh.h"
#include "Texture2D.h"
#include "ShaderProgram.h"

#include <map>

namespace AGE
{
	class ResourceManager : public Singleton<ResourceManager>
	{
	public:

		~ResourceManager();

		Mesh* LoadMesh();

		Texture2D* LoadTexture2D(const char* name);

		ShaderProgram* LoadShader(const char* name);

	private:
		ResourceManager(){}

		typedef std::map<std::string, Texture2D*>	Texture2DMap;
		typedef std::map<std::string, ShaderProgram*>		ShaderProgramMap;


		Texture2DMap	mTextures;
		ShaderProgramMap		mShaderPrograms;

		friend class Singleton<ResourceManager>;
	};
}

#endif
