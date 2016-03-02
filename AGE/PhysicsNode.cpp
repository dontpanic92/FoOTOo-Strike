#include "PhysicsNode.h"
#include "PhysicsEngine.h"

using namespace AGE;

PhysicsNode::PhysicsNode(Renderable* r) :SceneNode(r)
{
	InitPhysics();
	mTransform.AddValueChangedCallback([this](Transform*){
		NewtonBodySetMatrix(mBody, GetParent()->GetWorldMatrix() * mTransform.GetTransformMatrix());
	});
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

	mBody = NewtonCreateDynamicBody(GetPhysicsWorld(), collision, this->GetWorldMatrix());
	NewtonBodySetUserData(mBody, this);
	NewtonBodySetTransformCallback(mBody, &PhysicsNode::NewtonSetTransformCallback);

	NewtonDestroyCollision(collision);
}

void PhysicsNode::NewtonSetTransformCallback(const NewtonBody* const body, const dFloat* const matrix, int threadIndex)
{
	PhysicsNode* p = (PhysicsNode*)NewtonBodyGetUserData(body);
	Matrix4 parentMatrix = p->GetParent()->GetWorldMatrix();
	p->mTransform.SetTransformMatrixNoCallback(parentMatrix.Inverse() * Matrix4(matrix));
}
