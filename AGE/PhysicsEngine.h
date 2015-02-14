#ifndef __AGE_PHYSICSENGINE_HEADER__
#define __AGE_PHYSICSENGINE_HEADER__

#include "Singleton.h"
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

namespace AGE
{
	class PhysicsEngine : public Singleton < PhysicsEngine >
	{
	public:
		PhysicsEngine();
		~PhysicsEngine();

		void StartUp();
		//void ShutDown();

		void Update(float time);

		btDynamicsWorld* GetWorld() { return mDynamicsWorld; }

		btRigidBody* CreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape);

	private:
		btBroadphaseInterface* mBroadphase;
		btDefaultCollisionConfiguration* mCollisionConfiguration;
		btCollisionDispatcher* mDispatcher;
		btSequentialImpulseConstraintSolver* mSolver;
		btDiscreteDynamicsWorld* mDynamicsWorld;
	};

	static inline PhysicsEngine* GetPhysicsEngine() { return PhysicsEngine::GetInstance(); }
}

#endif
