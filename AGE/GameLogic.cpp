#include "GameLogic.h"
#include "Engine.h"
#include "AGEMeshImporter.h"
#include "AGESkeletonAnimationImporter.h"
using namespace AGE;
using namespace OIS;

SceneNode* CameraNode;

void GameLogicImp::StartUp(){
	InputEngine::GetInstance()->RegisterMouseListener(this);
	InputEngine::GetInstance()->RegisterKeyListener(this);

	AGEMeshImporter importer;
	Renderable* r1 = importer.LoadFromFile("../Resources/Models/b.AMESH");
	Renderable* r2 = importer.LoadFromFile("../Resources/Models/c.AMESH");
	AGESkeletonAnimationImporter importer2;
	mSkeleton = r2->GetSkeleton();
	SkeletonAnimation* animation = importer2.LoadFromeFile(r2->GetNumberOfVertex(), mSkeleton, "../Resources/Models/c.AMESH.ABONE");
	mSkeleton->AddAnimation("default", animation);

	SceneNode* node = new SceneNode();
	SceneNode* node2 = new SceneNode();

	node->Attach(r1);
	node2->Attach(r2);

	CameraNode = new SceneNode();
	CameraNode->Attach(node2);
	//node2->GetTransform()->Translate(Vector3f(0, 0, -5));
	node2->GetTransform()->RotateByRadian(Deg2Rad(180), 0.0f, 1.0f, 0.0f);
	Engine::GetInstance()->GetScene()->GetRoot()->Attach(node2);

	Engine::GetInstance()->GetScene()->GetCurrentCamera()->SetParent(CameraNode);
	Engine::GetInstance()->GetScene()->GetRoot()->Attach(node);
	Engine::GetInstance()->GetScene()->GetRoot()->Attach(CameraNode);
}

bool GameLogicImp::Update(float time){
	//Renderable* renderable = dynamic_cast<Renderable*>(Engine::GetInstance()->GetScene()->GetAttachable());
	//Transform* translate = renderable->GetTramsform();

	//float yRot = time * 60.0f / 1000.0f;
	//translate->Translate(Vector3f(0, 0, -time / 1000), Transform::Local);
	//translate->RotateByRadian(Deg2Rad(yRot), 0, 1, 0);

	static int itime = 0;
	if ((itime++) % 10 == 0)
		mSkeleton->Update();

	char keys[256];
	InputEngine::GetInstance()->GetKeyStates(keys);

	Transform* cameraTransform = CameraNode->GetTransform();// Engine::GetInstance()->GetScene()->GetCurrentCamera()->GetTransform();
	float speed = time / 50;// / 100;

	if(keys[KC_W]){
		cameraTransform->Translate(Vector3f(0, 0, -speed));
	}

	if(keys[KC_S]){
		cameraTransform->Translate(Vector3f(0, 0, speed));
	}

	if(keys[KC_A]){
		cameraTransform->Translate(Vector3f(-speed, 0, 0));
	}

	if(keys[KC_D]){
		cameraTransform->Translate(Vector3f(speed, 0, 0));
	}

	if(keys[KC_LCONTROL] || keys[KC_RCONTROL]){
		cameraTransform->Translate(Vector3f(0, -speed, 0), Transform::World);
	}

	if(keys[KC_SPACE]){
		cameraTransform->Translate(Vector3f(0, speed, 0), Transform::World);
	}

	return true;
}

bool GameLogicImp::mouseMoved( const MouseEvent &arg ){
	//printf("abs: %d, rel: %d\n", arg.state.X.abs, arg.state.X.rel);
	Transform* cameraTransform = CameraNode->GetTransform();// Engine::GetInstance()->GetScene()->GetCurrentCamera()->GetTransform();
	static float pitchDegree = 0.0f;
	static float yawDegree = 0.0f;

	float speed = - arg.state.X.rel / 5.0f;
	float yspeed = - arg.state.Y.rel / 5.0f;

	pitchDegree += yspeed;
	yawDegree += speed;
	if(pitchDegree >= 90)
		pitchDegree = 89.9999f;
	if(pitchDegree <= -90)
		pitchDegree = -89.9999f;

	while(yawDegree > 360)
		yawDegree -= 360;

	while(yawDegree < -360)
		yawDegree += 360;

	cameraTransform->ClearRotation();
	cameraTransform->RotateByRadian(Deg2Rad(yawDegree), 0, 1, 0);
	cameraTransform->RotateByRadian(Deg2Rad(pitchDegree), 1, 0, 0);

	return true;
}
