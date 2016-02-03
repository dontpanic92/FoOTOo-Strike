#include "ModelViewLevel.h"

bool ModelViewLevel::StartUp()
{
	ShutDown();
	mScene = new Scene();
	mScene->StartUp();

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

void ModelViewLevel::RotateCamera(int deltaX, int deltaY)
{
	Transform* t = mScene->GetCurrentCamera()->GetTransform();
	float ratio = 0.02;
	Matrix4x4f m = t->GetTransformMatrix();

	t->RotateByRadian(-deltaX * ratio, m[1][0], m[1][1], m[1][2], Transform::World);
	t->RotateByRadian(-deltaY * ratio, m[0][0], m[0][1], m[0][2], Transform::World);
}

bool ModelViewLevel::Load(const char* path)
{
	/*if (mSceneObject) {
		mScene->GetRoot()->Detach(mSceneObject);
		delete mSceneObject;
	}*/

	ShutDown();
	StartUp();
	mSceneObject = new StaticSceneObject(path);
	mScene->GetRoot()->Attach(mSceneObject);

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
