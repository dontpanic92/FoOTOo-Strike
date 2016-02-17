#include "SkyBox.h"
#include "Primitive.h"
#include "ResourceManager.h"
#include "RenderEngine.h"

using namespace AGE;

SkyBox::SkyBox()
{
	mRenderable = new Renderable();
	Mesh* m = Primitive::CreateSphereUnmanage(20);
	Material* material = ResourceManager::GetInstance()->LoadMaterial("sunsetcube1024.dds", TextureType::CubeTexture);
	Shader* shader = ResourceManager::GetInstance()->LoadShader("Sky");
	mRenderable->AddRenderObject(RenderEngine::GetInstance()->CreateRenderObject(mRenderable, m, material, shader, true));
}

SkyBox::~SkyBox()
{
	delete mRenderable;
}
