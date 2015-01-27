#ifndef __AGE_RESOURCEMANAGER_HEADER__
#define __AGE_RESOURCEMANAGER_HEADER__

#include "Singleton.h"
#include "Mesh.h"
#include "ShaderProgram.h"

#include <map>

namespace AGE
{
	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		ResourceManager() {}
		~ResourceManager();

		Mesh* LoadMesh();

		//Texture2D* LoadTexture2D(const char* name);

		ShaderProgram* LoadShader(const char* name);

		Material* LoadMaterial(const char* name);
	private:

		typedef std::map<std::string, Material*>	MaterialMap;
		typedef std::map<std::string, ShaderProgram*>		ShaderProgramMap;


		MaterialMap				mMaterials;
		ShaderProgramMap		mShaderPrograms;
	};
}

#endif
