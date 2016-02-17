#ifndef __AGE_RESOURCEMANAGER_HEADER__
#define __AGE_RESOURCEMANAGER_HEADER__

#include "Singleton.h"
#include "Mesh.h"
#include "Shader.h"

#include <map>

namespace AGE
{
	class AGE_EXPORT ResourceManager : public Singleton<ResourceManager>
	{
	public:
		const static wchar_t* RootDirectory;
		
		ResourceManager() {}
		~ResourceManager();



		Mesh* LoadMesh();

		//Texture2D* LoadTexture2D(const char* name);

		Shader* LoadShader(const char* name);

		Material* LoadMaterial(const char* name, TextureType type = TextureType::Texture2D);
	private:

		typedef std::map<std::string, Material*>	MaterialMap;
		typedef std::map<std::string, Shader*>		ShaderProgramMap;


		MaterialMap				mMaterials;
		ShaderProgramMap		mShaderPrograms;
	};
}

#endif
