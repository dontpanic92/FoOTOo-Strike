#include "ModelViewLevel.h"

bool ModelViewLevel::StartUp()
{
	ShutDown();
	mScene = new Scene();
	mScene->StartUp();

	Camera* c = mScene->GetCurrentCamera();
	mScene->GetRoot()->Attach(c);
	c->GetTransform()->SetPosition(Vector3f(0, 0, 50));
	//CameraNode->GetTransform()->RotateByRadian(Deg2Rad(180), 0.0f, 1.0f, 0.0f);

	Light* l = mScene->CreateLight();
	l->Direction[0] = -1;
	l->Direction[1] = -1;
	l->Direction[2] = -1;

	mScene->AddSkyBox();

	mPlane = new Renderable();
	Mesh* m = Primitive::CreateRectangleUnmanage(100, 100);
	Material* material = ResourceManager::GetInstance()->LoadMaterial("wood-texture.jpg");
	Shader* shader = ResourceManager::GetInstance()->LoadShader("DefaultLight");
	mPlane->AddRenderObject(RenderEngine::GetInstance()->CreateRenderObject(mPlane, m, material, shader, true));

	mPlaneObject = new SceneObject();
	mPlaneObject->SetRenderable(mPlane);

	mScene->GetRoot()->Attach(mPlaneObject);

	return true;
}

void ModelViewLevel::RotateCamera(int deltaX, int deltaY)
{
	auto f = [&](Transform* t){
		float ratio = 0.005;
		Matrix4x4f m = t->GetTransformMatrix();

		//t->RotateByRadian(-deltaX * ratio, m[1][0], m[1][1], m[1][2], Transform::World);
		t->RotateByRadian(-deltaY * ratio, m[0][0], m[0][1], m[0][2], Transform::World);
		t->RotateByRadian(-deltaX * ratio, 0, 1, 0, Transform::World);
	};
	f(mScene->GetCurrentCamera()->GetTransform());
	//f(mScene->GetLights()[0]->GetTransform());
}

void ModelViewLevel::AdjustDistance(int adjust)
{
	
	Camera* c = mScene->GetCurrentCamera();
	Vector3 v = c->GetTransform()->GetPosition();
	float len = v.GetLength() + -adjust / abs(adjust) * 5;

	v.Normalize();
	v = v * len;
	c->GetTransform()->SetPosition(v);
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
