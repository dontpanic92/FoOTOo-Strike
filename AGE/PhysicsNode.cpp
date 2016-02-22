#include "PhysicsNode.h"
#include "PhysicsEngine.h"

using namespace AGE;

PhysicsNode::PhysicsNode(Renderable* r)
{
	SetRenderable(r);
	InitPhysics();
}

void PhysicsNode::InitPhysics()
{
	int materialID = 0;
	bool optimize = true;

	NewtonCollision* collision = NewtonCreateTreeCollision(GetPhysicsWorld(), materialID);

	NewtonTreeCollisionBeginBuild(collision);

	for (int i = 0; i < mRenderable->GetNumberOfRenderObjects(); i++)
	{
		RenderObject* rObj = mRenderable->GetRenderObject(i);
		Mesh::Vertex* vertex = rObj->Mesh->GetVertexData();

		for (int j = 0; j < rObj->Mesh->GetNumberOfVertex(); j += 3)
		{
			NewtonTreeCollisionAddFace(collision, 3, vertex[j].Position, sizeof(vertex[j]), materialID);
		}
	}
	NewtonTreeCollisionEndBuild(collision, optimize ? 1 : 0);

	NewtonBody* const body = NewtonCreateDynamicBody(GetPhysicsWorld(), collision, this->GetWorldMatrix());
	NewtonBodySetUserData(body, this);

	NewtonDestroyCollision(collision);
}
