#include "ModelViewLevel.h"

bool ModelViewLevel::StartUp()
{
	ShutDown();
	mScene = new Scene();
	mScene->StartUp();


	mSceneObject = new StaticSceneObject("../Resources/Models/c.AMESH");

	SceneNode* node = mScene->CreateSceneNode();
	node->SetName("node");
	node->Attach(mSceneObject);


	SceneNode* CameraNode = mScene->CreateSceneNode();
	CameraNode->SetName("cameraNode");
	CameraNode->GetTransform()->SetPosition(Vector3f(0, 0, 50));
	//CameraNode->GetTransform()->RotateByRadian(Deg2Rad(180), 0.0f, 1.0f, 0.0f);

	CameraNode->Attach(mScene->GetCurrentCamera());
	Light* l = mScene->CreateLight();
	l->Direction[0] = -1;
	l->Direction[1] = -1;
	l->Direction[2] = -1;

	return true;
}

void ModelViewLevel::ShutDown()
{
	delete mScene;
	mScene = 0;
}

bool ModelViewLevel::Update(float time)
{
	return true;
}
