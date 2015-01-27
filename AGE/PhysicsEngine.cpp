#include "PhysicsEngine.h"

using namespace AGE;

PhysicsEngine::PhysicsEngine() : mDispatcher(0), mDynamicsWorld(0), mSolver(0), mCollisionConfiguration(0), mBroadphase(0) {}

PhysicsEngine::~PhysicsEngine() {
	delete mDynamicsWorld;
	delete mSolver;
	delete mDispatcher;
	delete mCollisionConfiguration;
	delete mBroadphase;
}

void PhysicsEngine::StartUp() {
	mBroadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);

	// The actual physics solver
	mSolver = new btSequentialImpulseConstraintSolver;

	// The world.
	mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);
	mDynamicsWorld->setGravity(btVector3(0, -10, 0));
	mDynamicsWorld->getDispatchInfo().m_allowedCcdPenetration = 0.0001f;
}

//From Bullet Demo
btRigidBody* PhysicsEngine::CreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape) {
	btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

//#define USE_MOTIONSTATE
/*#ifdef USE_MOTIONSTATE
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);

	btRigidBody* body = new btRigidBody(cInfo);
	body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);

#else*/
	btRigidBody* body = new btRigidBody(mass, 0, shape, localInertia);
	body->setWorldTransform(startTransform);
//#endif

	mDynamicsWorld->addRigidBody(body);

	return body;
}

void PhysicsEngine::Update(float time) {
	mDynamicsWorld->stepSimulation(time * 0.01, 10);
}
