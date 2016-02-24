#include "MetaLevel.h"
#include "RenderWidget.h"

using namespace AGE;

MetaLevel::MetaLevel(RenderWidget* widget) :mWidget(widget)
{
}

bool MetaLevel::StartUp()
{
	ShutDown();
	mScene = new Scene();
	mScene->StartUp();
	Light* l = mScene->CreateLight();
	l->Direction[0] = -1;
	l->Direction[1] = -1;
	l->Direction[2] = -1;
	return true;
}

void MetaLevel::ShutDown()
{
	UnloadScene();
}

bool MetaLevel::Update(float time)
{
	return true;
}

void MetaLevel::RotateCamera(int deltaX, int deltaY)
{
	auto f = [&](Transform* t){
		float ratio = 0.005;
		Matrix4x4f m = t->GetTransformMatrix();

		t->RotateByRadian(deltaY * ratio, m[0][0], m[0][1], m[0][2], Transform::World);
		t->RotateByRadian(-deltaX * ratio, 0, 1, 0, Transform::World);
	};
	f(mScene->GetCurrentCamera()->GetTransform());
}

void MetaLevel::AdjustDistance(int adjust)
{
	Camera* c = mScene->GetCurrentCamera();
	Vector3 v = c->GetTransform()->GetPosition();
	float len = v.GetLength() + -adjust / abs(adjust) * 5;

	v.Normalize();
	v = v * len;
	c->GetTransform()->SetPosition(v);
}

void MetaLevel::AddPrimitive(PrimitiveType type)
{
	AGE::Mesh* mesh = nullptr;
	switch (type) {
	case PrimitiveType::Plane:
		mesh = Primitive::GetInstance()->CreateRectangleUnmanage(20, 20);
		break;
	case PrimitiveType::IcoSphere:
		mesh = Primitive::GetInstance()->CreateSphereUnmanage(15);
		break;
	default:
		return;
	}

	auto r = new Renderable();
	Shader* shader = ResourceManager::GetInstance()->LoadShader("DefaultLight");
	Material* material = ResourceManager::GetInstance()->LoadMaterial("NoMaterial");
	r->AddRenderObject(RenderEngine::GetInstance()->CreateRenderObject(r, mesh, material, shader, true));

	auto obj = new SceneNode();
	obj->SetRenderable(r);

	mScene->GetRoot()->Attach(obj);
	mWidget->update();
}

void MetaLevel::LoadScene(EScene* scene)
{
	mScene = new Scene();
	mScene->StartUp();

	Camera* c = mScene->GetCurrentCamera();
	mScene->GetRoot()->Attach(c);
	c->GetTransform()->SetPosition(Vector3f(0, 0, -50));
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

	mPlaneObject = new PhysicsNode(mPlane);

	mScene->GetRoot()->Attach(mPlaneObject);
}

void MetaLevel::UnloadScene()
{
	delete mScene;
	mScene = 0;
}
