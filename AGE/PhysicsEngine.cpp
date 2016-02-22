#include "PhysicsEngine.h"
#include "Renderable.h"
#include "SceneNode.h"

using namespace AGE;

PhysicsEngine::PhysicsEngine() 
	: mWorld(nullptr)
{
	
}

PhysicsEngine::~PhysicsEngine()
{
	ShutDown();
}

void PhysicsEngine::StartUp()
{

	mWorld = NewtonCreate();

	NewtonWorldSetUserData(mWorld, this);

}

void PhysicsEngine::ShutDown()
{
	if (mWorld) {
		NewtonWaitForUpdateToFinish(mWorld);
		NewtonDestroy(mWorld);

		mWorld = nullptr;
	}

}

void PhysicsEngine::Update(float time)
{

	NewtonUpdate(mWorld, time);
}

SceneNode* PhysicsEngine::GetRaycastClosetObject(const Vector3f& from, const Vector3f& to, SceneNode* exclude)
{
	class RaycastRecord
	{
	public:
		float IntersetParam = 999999.9f;
		const NewtonBody* Body = nullptr;
		const void* SelfEntity = nullptr;

		RaycastRecord(void* self) :SelfEntity(self) {}

		static dFloat RaycastCallback(const NewtonBody* const body, const NewtonCollision* const collisionHit, const dFloat* const contact, const dFloat* const normal, dLong collisionID, void* const userData, dFloat intersetParam)
		{
			RaycastRecord* p = (RaycastRecord*)userData;
			return p->pRaycastCallback(body, collisionHit, contact, normal, collisionID, intersetParam);
		}
	private:

		dFloat pRaycastCallback(const NewtonBody* const body, const NewtonCollision* const collisionHit, const dFloat* const contact, const dFloat* const normal, dLong collisionID, dFloat intersetParam)
		{
			void* p = nullptr;
			p = NewtonBodyGetUserData(body);
			if (intersetParam < IntersetParam && (!p || p != SelfEntity)) {
				IntersetParam = intersetParam;
				Body = body;
			}

			return IntersetParam;
		}
	};

	RaycastRecord record(exclude);
	NewtonWorldRayCast(GetPhysicsWorld(), from, to, RaycastRecord::RaycastCallback, &record, nullptr, 0);

	if (record.Body) {
		if (auto p = NewtonBodyGetUserData(record.Body)) {
			return (SceneNode*)p;
		}
	}

	return nullptr;
}
