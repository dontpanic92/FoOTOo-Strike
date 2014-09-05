#include "AGEMeshImporter.h"
#include "ResourceManager.h"
using namespace AGE;

SceneNode* AGEMeshImporter::LoadFromFile(const char* filename) {
	FILE* filp;

	fopen_s(&filp, filename, "rb");

	if (!filp)
		return NULL;

	AM1FileHeader fileHeader;
	fread(&fileHeader, 1, sizeof(fileHeader), filp);

	SceneNode* node = new SceneNode();

	AM1MaterialHeader matHeader;
	for (int i = 0; i < fileHeader.MaterialNum; i++) {
		fread(&matHeader, 1, sizeof(matHeader), filp);
		Mesh* mesh = new Mesh(matHeader.VertexNum);


		fread(mesh->GetVertexData(), sizeof(float), matHeader.VertexNum * 3, filp);
		fread(mesh->GetNormalData(), sizeof(float), matHeader.VertexNum * 3, filp);
		fread(mesh->GetTextureData(), sizeof(float), matHeader.VertexNum * 2, filp);
		//memset(mesh->GetTextureData(), 0, sizeof(float)* matHeader.VertexNum * 2);

		/*for (uint i = 0; i < matHeader.VertexNum * 3; i += 3) {
			if (mesh->GetVertexData()[i] < 10 && mesh->GetVertexData()[i] > -10)
				printf("%f %f %f\n", mesh->GetVertexData()[i], mesh->GetVertexData()[i + 1], mesh->GetVertexData()[i + 2]);
		}*/

		ShaderProgram* shader = ResourceManager::GetInstance()->LoadShader("DefaultLight");
		Material* material = new Material();
		Texture2D* texture2 = ResourceManager::GetInstance()->LoadTexture2D(matHeader.TextureName);
		material->SetTexture(texture2);

		Renderable* renderable = new Renderable();
		renderable->SetMesh(mesh);
		renderable->SetShader(shader);
		renderable->SetMaterial(material);

		node->Attach(renderable);
	}

	fclose(filp);

	return node;
}