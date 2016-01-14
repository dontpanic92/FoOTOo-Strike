#include "SimpleLevel.h"

bool SimpleLevel::StartUp()
{
	ShutDown();
	mScene = new Scene();
	mScene->StartUp();

	InputEngine::GetInstance()->RegisterMouseListener(this);
	InputEngine::GetInstance()->RegisterKeyListener(this);

	//GetPhysicsWorld()->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	
	ground = new StaticSceneObject("../Resources/Models/a.AMESH");
	actor = new FootooActor;
	leet = new AIActor;

	SceneNode* leetNode = mScene->CreateSceneNode();
	leetNode->Attach(leet);
	leetNode->SetName("leetNode");

	ActorNode = mScene->CreateSceneNode();
	SceneNode* node = mScene->CreateSceneNode();
	ActorNode->SetName("actornode");
	node->SetName("node");

	ActorNode->Attach(actor);
	node->Attach(ground);


	SceneNode* CameraNode = mScene->CreateSceneNode();

	CameraNode->SetName("cameraNode");
	ActorNode->Attach(CameraNode);

	CameraNode->GetTransform()->SetPosition(Vector3f(0, 0, -5));
	
	//node2->GetTransform()->Translate(Vector3f(0, 0, -5));
	CameraNode->GetTransform()->RotateByRadian(Deg2Rad(180), 0.0f, 1.0f, 0.0f);
	//Engine::GetInstance()->GetScene()->GetRoot()->Attach(node2);

	CameraNode->Attach(mScene->GetCurrentCamera());
	Light* l = mScene->CreateLight();
	l->Direction[0] = -1;
	l->Direction[1] = -1;
	l->Direction[2] = -1;


	Enter();

	return true;
}

void SimpleLevel::ShutDown()
{
	delete mScene;
	mScene = 0;
}

void SimpleLevel::Enter()
{
	btTransform startTransform;
	startTransform.setOrigin(btVector3(111.0, 4000.0, 100.0));
	actor->GetPhysicsObject()->setWorldTransform(startTransform);

	startTransform.setOrigin(btVector3(200.0, 4000.0, 100.0));
	leet->GetPhysicsObject()->setWorldTransform(startTransform);
}

void SimpleLevel::Exit()
{

}

bool SimpleLevel::Update(float time)
{
	ProcessMouse(InputEngine::GetInstance()->GetMouseState());
	actor->Update(time);
	leet->Update(time);

	char keys[256];
	InputEngine::GetInstance()->GetKeyStates(keys);

	Transform* cameraTransform = ActorNode->GetTransform();
	float speed = 4;

	auto aController = actor->GetActorController();
	Vector3f v = Vector3f(0, 0, 0);
	if (keys[KC_W]) {
		//cameraTransform->Translate(Vector3f(0, 0, -speed));

		//aController->setWalkDirection(btVector3(0, 0, -speed));
		v[2] += 1;
	}

	if (keys[KC_S]) {
		//cameraTransform->Translate(Vector3f(0, 0, speed));
		//aController->setWalkDirection(btVector3(0, 0, speed));
		v[2] += -1;
	}

	if (keys[KC_A]) {
		//cameraTransform->Translate(Vector3f(-speed, 0, 0));
		//aController->setWalkDirection(btVector3(-speed, 0, 0));
		v[0] += 1;
	}

	if (keys[KC_D]) {
		//cameraTransform->Translate(Vector3f(speed, 0, 0));
		//aController->setWalkDirection(btVector3(speed, 0, 0));
		v[0] += -1;
	}

	if (keys[KC_LCONTROL] || keys[KC_RCONTROL]) {
		//cameraTransform->Translate(Vector3f(0, -speed, 0), Transform::World);
	}

	if (keys[KC_SPACE]) {
		//cameraTransform->Translate(Vector3f(0, speed, 0), Transform::World);
		aController->jump();
	}

	//printf("%f %f %f\n", v[0], v[1], v[2]);

	Matrix3x3f ma = cameraTransform->GetTransformMatrix();
	v = v * ma;
	v[1] = 0;
	v.Normalize();
	v = v * speed;
	//btMatrix3x3 m;
	//m.setValue(ma[0][0], ma[0][1], ma[0][2], ma[1][0], ma[1][1], ma[1][2], ma[2][0], ma[2][1], ma[2][2]);
	//btScalar r, y, p;
	//m.getEulerYPR(r, y, p);
	//btQuaternion q(r, y, p);

	//v.rotate(q.getAxis(), q.getAngle());
	aController->setWalkDirection(btVector3(v[0], v[1], v[2]));

	//if (v.x() != 0 || v.y() != 0 || v.z() != 0)
	//std::cout << v.x() << " " << v.y() << " " << v.z() << std::endl;

	///////btTransform trans = aController->getGhostObject()->getWorldTransform();
	///////btVector3 v2 = trans.getOrigin();
	///////cameraTransform->SetPosition(Vector3f(v2.x(), v2.y(), v2.z()));
	//std::cout << v.x() << " " << v.y() << " " << v.z() << std::endl;
	return true;
}

void SimpleLevel::ProcessMouse(const MouseState &state)
{
	Transform* cameraTransform = ActorNode->GetTransform();// Engine::GetInstance()->GetScene()->GetCurrentCamera()->GetTransform();
	static float pitchDegree = 0.0f;
	static float yawDegree = 0.0f;

	float speed = -state.X.rel / 5.0f;
	float yspeed = state.Y.rel / 5.0f;

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

	////////btTransform trans;
	////////trans.setIdentity();
	////////btQuaternion q;
	////////q.setEuler(yawDegree, 0, 0);
	//////actor->GetPhysicsObject()->getWorldTransform().setRotation(q);
	//cameraTransform->ClearRotation();
	//btQuaternion q = trans.getRotation();
	//btVector3 v3 = q.getAxis();
	//cameraTransform->RotateByRadian(Deg2Rad(q.getAngle()), v3.x(), v3.y(), v3.z());
}
