#include "AGEMeshImporter.h"
#include "ResourceManager.h"
using namespace AGE;

Renderable* AGEMeshImporter::LoadFromFile(const char* filename) {
	FILE* filp;

	fopen_s(&filp, filename, "rb");

	if (!filp)
		return NULL;

	AM1FileHeader fileHeader;
	fread(&fileHeader, 1, sizeof(fileHeader), filp);


	float *vertex = new float[fileHeader.VertexNum * 3];
	fread(vertex, sizeof(float), fileHeader.VertexNum * 3, filp);

	//SceneNode* node = new SceneNode();
	Renderable* renderable = new Renderable();
	renderable->SetVertexData(vertex, fileHeader.VertexNum);
	ShaderProgram* shader = ResourceManager::GetInstance()->LoadShader("DefaultLight");
	renderable->SetShader(shader);

	delete[] vertex;

	AM1MaterialHeader matHeader;
	for (int i = 0; i < fileHeader.MaterialNum; i++) {
		fread(&matHeader, 1, sizeof(matHeader), filp);
		Mesh* mesh = new Mesh(matHeader.VertexNum, renderable);
		uint32_t *matVertex = new uint32_t[matHeader.VertexNum];

		//fread(mesh->GetIndexData(), sizeof(uint32_t), matHeader.VertexNum * 3, filp);
		fread(matVertex, sizeof(uint32_t), matHeader.VertexNum, filp);
		fread(mesh->GetNormalData(), sizeof(float), matHeader.VertexNum * 3, filp);
		fread(mesh->GetTextureData(), sizeof(float), matHeader.VertexNum * 2, filp);
		//memset(mesh->GetTextureData(), 0, sizeof(float)* matHeader.VertexNum * 2);

		GLushort* index = mesh->GetIndexData();

		for (uint i = 0; i < matHeader.VertexNum; i++) {
			//mesh->SetVertex(i, &vertex[matVertex[i] * 3]);
			index[i] = matVertex[i];
		}

		mesh->UpdateVertex();

		Material* material = new Material();
		Texture2D* texture2 = ResourceManager::GetInstance()->LoadTexture2D(matHeader.TextureName);
		material->SetTexture(texture2);

		renderable->AddRenderObject(mesh, material);

		delete[] matVertex;
	}

	if (fileHeader.BoneNum > 0) {
		Skeleton* skeleton = new Skeleton(fileHeader.VertexNum, fileHeader.BoneNum);

		for (int i = 0; i < fileHeader.BoneNum; i++) {
			AM1Bone bone;
			fread(&bone, sizeof(bone), 1, filp);
			skeleton->AddBone(bone.BoneID, bone.ParentID);
			skeleton->SetBoneBindPoseMatrixInv(bone.BoneID, bone.BindPoseMatrixInv);
		}

		skeleton->BuildTree();

		for (int i = 0; i < fileHeader.VertexNum; i++) {
			int id[4];
			float w[4];
			fread(&id, sizeof(int), 4, filp);
			fread(&w, sizeof(float), 4, filp);
			skeleton->BindVertex(i, id, w);
		}

		renderable->SetSkeleton(skeleton);
	}

	fclose(filp);

	return renderable;
}