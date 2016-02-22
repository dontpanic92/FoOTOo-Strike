
#include "ActorLogic.h"
#include "FootooActor.h"

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

	Transform worldTran = GetScene()->GetCurrentCamera()->GetWorldTransform();
	Vector3f v = worldTran.GetPosition();
	Vector3f v2 = v + Vector3f(0, 0, 1000) * (Matrix3)worldTran.GetTransformMatrix();

	printf("from: %f %f %f, to: %f %f %f\n", v[0], v[1], v[2], v2[0], v2[1], v2[2]);

	auto p = GetPhysicsEngine()->GetRaycastClosetObject(v, v2, this->mActor);
	if (p) {
		printf("hitted! %s\n", p->GetName());
	}
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