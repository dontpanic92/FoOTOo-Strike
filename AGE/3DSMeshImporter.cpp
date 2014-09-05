#include "3DSMeshImporter.h"
#include "Log.h"
#include "ResourceManager.h"
/*
using namespace AGE;
uint total = 0;
Scene* AGE3DSMeshImporter::LoadSceneFromFile(){
	total = 0;

	Lib3dsFile * file;
	file = lib3ds_file_open("../Resources/Models/cs_assault.3ds");
	// If loading the model failed, we throw an exception
	if (!file)
	{
		Log::Error("Cannot load 3ds file.");
		return NULL;
	}
	lib3ds_file_eval(file, 0);

	Scene* scene = new Scene;
	Lib3dsNode *nod;

	if (file->nodes){
		for (nod = file->nodes; nod != 0; nod = nod->next) {
			ProcessNode(file, nod, scene);
		}
	}
	else if (file->meshes){
		for (uint i = 0; i < file->nmeshes; i++)
			ProcessMesh(file, file->meshes[i], scene);
	}

	lib3ds_file_free(file);
	printf("%d\n", total);
	return scene;
}

void AGE3DSMeshImporter::ProcessMesh(Lib3dsFile* file, Lib3dsMesh* mesh, Scene* scene){
	Lib3dsFace      *face;
	Lib3dsMaterial  *mat;
	Lib3dsMeshInstanceNode *meshData;

	float    *norm_verts;
	float    *norm_faces;
	float       M[4][4];


	norm_verts = new float[3 * 3 * mesh->nfaces];
	//norm_faces = new float[3 * mesh->nfaces];

	//lib3ds_matrix_copy(M, mesh->matrix);
	//lib3ds_matrix_inv(M);
	//glMultMatrixf(&M[0][0]);


	//lib3ds_mesh_calculate_face_normals(mesh, (float(*)[3])norm_faces);

	lib3ds_mesh_calculate_vertex_normals(mesh, (float(*)[3])norm_verts);
	//lib3ds_matrix_copy(M, mesh->matrix);

	Mesh* ageMesh = new Mesh(mesh->nfaces * 3, mesh->nfaces * 3);
	//ageMesh->SetData((GLfloat*)mesh->vertices, (GLfloat*)norm_verts, (GLfloat*)mesh->texcos, NULL);

	uint* materials = new uint[file->nmaterials];
	Material** ms = new Material*[file->nmaterials];
	uint* curIndex = new uint[file->nmaterials];

	memset(materials, 0, sizeof(uint) * file->nmaterials);
	memset(ms, 0, sizeof(Material*) * file->nmaterials);
	memset(curIndex, 0, sizeof(uint) * file->nmaterials);

	for (uint fi = 0; fi < mesh->nfaces; ++fi) {
		materials[mesh->faces[fi].material]++;
	}
	//Material* ma = new Material(mesh->nfaces * 3);
	//Texture2D* texture2 = ResourceManager::GetInstance()->LoadTexture2D(file->materials[0]->texture1_map.name);

	//Texture2D* texture2 = new Texture2D();
	//char path[256];
	//strcpy(path, "../Resources/Textures/");
	//strcat(path, file->materials[face->material]->texture1_map.name);
	//texture2->Load(path, Texture2D::AUTO);

	//ma->SetTexture(texture2);
	for (uint fi = 0; fi < mesh->nfaces; ++fi) {
		total++;
		face = &(mesh->faces[fi]);
		float vertex[9], normal[9], texCoord[6];
		ushort index[3] = { fi * 3, fi * 3 + 1, fi * 3 + 2 };
		//ma->AddIndex(fi, face->index);
		memcpy(vertex, mesh->vertices[face->index[0]], sizeof(float) * 3);
		memcpy(vertex + 3, mesh->vertices[face->index[1]], sizeof(float) * 3);
		memcpy(vertex + 6, mesh->vertices[face->index[2]], sizeof(float) * 3);
		memcpy(normal, &norm_verts[fi * 9], sizeof(float) * 9);
		if (mesh->texcos){
			memcpy(texCoord, mesh->texcos[face->index[0]], sizeof(float) * 2);
			memcpy(texCoord + 2, mesh->texcos[face->index[1]], sizeof(float) * 2);
			memcpy(texCoord + 4, mesh->texcos[face->index[2]], sizeof(float) * 2);
		}
		ageMesh->AddVertex(fi, vertex);
		ageMesh->AddNormal(fi, normal);
		ageMesh->AddTexCoord(fi, texCoord);

		if (face->material != -1){
			if (!ms[face->material]){
				ms[face->material] = new Material(materials[face->material] * 3);
				Texture2D* texture2 = ResourceManager::GetInstance()->LoadTexture2D(file->materials[face->material]->texture1_map.name);

				//Texture2D* texture2 = new Texture2D();
				//char path[256];
				//strcpy(path, "../Resources/Textures/");
				//strcat(path, file->materials[face->material]->texture1_map.name);
				//texture2->Load(path, Texture2D::AUTO);

				ms[face->material]->SetTexture(texture2);
				ageMesh->AddMaterial(ms[face->material]);
			}

			ms[face->material]->AddIndex(curIndex[face->material], index);
			curIndex[face->material]++;
		}
		//memcpy(normal, mesh->vertices[face->index[1]], sizeof(float) * 3);
		//memcpy(normal, mesh->vertices[face->index[2]], sizeof(float) * 3);

		//ms[face->material]->AddIndex(curIndex[face->material], face->index);
		//curIndex[face->material]++;

		/*mat = 0;
		if (face->material >= 0 && face->material < file->nmaterials)
		mat = file->materials[face->material];
		if (mat){
		printf("1: %s 2: \n", mat->texture1_map.name, mat->texture2_map.name);
		}
		mat = 0;

		if (face->material >= 0 && face->material < file->nmaterials)
		mat = file->materials[face->material];

		if (mat)
		{
		static GLfloat a[4] = { 0, 0, 0, 1 };
		float s;
		glMaterialfv(GL_FRONT, GL_AMBIENT, a);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat->diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat->specular);
		s = pow(2, 10.0*mat->shininess);

		if (s>128.0) {
		s = 128.0;
		}

		glMaterialf(GL_FRONT, GL_SHININESS, s);
		}
		else {
		float a[] = { 0.2, 0.2, 0.2, 1.0 };
		float d[] = { 0.8, 0.8, 0.8, 1.0 };
		float s[] = { 0.0, 0.0, 0.0, 1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, a);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, d);
		glMaterialfv(GL_FRONT, GL_SPECULAR, s);
		}
		

		//glNormal3fv(norm_faces[fi].v);  // face normal   

		//for (int i = 0; i < 3; ++i) {
		//glNormal3fv(norm_verts[3 * fi + i].v);  // vertex normal   
		//}

	}

	//ageMesh->AddMaterial(ma);
	ageMesh->BuildMatierials();

	ShaderProgram* shader = ResourceManager::GetInstance()->LoadShader("DefaultLight");

	Renderable* renderable = new Renderable();
	renderable->SetMesh(ageMesh);
	renderable->SetShader(shader);
	//renderable->SetTexture(texture2);
	//Transform* t = renderable->GetTramsform();
	//renderable->GetTramsform()->RotateByRadian(Deg2Rad(180), 0.0f, 1.0f, 0.0f);
	//renderable->GetTramsform()->RotateByRadian(Deg2Rad(-90), 1.0f, 0.0f, 0.0f);
	//delete[] norm_faces;
	delete[] norm_verts;
	delete[] materials;
	delete[] ms;
	delete[] curIndex;

	scene->GetRoot()->Attach(renderable);
}

void AGE3DSMeshImporter::ProcessNode(Lib3dsFile* file, Lib3dsNode* node, Scene* scene){
	Lib3dsNode* p;
	Lib3dsMesh      *mesh;
	for (p = node->childs; p != 0; p = p->next){
		ProcessNode(file, p, scene);
	}

	if (node->type == LIB3DS_NODE_MESH_INSTANCE)
	{
		if (strcmp(node->name, "$$$DUMMY") == 0) {
			return;
		}
		mesh = lib3ds_file_mesh_for_node(file, node);
		//assert(mesh);
		if (!mesh) {
			return;
		}

		ProcessMesh(file, mesh, scene);

	}
}
*/