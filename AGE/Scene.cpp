#include "Scene.h"
#include "ResourceManager.h"
#include "Primitive.h"
#include "Math.h"
#include "OpenGLRenderer.h"

#include <StopWatch.h>
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
	v[2] = -10;
	//v[2] = -10;
	mCamera.GetTransform()->Translate(v);
}

Scene::~Scene(){
	for(unsigned int i = 0; i < mAllAttachable.size(); i++){
		delete mAllAttachable[i];
	}
}

Renderable* Scene::LoadMesh(){
	Vector3f points[4];

	points[0].Set(0.5, 0.5, -1);
	points[1].Set(0.5, -0.5, -1);
	points[2].Set(-0.5, -0.5, -1);
	points[3].Set(-0.5, 0.5, -1);

	Renderable * attObj = Primitive::GetInstance()->CreatePlaneUnmanage(points);//ResourceManager::GetInstance()->LoadMeshUnmanage();
	attObj->GetTramsform()->Translate(Vector3f(0, 0, -2.5));

	mAllAttachable.push_back(attObj);

	return attObj;
}

void Scene::Render(){
	ShaderProgram* shader = dynamic_cast<Renderable*>(mAllAttachable[0])->GetShader();
	
	Transform* translate = mAllAttachable[0]->GetTramsform();

	RenderEngine::GetInstance()->Begin(translate->GetTransformMatrix());

	GLfloat vBlack[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	FlatShader shaderData;
	shaderData.ColorVector = vBlack;
	shaderData.MVPMatrix = translate->GetTransformMatrix() * mCamera.GetTransform()->GetTransformMatrix() * mCamera.GetProjectMatrix();

	shader->Begin(&shaderData);

	RenderEngine::GetInstance()->Render(dynamic_cast<Renderable*>(mAllAttachable[0]));
	RenderEngine::GetInstance()->End();
}