#include "StaticSceneObject.h"
#include "PhysicsEngine.h"
using namespace AGE;

void StaticSceneObject::InitPhysics()
{
	btTriangleMesh* meshInterface = new btTriangleMesh();

	for (int i = 0; i < mRenderable->GetNumberOfRenderObjects(); i++) 
	{
		RenderObject* rObj = mRenderable->GetRenderObject(i);
		Mesh::Vertex* vertex = rObj->Mesh->GetVertexData();

		for (int j = 0; j < rObj->Mesh->GetNumberOfVertex(); j+=3)
		{
			btVector3 a(vertex[j].Position[0], vertex[j].Position[1], vertex[j].Position[2]);
			btVector3 b(vertex[j + 1].Position[0], vertex[j + 1].Position[1], vertex[j + 1].Position[2]);
			btVector3 c(vertex[j + 2].Position[0], vertex[j + 2].Position[1], vertex[j + 2].Position[2]);

			meshInterface->addTriangle(a, b, c);
		}
	}

	bool	useQuantizedAabbCompression = true;
	btBvhTriangleMeshShape* trimeshShape = new btBvhTriangleMeshShape(meshInterface, useQuantizedAabbCompression);

	btTransform start;
	start.setIdentity();

	btRigidBody* body = GetPhysicsEngine()->CreateRigidBody(0, start, trimeshShape);
}