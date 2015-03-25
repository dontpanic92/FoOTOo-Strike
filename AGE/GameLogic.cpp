#include "GameLogic.h"
#include "Engine.h"
#include "AGEMeshImporter.h"
#include "AGESkeletonAnimationImporter.h"
#include "PhysicsEngine.h"

#include "Logic.h"

#include <bullet/BulletDynamics/Character/btKinematicCharacterController.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

using namespace AGE;
using namespace OIS;

SceneNode* CameraNode;


class KeyboardLogicEvent : public LogicEvent
{
public:
	int GetEventType()
	{
		return 0;
	}

	int KeyID;
};

class MouseLogicEvent : public LogicEvent
{
public:
	int GetEventType()
	{
		return 1;
	}

	MouseEvent mouseEvent;
};


class ActorLogicIdle : public LogicState
{
public:
	ActorLogicIdle(int id, Logic* logic, Skeleton* skel)
		:LogicState(id, logic), mSkeleton(skel)
	{
	}

	void Enter() override
	{
		mSkeleton->StartPlay("default");
	}

	void React(LogicEvent* evt) override
	{
		//if (evt->GetEventType() == 1) {
		//	MouseLogicEvent* e = (MouseLogicEvent*)evt;
		//	e->mouseEvent.
		//}
	}

	void Update(float deltaTime) override
	{
		mSkeleton->Update(deltaTime);
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
	void Exit() override
	{
		
	}
private:
	Skeleton* mSkeleton;
	MouseState state;
};

class ActorLogicReload : public LogicState
{
public:
	ActorLogicReload(int id, Logic* logic, Skeleton* skel)
		:LogicState(id, logic), mSkeleton(skel)
	{
	}

	virtual void Enter()
	{
		mSkeleton->StartPlay("reload", false);
	}

	void React(LogicEvent* evt) override
	{
	}

	virtual void Update(float deltaTime)
	{
		if (mSkeleton->IsPlaying())
			mSkeleton->Update(deltaTime);
		else
			mLogic->Transit(1);
	}

	virtual void Exit()
	{

	}
private:
	Skeleton* mSkeleton;
};


class ActorLogicShoot1 : public LogicState
{
public:
	ActorLogicShoot1(int id, Logic* logic, Skeleton* skel)
		:LogicState(id, logic), mSkeleton(skel)
	{
	}

	void Enter() override
	{
		mSkeleton->StartPlay("shoot1", false);
	}

	void Update(float deltaTime) override
	{
		if (mSkeleton->IsPlaying())
			mSkeleton->Update(deltaTime);
		else
			mLogic->Transit(1);
	}
	void Exit() override
	{

	}
private:
	Skeleton* mSkeleton;
};

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

		mLogic.NewState<ActorLogicIdle>(1, &mLogic, mSkeleton);
		mLogic.NewState<ActorLogicReload>(2, &mLogic, mSkeleton);
		mLogic.NewState<ActorLogicShoot1>(3, &mLogic, mSkeleton);

		mLogic.SetInitialState(1);
	}

	Renderable* GetRenderable() { return mRenderable; }

	Skeleton* GetSkeleton() { return mSkeleton; }

	void Update(float deltaTime)
	{
		mLogic.GetCurrentState()->Update(deltaTime);
	}

private:
	Logic mLogic;

	Renderable* mRenderable;
	Skeleton* mSkeleton;
};
Actor * actor;

