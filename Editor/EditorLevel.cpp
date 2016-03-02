#include "EditorLevel.h"
#include "RenderWidget.h"
#include "EScene.h"
#include "PropertySceneNode.h"

EditorLevel::EditorLevel(RenderWidget* widget) :mWidget(widget)
{
}

bool EditorLevel::StartUp()
{
	ShutDown();
	mScene = nullptr;
	return true;
}

void EditorLevel::ShutDown()
{
	mScene = nullptr;

	delete mPlaneObject;
	mPlaneObject = nullptr;
	delete mPlane;
	mPlane = nullptr;
}

bool EditorLevel::Update(float time)
{
	return true;
}

void EditorLevel::RotateCamera(int deltaX, int deltaY)
{
	using namespace AGE;

	if (!mScene)
		return;

	auto f = [&](Transform* t){
		float ratio = 0.005;
		Matrix4x4f m = t->GetTransformMatrix();

		t->RotateByRadian(deltaY * ratio, m[0][0], m[0][1], m[0][2], Transform::World);
		t->RotateByRadian(-deltaX * ratio, 0, 1, 0, Transform::World);
	};
	f(mScene->GetCurrentCamera()->GetTransform());

	mWidget->update();
}

void EditorLevel::AdjustDistance(int adjust)
{
	using namespace AGE;

	if (!mScene)
		return;

	Camera* c = mScene->GetCurrentCamera();
	Vector3 v = c->GetTransform()->GetPosition();
	float len = v.GetLength() + -adjust / abs(adjust) * 5;

	v.Normalize();
	v = v * len;
	c->GetTransform()->SetPosition(v);

	mWidget->update();
}

void EditorLevel::AddPrimitive(PrimitiveType type)
{
	using namespace AGE;

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

void EditorLevel::SetScene(EScene* scene)
{
	using namespace AGE;

	mScene = scene;
	if (!mScene)
		return;

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

	mWidget->update();
}

void EditorLevel::AddSceneNode(AGE::Renderable* r)
{
	using namespace AGE;

	PhysicsNode* node = new PhysicsNode(r);
	node->SetUserData(0, make_shared<PropertySceneNode>(mWidget, node));
	mScene->GetRoot()->Attach(node);
	mWidget->update();
}

void EditorLevel::AddPhysicsNode(AGE::Renderable* r)
{
	using namespace AGE;

	PhysicsNode* node = new PhysicsNode(r);
	mScene->GetRoot()->Attach(node);
	mWidget->update();
}
