#ifndef __AGE_ACTOR_HEADER__
#define __AGE_ACTOR_HEADER__

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "btKinematicCharacterController.h"
#include "PhysicsEngine.h"
#include "Singleton.h"
#include "InputEngine.h"
#include "DynamicSceneObject.h"

namespace AGE
{
	class AGE_EXPORT Actor : public RenderableSceneObject
	{
	public:
		Actor()
		{
			InitPhysics();
		}

		virtual ~Actor();

		virtual const char* GetRepr();

		void InitPhysics();

		btKinematicCharacterController* GetActorController()
		{
			return mCharacterController;
		}

		btPairCachingGhostObject* GetPhysicsObject()
		{
			return mGhostObject;
		}

	protected:
		btKinematicCharacterController* mCharacterController;
		btPairCachingGhostObject*		mGhostObject;
	};
}

#endif
