#ifndef __AGE_FOOTOOACTOR_HEADER__
#define __AGE_FOOTOOACTOR_HEADER__

#include <AGE.h>

#include "AGEMeshImporter.h"
#include "AGESkeletonAnimationImporter.h"

#include <Logic.h>
#include "ActorLogic.h"

#include <newton/CustomPlayerControllerManager.h>

using namespace AGE;


class FootooActor : public AGE::Actor
{
public:

	FootooActor()
	{
		AGEMeshImporter importer;
		mRenderable = importer.LoadFromFile("../Resources/Models/c.AMESH", false);
		mRenderable->SetParent(this);
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
		SoundClipin.SetParent(this);
		SoundClipout.Load("../Resources/Sounds/de_clipout.wav", false);
		SoundClipout.SetParent(this);
		SoundShoot.Load("../Resources/Sounds/deagle-1.wav", false);
		SoundShoot.SetParent(this);
	}

	Skeleton* GetSkeleton() { return mSkeleton; }

	const char* GetRepr() override { return "FootooActor"; }

	void Update(float deltaTime)
	{
		AudioEngine::GetInstance()->SetListenerPosition(GetWorldTransform().GetPosition());
		SoundClipin.Update(); 
		SoundClipout.Update();
		SoundShoot.Update();
		mLogic.GetCurrentState()->Update(deltaTime);

		float m[3];
		NewtonBodyGetPosition(mController->GetBody(), m); 
		m[1] += mHeight / 2;
		GetParent()->GetTransform()->SetPosition(m);
	}

	Sound3D SoundShoot;
	Sound3D SoundClipin;
	Sound3D SoundClipout;

private:
	Logic mLogic;
	Skeleton* mSkeleton;

};

class AIActor : public Actor
{
public:

	AIActor()
	{
		AGEMeshImporter importer;
		mRenderable = importer.LoadFromFile("../Resources/Models/leet.AMESH", false);
		mRenderable->SetParent(this);
		AGESkeletonAnimationImporter importer2;
		mSkeleton = mRenderable->GetSkeleton();
		SkeletonAnimation* idleAnimation = importer2.LoadFromeFile(mSkeleton, "../Resources/Models/leet.AMESH.IDLE.AANIM");
		mSkeleton->AddAnimation("idle", idleAnimation);

		//mLogic.NewState<ActorLogicIdle>(1, &mLogic, this);
		//mLogic.NewState<ActorLogicReload>(2, &mLogic, this);
		//mLogic.NewState<ActorLogicShoot1>(3, &mLogic, this);

		//mLogic.SetInitialState(1);

		SoundClipin.Load("../Resources/Sounds/de_clipin.wav", false);
		SoundClipin.SetParent(this);
		SoundClipout.Load("../Resources/Sounds/de_clipout.wav", false);
		SoundClipout.SetParent(this);
		SoundShoot.Load("../Resources/Sounds/deagle-1.wav", false);
		SoundShoot.SetParent(this);

		mSkeleton->StartPlay("idle");
	}

	Skeleton* GetSkeleton() { return mSkeleton; }

	void Update(float deltaTime)
	{
		/*btTransform trans = mCharacterController->getGhostObject()->getWorldTransform();
		btVector3 v2 = trans.getOrigin();

		GetParent()->GetTransform()->SetPosition(Vector3f(v2.x(), v2.y(), v2.z()));*/
		float m[3];
		NewtonBodyGetPosition(mController->GetBody(), m);
		m[1] += mHeight / 2;
		GetParent()->GetTransform()->SetPosition(m);
		mSkeleton->Update(deltaTime);
	}

	Sound3D SoundShoot;
	Sound3D SoundClipin;
	Sound3D SoundClipout;

private:
	Logic mLogic;
	Skeleton* mSkeleton;
};

#endif
