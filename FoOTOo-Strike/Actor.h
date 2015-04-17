#ifndef __AGE_ACTOR_HEADER__
#define __AGE_ACTOR_HEADER__

#include <AGE.h>

#include "AGEMeshImporter.h"
#include "AGESkeletonAnimationImporter.h"

#include <Logic.h>
#include "ActorLogic.h"

#include <bullet/BulletDynamics/Character/btKinematicCharacterController.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
using namespace AGE;

class Actor
{
public:

	Actor()
	{
		AGEMeshImporter importer;
		mRenderable = importer.LoadFromFile("../Resources/Models/c.AMESH");
		AGESkeletonAnimationImporter importer2;
		mSkeleton = mRenderable->GetSkeleton();
		SkeletonAnimation* idleAnimation = importer2.LoadFromeFile(mSkeleton, "../Resources/Models/c.AMESH.IDLE.AANIM");
		SkeletonAnimation* reloadAnimation = importer2.LoadFromeFile(mSkeleton, "../Resources/Models/c.AMESH.RELOAD.AANIM");
		SkeletonAnimation* shoot1Animation = importer2.LoadFromeFile(mSkeleton, "../Resources/Models/c.AMESH.SHOOT1.AANIM");
		mSkeleton->AddAnimation("default", idleAnimation);
		mSkeleton->AddAnimation("reload", reloadAnimation);
		mSkeleton->AddAnimation("shoot1", shoot1Animation);

		mLogic.NewState<ActorLogicIdle>(1, &mLogic, this);
		mLogic.NewState<ActorLogicReload>(2, &mLogic, this);
		mLogic.NewState<ActorLogicShoot1>(3, &mLogic, this);

		mLogic.SetInitialState(1);

		SoundClipin.Load("../Resources/Sounds/de_clipin.wav", false);
		SoundClipin.SetParent(mRenderable);
		SoundClipout.Load("../Resources/Sounds/de_clipout.wav", false);
		SoundClipout.SetParent(mRenderable);
		SoundShoot.Load("../Resources/Sounds/deagle-1.wav", false);
		SoundShoot.SetParent(mRenderable);
	}

	Renderable* GetRenderable() { return mRenderable; }

	Skeleton* GetSkeleton() { return mSkeleton; }

	void Update(float deltaTime)
	{
		AudioEngine::GetInstance()->SetListenerPosition(mRenderable->GetWorldTransform().GetPosition());
		SoundClipin.Update(); 
		SoundClipout.Update();
		SoundShoot.Update();
		mLogic.GetCurrentState()->Update(deltaTime);
	}

	void InitPhysics(const btTransform& startTransform)
	{
		mGhostObject = new btPairCachingGhostObject();
		mGhostObject->setWorldTransform(startTransform);
		//sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

		btScalar characterHeight = 10;
		btScalar characterWidth = 30;
		btConvexShape* capsule = new btCapsuleShape(characterWidth, characterHeight);
		mGhostObject->setCollisionShape(capsule);
		mGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

		btScalar stepHeight = btScalar(0);
		mCharacterController = new btKinematicCharacterController(mGhostObject, capsule, stepHeight);
		mCharacterController->setJumpSpeed(30);
		mCharacterController->setGravity(10);
	}

	btKinematicCharacterController* GetPhysicsController()
	{
		return mCharacterController;
	}

	btPairCachingGhostObject* GetPhysicsObject()
	{
		return mGhostObject;
	}

	Sound3D SoundShoot;
	Sound3D SoundClipin;
	Sound3D SoundClipout;

private:
	Logic mLogic;

	btKinematicCharacterController* mCharacterController;
	btPairCachingGhostObject* mGhostObject;
	Renderable* mRenderable;
	Skeleton* mSkeleton;

};

class AIActor
{
public:

	AIActor()
	{
		AGEMeshImporter importer;
		mRenderable = importer.LoadFromFile("../Resources/Models/leet.AMESH");
		AGESkeletonAnimationImporter importer2;
		mSkeleton = mRenderable->GetSkeleton();
		SkeletonAnimation* idleAnimation = importer2.LoadFromeFile(mSkeleton, "../Resources/Models/leet.AMESH.IDLE.AANIM");
		mSkeleton->AddAnimation("idle", idleAnimation);

		//mLogic.NewState<ActorLogicIdle>(1, &mLogic, this);
		//mLogic.NewState<ActorLogicReload>(2, &mLogic, this);
		//mLogic.NewState<ActorLogicShoot1>(3, &mLogic, this);

		//mLogic.SetInitialState(1);

		SoundClipin.Load("../Resources/Sounds/de_clipin.wav", false);
		SoundClipin.SetParent(mRenderable);
		SoundClipout.Load("../Resources/Sounds/de_clipout.wav", false);
		SoundClipout.SetParent(mRenderable);
		SoundShoot.Load("../Resources/Sounds/deagle-1.wav", false);
		SoundShoot.SetParent(mRenderable);

		mSkeleton->StartPlay("idle");
	}

	Renderable* GetRenderable() { return mRenderable; }

	Skeleton* GetSkeleton() { return mSkeleton; }

	void Update(float deltaTime)
	{
		//AudioEngine::GetInstance()->SetListenerPosition(mRenderable->GetWorldTransform().GetPosition());
		//SoundClipin.Update();
		//SoundClipout.Update();
		//SoundShoot.Update();
		//mLogic.GetCurrentState()->Update(deltaTime);
		btTransform trans = mCharacterController->getGhostObject()->getWorldTransform();
		btVector3 v2 = trans.getOrigin();
		mRenderable->GetParent()->GetTransform()->SetPosition(Vector3f(v2.x(), v2.y(), v2.z()));
		mSkeleton->Update(deltaTime);
	}

	void InitPhysics(const btTransform& startTransform)
	{
		mGhostObject = new btPairCachingGhostObject();
		mGhostObject->setWorldTransform(startTransform);
		//sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

		btScalar characterHeight = 10;
		btScalar characterWidth = 30;
		btConvexShape* capsule = new btCapsuleShape(characterWidth, characterHeight);
		mGhostObject->setCollisionShape(capsule);
		mGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

		//btRigidBody* ground = GetPhysicsEngine()->CreateRigidBody(0.f, startTransform, capsule);

		btScalar stepHeight = btScalar(0);
		mCharacterController = new btKinematicCharacterController(mGhostObject, capsule, stepHeight);
		mCharacterController->setJumpSpeed(30);
		mCharacterController->setGravity(10);
	}

	btKinematicCharacterController* GetPhysicsController()
	{
		return mCharacterController;
	}

	btPairCachingGhostObject* GetPhysicsObject()
	{
		return mGhostObject;
	}

	Sound3D SoundShoot;
	Sound3D SoundClipin;
	Sound3D SoundClipout;

private:
	Logic mLogic;

	btKinematicCharacterController* mCharacterController;
	btPairCachingGhostObject* mGhostObject;
	Renderable* mRenderable;
	Skeleton* mSkeleton;

};

#endif
