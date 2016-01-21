
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

	Transform worldTran = GetScene()->GetCurrentCamera()->GetWorldMatrix();
	Vector3f v = worldTran.GetPosition();
	Vector3f toVector = Vector3f(0, 0, -1000) * worldTran.GetTransformMatrix();

	printf("from: %f %f %f, to: %f %f %f\n", v[0], v[1], v[2], toVector[0], toVector[1], toVector[2]);

	class RaycastRecord
	{
	public:
		float IntersetParam = 999999.9f;
		const NewtonBody* Body = nullptr;
		const void* SelfEntity = nullptr;

		RaycastRecord(void* self) :SelfEntity(self){}

		static dFloat RaycastCallback(const NewtonBody* const body, const NewtonCollision* const collisionHit, const dFloat* const contact, const dFloat* const normal, dLong collisionID, void* const userData, dFloat intersetParam)
		{
			RaycastRecord* p = (RaycastRecord*)userData;
			return p->pRaycastCallback(body, collisionHit, contact, normal, collisionID, intersetParam);
		}
	private:

		dFloat pRaycastCallback(const NewtonBody* const body, const NewtonCollision* const collisionHit, const dFloat* const contact, const dFloat* const normal, dLong collisionID, dFloat intersetParam)
		{
			void* p = nullptr;
			p = NewtonBodyGetUserData(body);
			if (intersetParam < IntersetParam && (!p || p != SelfEntity)) {
				IntersetParam = intersetParam;
				Body = body;
			}

			return IntersetParam;
		}
	};

	RaycastRecord record(this->mActor);

	NewtonWorldRayCast(GetPhysicsWorld(), v, toVector, RaycastRecord::RaycastCallback, &record, nullptr, 0);

	if (record.Body != nullptr) {
		Actor* p = (Actor*)NewtonBodyGetUserData(record.Body);
		if (p) {
			printf("hitted! %s\n", p->GetRepr());
		}
	}

	/*btVector3 from(v[0], v[1], v[2]);
	btVector3 to(toVector[0], toVector[1],  toVector[2]);
	btCollisionWorld::AllHitsRayResultCallback allResults(from, to);
	allResults.m_flags |= btTriangleRaycastCallback::kF_KeepUnflippedNormal;
	GetPhysicsEngine()->GetWorld()->rayTest(from, to, allResults);
	if (allResults.m_hitFractions.size()) {
		printf("hit: %u\n", allResults.m_hitFractions.size());
		for (int i = 0; i < allResults.m_collisionObjects.size(); i++) {
			void* p = allResults.m_collisionObjects[i]->getUserPointer();
			if (p) {
				printf("hitted! %p\n", p);
			}
		}
	} else {
		printf("no hit\n");
	}
	*/
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