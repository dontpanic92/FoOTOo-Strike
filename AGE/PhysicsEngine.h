#ifndef __AGE_PHYSICSENGINE_HEADER__
#define __AGE_PHYSICSENGINE_HEADER__

#include <newton/Newton.h>
#include "Singleton.h"
#include "Def.h"
#include "LinearMath.h"

namespace AGE
{
	class SceneNode;
	class AGE_EXPORT PhysicsEngine : public Singleton < PhysicsEngine >
	{
	public:
		PhysicsEngine();
		~PhysicsEngine();

		void StartUp();
		void ShutDown();

		void Update(float time);

		NewtonWorld* GetWorld() { return mWorld; }

		SceneNode* GetRaycastClosetObject(const Vector3& from, const Vector3& to, SceneNode* exclude);

	private:
		NewtonWorld* mWorld;
	};

	static inline PhysicsEngine* GetPhysicsEngine() { return PhysicsEngine::GetInstance(); }

	static inline NewtonWorld* GetPhysicsWorld() { return GetPhysicsEngine()->GetWorld(); }
}

#endif
