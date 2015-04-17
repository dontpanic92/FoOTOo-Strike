#include "ActorLogic.h"
#include "Actor.h"

void ActorLogicIdle::Enter()
{
	mActor->GetSkeleton()->StartPlay("default");
}

void ActorLogicIdle::Update(float deltaTime)
{
	mActor->GetSkeleton()->Update(deltaTime);
	char keys[256];
	InputEngine::GetInstance()->GetKeyStates(keys);
	if (keys[KC_R]) {
		mLogic->Transit(2);
	}

	const MouseState& state = InputEngine::GetInstance()->GetMouseState();
	if (state.buttonDown(OIS::MouseButtonID::MB_Left)) {
		mLogic->Transit(3);
	}
}

void ActorLogicReload::Enter()
{
	mActor->GetSkeleton()->StartPlay("reload", false);
}

void ActorLogicReload::Update(float deltaTime)
{
	if (mActor->GetSkeleton()->IsPlaying()) {
		mActor->GetSkeleton()->Update(deltaTime);
		uint fn = mActor->GetSkeleton()->GetCurrentFrameNumber();
		if (fn == 14) {
			mActor->SoundClipout.Play();
		} else if (fn == 34) {
			mActor->SoundClipin.Play();
		}
	}else
		mLogic->Transit(1);
}

void ActorLogicShoot1::Enter() 
{
	mActor->GetSkeleton()->StartPlay("shoot1", false);
}

void ActorLogicShoot1::Update(float deltaTime)
{
	if (mActor->GetSkeleton()->IsPlaying()) {
		mActor->GetSkeleton()->Update(deltaTime);
		uint fn = mActor->GetSkeleton()->GetCurrentFrameNumber();
		if (fn == 1) {
			//if (mActor->SoundShoot.IsPlaying()) {
			//	mActor->SoundShoot.Restart();
			//} else {
			mActor->SoundShoot.Stop();
			//AudioEngine::GetInstance()->Update();
			//}
		} else if (fn == 2) {

			mActor->SoundShoot.Play();
		}
	}else
		mLogic->Transit(1);
}