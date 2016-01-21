#ifndef __AGE_PHYSICSENGINE_HEADER__
#define __AGE_PHYSICSENGINE_HEADER__

#include <newton/Newton.h>
#include "Singleton.h"
#include "Def.h"
#include "LinearMath.h"

namespace AGE
{
	class Renderable;

	class AGE_EXPORT PhysicsEngine : public Singleton < PhysicsEngine >
	{
	public:
		PhysicsEngine();
		~PhysicsEngine();

		void StartUp();
		void ShutDown();

		void Update(float time);

		NewtonWorld* GetWorld() { return mWorld; }

		/*btRigidBody* CreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape, const btVector4& color = btVector4(1, 0, 0, 1))
		{
			btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);

			btVector3 localInertia(0, 0, 0);
			if (isDynamic)
				shape->calculateLocalInertia(mass, localInertia);

			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

			btRigidBody* body = new btRigidBody(mass, 0, shape, localInertia);
			body->setWorldTransform(startTransform);

			body->setUserIndex(-1);
			mDynamicsWorld->addRigidBody(body);
			return body;
		}*/

	private:
		NewtonWorld* mWorld;
	};

	static inline PhysicsEngine* GetPhysicsEngine() { return PhysicsEngine::GetInstance(); }

	static inline NewtonWorld* GetPhysicsWorld() { return GetPhysicsEngine()->GetWorld(); }
}

#endif