void GameLogicImp::StartUp()
{
	InputEngine::GetInstance()->RegisterMouseListener(this);
	InputEngine::GetInstance()->RegisterKeyListener(this);

	AGEMeshImporter importer;
	Renderable* r1 = importer.LoadFromFile("../Resources/Models/a.AMESH");

	actor = new Actor;

	SceneNode* node2 = Engine::GetInstance()->GetScene()->CreateSceneNode();
	SceneNode* node = Engine::GetInstance()->GetScene()->CreateSceneNode();

	node2->Attach(actor->GetRenderable());
	node->Attach(r1);
	

	CameraNode = Engine::GetInstance()->GetScene()->CreateSceneNode();
	CameraNode->GetTransform()->SetPosition(Vector3f(0, 0, -50));
	CameraNode->Attach(node2);
	node2->GetTransform()->Translate(Vector3f(0, 0, -5));
	node2->GetTransform()->RotateByRadian(Deg2Rad(180), 0.0f, 1.0f, 0.0f);
	//Engine::GetInstance()->GetScene()->GetRoot()->Attach(node2);

	//Engine::GetInstance()->GetScene()->GetCurrentCamera()->SetParent(CameraNode);
	CameraNode->Attach(Engine::GetInstance()->GetScene()->GetCurrentCamera());
	//Engine::GetInstance()->GetScene()->GetRoot()->Attach(node);
	//Engine::GetInstance()->GetScene()->GetRoot()->Attach(CameraNode);
	Light* l = Engine::GetInstance()->GetScene()->CreateLight();
	l->Direction[0] = -1;
	l->Direction[1] = -1;
	l->Direction[2] = -1;
	InitPhysics(r1);
}
btKinematicCharacterController* m_character;
btPairCachingGhostObject* m_ghostObject;
void GameLogicImp::InitPhysics(Renderable *r1)
{

	int vertStride = sizeof(btVector3);
	int indexStride = 3 * sizeof(int);


	int totalTriangles = 0;

	for (int i = 0; i < r1->GetNumberOfRenderObjects(); i++) {
		const RenderObject * ro = r1->GetRenderObject(i);
		totalTriangles += ro->Mesh->GetNumberOfVertex() / 3;
	}


	int *indices = new int[totalTriangles * 3];
	int k = 0;

	btTransform	startTransform;
	startTransform.setIdentity();
	int stripe = 3 * sizeof(int);

	for (int i = 0; i < r1->GetNumberOfRenderObjects(); i++) {
		//GLushort *idx = r1->GetRenderObject(i)->Mesh->GetIndexData();
		for (int j = 0; j < r1->GetRenderObject(i)->Mesh->GetNumberOfVertex(); j++) {
			indices[k++] = k;// idx[j];
		}


		btTriangleIndexVertexArray *indexArray = new btTriangleIndexVertexArray(r1->GetRenderObject(i)->Mesh->GetNumberOfVertex() / 3, indices, stripe,
																				(int)r1->GetRenderObject(i)->Mesh->GetNumberOfVertex(), (btScalar*)r1->GetRenderObject(i)->Mesh->GetVertexData(), (int)sizeof(Mesh::Vertex));
		btCollisionShape *shape = new btBvhTriangleMeshShape(indexArray, true);
		btRigidBody* ground = GetPhysicsEngine()->CreateRigidBody(0.f, startTransform, shape);
	}



	//btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin (btVector3(111.0, 4000.0, 0.0));
	//startTransform.setOrigin(btVector3(-1400, 0, -1800));


	m_ghostObject = new btPairCachingGhostObject();
	m_ghostObject->setWorldTransform(startTransform);
	//sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

	btScalar characterHeight = 10;
	btScalar characterWidth = 30;
	btConvexShape* capsule = new btCapsuleShape(characterWidth, characterHeight);
	m_ghostObject->setCollisionShape(capsule);
	m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	btScalar stepHeight = btScalar(0);
	m_character = new btKinematicCharacterController(m_ghostObject, capsule, stepHeight);
	m_character->setJumpSpeed(30);
	m_character->setGravity(10);

	GetPhysicsEngine()->GetWorld()->addCollisionObject(m_ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	GetPhysicsEngine()->GetWorld()->addAction(m_character);
	GetPhysicsEngine()->GetWorld()->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

}
#include <iostream>

bool GameLogicImp::Update(float time)
{
	//Renderable* renderable = dynamic_cast<Renderable*>(Engine::GetInstance()->GetScene()->GetAttachable());
	//Transform* translate = renderable->GetTramsform();

	//float yRot = time * 60.0f / 1000.0f;
	//translate->Translate(Vector3f(0, 0, -time / 1000), Transform::Local);
	//translate->RotateByRadian(Deg2Rad(yRot), 0, 1, 0);

	//static int itime = 0;

	//if ((itime++) % 10 == 0)
	//if (!mSkeleton->IsPlaying())
	///	mSkeleton->StartPlay("default");
	///mSkeleton->Update(time);
	ProcessMouse(InputEngine::GetInstance()->GetMouseState());
	actor->Update(time);

	char keys[256];
	InputEngine::GetInstance()->GetKeyStates(keys);

	Transform* cameraTransform = CameraNode->GetTransform();// Engine::GetInstance()->GetScene()->GetCurrentCamera()->GetTransform();
	float speed = 0.5;// time / 5;// / 100;
	Vector3f v = Vector3f(0, 0, 0);
	if (keys[KC_W]) {
		//cameraTransform->Translate(Vector3f(0, 0, -speed));

		m_character->setWalkDirection(btVector3(0, 0, -speed));
		v[2] += -1;
	}

	if (keys[KC_S]) {
		//cameraTransform->Translate(Vector3f(0, 0, speed));
		m_character->setWalkDirection(btVector3(0, 0, speed));
		v[2] += 1;
	}

	if (keys[KC_A]) {
		//cameraTransform->Translate(Vector3f(-speed, 0, 0));
		m_character->setWalkDirection(btVector3(-speed, 0, 0));
		v[0] += -1;
	}

	if (keys[KC_D]) {
		//cameraTransform->Translate(Vector3f(speed, 0, 0));
		m_character->setWalkDirection(btVector3(speed, 0, 0));
		v[0] += 1;
	}

	if (keys[KC_LCONTROL] || keys[KC_RCONTROL]) {
		//cameraTransform->Translate(Vector3f(0, -speed, 0), Transform::World);
	}

	if (keys[KC_SPACE]) {
		//cameraTransform->Translate(Vector3f(0, speed, 0), Transform::World);
		m_character->jump();
	}

	v.Normalize();
	v = v * speed;


	//printf("%f %f %f\n", v[0], v[1], v[2]);

	Matrix3x3f ma = cameraTransform->GetTransformMatrix();
	v = v * ma;
	v[1] = 0;
	//btMatrix3x3 m;
	//m.setValue(ma[0][0], ma[0][1], ma[0][2], ma[1][0], ma[1][1], ma[1][2], ma[2][0], ma[2][1], ma[2][2]);
	//btScalar r, y, p;
	//m.getEulerYPR(r, y, p);
	//btQuaternion q(r, y, p);

	//v.rotate(q.getAxis(), q.getAngle());
	m_character->setWalkDirection(btVector3(v[0], v[1], v[2]));

	//if (v.x() != 0 || v.y() != 0 || v.z() != 0)
	//std::cout << v.x() << " " << v.y() << " " << v.z() << std::endl;

	btTransform trans = m_character->getGhostObject()->getWorldTransform();
	btVector3 v2 = trans.getOrigin();
	cameraTransform->SetPosition(Vector3f(v2.x(), v2.y(), v2.z()));
	//std::cout << v.x() << " " << v.y() << " " << v.z() << std::endl;
	return true;
}

void GameLogicImp::ProcessMouse(const MouseState &state)
{
	Transform* cameraTransform = CameraNode->GetTransform();// Engine::GetInstance()->GetScene()->GetCurrentCamera()->GetTransform();
	static float pitchDegree = 0.0f;
	static float yawDegree = 0.0f;

	float speed = -state.X.rel / 5.0f;
	float yspeed = -state.Y.rel / 5.0f;

	pitchDegree += yspeed;
	yawDegree += speed;
	if (pitchDegree >= 90)
		pitchDegree = 89.9999f;
	if (pitchDegree <= -90)
		pitchDegree = -89.9999f;

	while (yawDegree > 360)
		yawDegree -= 360;

	while (yawDegree < -360)
		yawDegree += 360;

	cameraTransform->ClearRotation();
	cameraTransform->RotateByRadian(Deg2Rad(yawDegree), 0, 1, 0);
	cameraTransform->RotateByRadian(Deg2Rad(pitchDegree), 1, 0, 0);

	btTransform trans;
	trans.setIdentity();
	btQuaternion q;
	q.setEuler(yawDegree, 0, 0);
	m_character->getGhostObject()->getWorldTransform().setRotation(q);
	//cameraTransform->ClearRotation();
	//btQuaternion q = trans.getRotation();
	//btVector3 v3 = q.getAxis();
	//cameraTransform->RotateByRadian(Deg2Rad(q.getAngle()), v3.x(), v3.y(), v3.z());
}

bool GameLogicImp::mouseMoved(const MouseEvent &arg)
{
	//printf("abs: %d, rel: %d\n", arg.state.X.abs, arg.state.X.rel);
	ProcessMouse(arg.state);
	return true;
}
