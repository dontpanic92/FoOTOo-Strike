#include "Engine.h"
#include "ResourceManager.h"
#include "Primitive.h"
#include "Math.h"
#include "OpenGLRenderer.h"

using namespace AGE;

SceneNode::SceneNode() : mParent(0){
}

SceneNode::~SceneNode(){
}

void SceneNode::Attach(Attachable* attach){
	mAttachable.push_back(attach);
}

void SceneNode::Attach(SceneNode* attach){
	mNodes.push_back(attach);
}

void SceneNode::Render(const Matrix4x4f& parentMatrix) {

	//const vector<Attachable*>& attachable = mRoot.GetAttachable();
	Matrix4x4f currentMatrix = mTransform.GetTransformMatrix() * parentMatrix;
	for (int i = 0; i < mAttachable.size(); i++){
		ShaderProgram* shader = dynamic_cast<Renderable*>(mAttachable[i])->GetShader();
		//dynamic_cast<Renderable*>(attachable[i])->GetTexture()->Use();

		Transform* translate = mAttachable[i]->GetTramsform();
		GLfloat vBlack[] = { .6f, 0.6f, 0.6f, 1.0f };
		Shader shaderData;
		shaderData.ColorVector = vBlack;

		shaderData.MMatrix = currentMatrix * translate->GetTransformMatrix();
		shaderData.VMatrix = Engine::GetInstance()->GetScene()->GetCurrentCamera()->CalcViewMatrix();//GetTransform()->GetInverseTransformMatrix();
		shaderData.PMatrix = Engine::GetInstance()->GetScene()->GetCurrentCamera()->GetProjectMatrix();

		shader->Begin(&shaderData);

		RenderEngine::GetInstance()->Render(dynamic_cast<Renderable*>(mAttachable[i]));
		//shader->End();
	}

	

	for (uint i = 0; i < mNodes.size(); i++){
		mNodes[i]->Render(currentMatrix);
	}
}

Scene::Scene(){
	mCamera.SetParameters(60.f, float(800)/float(600), 1.0f, 10000.0f);
	//Vector3f v;
	//v[2] = 5;
	//mCamera.GetTransform()->Translate(v);
}

Scene::~Scene(){
	for(unsigned int i = 0; i < mAttachable.size(); i++){
		delete mAttachable[i];
	}
}

void Scene::AttachCameraOnNode(SceneNode* node) { 
	mCamera.SetParent(node);
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

	mAttachable.push_back(attObj);

	Renderable* attObj2 = Primitive::GetInstance()->CreatePlaneUnmanage(points);

	mAttachable.push_back(attObj2);

	return attObj;
}

void Scene::Render(){


	RenderEngine::GetInstance()->Begin();
	for (int i = 0; i < mAttachable.size(); i++){
		ShaderProgram* shader = dynamic_cast<Renderable*>(mAttachable[i])->GetShader();
		//dynamic_cast<Renderable*>(mAttachable[i])->GetTexture()->Use();
		Transform* translate = mAttachable[i]->GetTramsform();
		GLfloat vBlack[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		Shader shaderData;
		shaderData.ColorVector = vBlack;

		shaderData.MMatrix = translate->GetTransformMatrix();
		shaderData.VMatrix = mCamera.GetTransform()->GetInverseTransformMatrix();
		shaderData.PMatrix = mCamera.GetProjectMatrix();

		shader->Begin(&shaderData);

		RenderEngine::GetInstance()->Render(dynamic_cast<Renderable*>(mAttachable[i]));
	}

	mRoot.Render(Matrix4x4f::Identity);

	RenderEngine::GetInstance()->End();
}