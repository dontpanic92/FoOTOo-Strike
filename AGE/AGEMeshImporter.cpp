#include "AGEMeshImporter.h"
#include "ResourceManager.h"
#include "RenderEngine.h"
using namespace AGE;

Renderable* AGEMeshImporter::LoadFromFile(const char* filename)
{
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
	//renderable->SetVertexData(vertex, fileHeader.VertexNum);

	uint32_t ** vertexIndices = new uint32_t *[fileHeader.MaterialNum];
	Mesh** meshes = new Mesh *[fileHeader.MaterialNum];
	AM1MaterialHeader matHeader;
	for (int i = 0; i < fileHeader.MaterialNum; i++) {
		fread(&matHeader, 1, sizeof(matHeader), filp);
		Mesh* mesh = new Mesh(renderable);
		meshes[i] = mesh;
		vertexIndices[i] = new uint32_t[matHeader.VertexNum];
		float* normal = new float[matHeader.VertexNum * 3];
		float* textureCoord = new float[matHeader.VertexNum * 2];

		//fread(mesh->GetIndexData(), sizeof(uint32_t), matHeader.VertexNum * 3, filp);
		fread(vertexIndices[i], sizeof(uint32_t), matHeader.VertexNum, filp);
		fread(normal, sizeof(float), matHeader.VertexNum * 3, filp);
		fread(textureCoord, sizeof(float), matHeader.VertexNum * 2, filp);
		//memset(mesh->GetTextureData(), 0, sizeof(float)* matHeader.VertexNum * 2);

		//GLushort* index = mesh->GetIndexData();

		Mesh::Vertex* meshVertex = new Mesh::Vertex[matHeader.VertexNum];

		for (uint j = 0; j < matHeader.VertexNum; j++) {
			//mesh->SetVertex(i, &vertex[matVertex[i] * 3]);
			memcpy(meshVertex[j].Position, &vertex[vertexIndices[i][j] * 3], sizeof(float) * 3);
			memcpy(meshVertex[j].Normal, &normal[j * 3], sizeof(float) * 3);
			memcpy(meshVertex[j].TextureCoord, &textureCoord[j * 2], sizeof(float) * 2);
		}
		mesh->SetVertexData(meshVertex, matHeader.VertexNum);

		delete[] meshVertex;
		//mesh->UpdateVertex();

		Material* material = ResourceManager::GetInstance()->LoadMaterial(matHeader.TextureName);
		Shader* shader = ResourceManager::GetInstance()->LoadShader("DefaultLight");
		//material->SetShader(shader);
		//new Material();
		//Texture2D* texture2 = ResourceManager::GetInstance()->LoadTexture2D(matHeader.TextureName);
		//material->SetTexture(texture2);

		renderable->AddRenderObject(RenderEngine::GetInstance()->CreateRenderObject(renderable, mesh, material, shader));

		delete[] normal;
		delete[] textureCoord;
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

		Mesh::SkeletonData* skeletonData = new Mesh::SkeletonData[fileHeader.VertexNum];

		for (int i = 0; i < fileHeader.VertexNum; i++) {
			//int id[4];
			//float w[4];
			fread(&skeletonData[i].BoneID, sizeof(int), 4, filp);
			fread(&skeletonData[i].weight, sizeof(float), 4, filp);
			//skeleton->BindVertex(i, id, w);
		}

		for (int i = 0; i < fileHeader.MaterialNum; i++) {
			Mesh* mesh =meshes[i];

			for (int j = 0; j < mesh->GetNumberOfVertex(); j++) {
				mesh->SetSkeletonData(j, skeletonData[vertexIndices[i][j]]);
			}
		}



		delete[] skeletonData;

		renderable->SetSkeleton(skeleton);
	}

	fclose(filp);
	delete[] vertex;
	delete[] meshes;
	for (int i = 0; i < fileHeader.MaterialNum; i++) {
		delete[] vertexIndices[i];
	}
	delete[] vertexIndices;
	return renderable;
}