#include "SimpleLevel.h"

bool SimpleLevel::StartUp()
{
	ShutDown();
	mScene = new Scene();
	mScene->StartUp();


	ground = new StaticSceneObject("../Resources/Models/a.AMESH");

	ActorNode = mScene->CreateSceneNode();
	SceneNode* node = mScene->CreateSceneNode();
	ActorNode->SetName("actornode");
	node->SetName("node");

	ActorNode->GetTransform()->SetPosition(Vector3f(111.0, 2000.0, 100.0));
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

	return true;
}

void SimpleLevel::ShutDown()
{
	delete mScene;
	mScene = 0;
}

bool SimpleLevel::Update(float time)
{
	return true;
}
