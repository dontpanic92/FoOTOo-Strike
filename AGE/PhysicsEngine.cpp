#include "PhysicsEngine.h"
#include "Renderable.h"

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
	/*
	mDynamicsWorld->setGravity(btVector3(0, -600, 0));*/

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
