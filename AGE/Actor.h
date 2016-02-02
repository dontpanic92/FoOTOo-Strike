#ifndef __AGE_ACTOR_HEADER__
#define __AGE_ACTOR_HEADER__

#include "PhysicsEngine.h"
#include "Singleton.h"
#include "InputEngine.h"
#include "DynamicSceneObject.h"

#include <newton/Newton.h>
#include <newton/CustomPlayerControllerManager.h>

namespace AGE
{


	class BasicPlayerController : public CustomPlayerController
	{
	public:
		virtual void PostUpdate(dFloat timestep, int threadIndex);
	};

	class BasicPlayerControllerManager : public CustomControllerManager<BasicPlayerController>
	{
	public:
		BasicPlayerControllerManager(NewtonWorld* const world)
			:CustomControllerManager<BasicPlayerController>(world, "AGE_PLAYERMANAGER")
		{}

		~BasicPlayerControllerManager() {}

		virtual void ApplyPlayerMove(CustomPlayerController* const controller, dFloat timestep);

		virtual BasicPlayerController* CreatePlayer(dFloat mass, dFloat outerRadius, dFloat innerRadius, dFloat height, dFloat stairStep, const dMatrix& localAxis);

		virtual int ProcessContacts(const CustomPlayerController* const controller, NewtonWorldConvexCastReturnInfo* const contacts, int count) const
		{
			return count;
		}
	};

	class AGE_EXPORT Actor : public SceneObject
	{
	public:
		Actor()
		{
			InitPhysics();
		}

		virtual ~Actor();

		virtual const char* GetRepr();

		void InitPhysics();

		Vector3f GetVelocity() { return mVelocity; }

		float GetHeadingAngle() { return mHeadingAngle; }

		CustomPlayerController* GetController() { return mController; }

		void SetPosition(Vector3f position);

		void SetVelocity(Vector3f v) { mVelocity = v; }

	protected:

		Vector3f mVelocity = Vector3f(0, 0, 0);
		float mHeadingAngle = 0;

		float mHeight = 75;
		float mRadius = 20;

		CustomPlayerController* mController;

		static BasicPlayerControllerManager* mPlayerManager;
	};
}

#endif
