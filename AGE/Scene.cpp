#include "Scene.h"
#include "ResourceManager.h"
#include "Primitive.h"
#include "Math.h"
#include "OpenGLRenderer.h"

using namespace AGE;

SceneNode::SceneNode(){
}

SceneNode::~SceneNode(){
}

void SceneNode::Attach(Attachable* attach){
	mChildrenAttachable.push_back(attach);
}


Scene::Scene(){
	mCamera.SetParameters(35.0f, float(800)/float(600), 1.0f, 1000.0f);
	Vector3f v;
	v[2] = 20;
	mCamera.GetTransform()->Translate(v, Transform::World);
}

Scene::~Scene(){
	for(unsigned int i = 0; i < mAllAttachable.size(); i++){
		delete mAllAttachable[i];
	}
}

Renderable* Scene::LoadMesh(){
	Vector3f points[4];

	points[0].Set(10, -1, 10);
	points[1].Set(10, -1, -10);
	points[2].Set(-10, -1, -10);
	points[3].Set(-10, -1, 10);
	//Primitive::GetInstance()->CreatePlaneUnmanage(points);//
	Renderable * attObj = Primitive::GetInstance()->CreateTorusUnmanage();//
	attObj->GetTramsform()->Translate(Vector3f(0, 0, -2.5), Transform::Local);

	mAllAttachable.push_back(attObj);

	Renderable* attObj2 = Primitive::GetInstance()->CreatePlaneUnmanage(points);

	mAllAttachable.push_back(attObj2);

	return attObj;
}

void Scene::Render(){


	RenderEngine::GetInstance()->Begin();

	for(int i = 0; i < mAllAttachable.size(); i++){
		ShaderProgram* shader = dynamic_cast<Renderable*>(mAllAttachable[i])->GetShader();

		Transform* translate = mAllAttachable[i]->GetTramsform();
		GLfloat vBlack[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		FlatShader shaderData;
		shaderData.ColorVector = vBlack;

		shaderData.MVPMatrix = translate->GetTransformMatrix() * mCamera.GetTransform()->GetInverseTransformMatrix() * mCamera.GetProjectMatrix();

		shader->Begin(&shaderData);

		RenderEngine::GetInstance()->Render(dynamic_cast<Renderable*>(mAllAttachable[i]));
	}
	RenderEngine::GetInstance()->End();
}