#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "PhysicsEngine.h"
#include "Renderable.h"

using namespace AGE;

PhysicsEngine::PhysicsEngine() 
	: mBroadphase(nullptr),
	mDispatcher(nullptr),
	mSolver(nullptr),
	mCollisionConfiguration(nullptr),
	mDynamicsWorld(nullptr)
{
	
}

PhysicsEngine::~PhysicsEngine()
{
	ShutDown();
}

void PhysicsEngine::StartUp()
{
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	//m_collisionConfiguration->setConvexConvexMultipointIterations();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);

	mBroadphase = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
	mSolver = sol;

	mBroadphase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

	mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);

	

	mDynamicsWorld->setGravity(btVector3(0, -600, 0));
}

void PhysicsEngine::ShutDown()
{
	if (mDynamicsWorld) {
		mDynamicsWorld = nullptr;
	}

}

void PhysicsEngine::Update(float time)
{

	mDynamicsWorld->stepSimulation(time / 1000);
}
