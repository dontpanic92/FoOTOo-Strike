#include "Engine.h"
#include "ResourceManager.h"
#include "Primitive.h"
#include "Math.h"
#include "OpenGLRenderer.h"
#include "RenderQueue.h"
#include <stack>

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

void SceneNode::Render(const Matrix4x4f& parentMatrix, const Matrix4x4f & viewMatrix) {

	//const vector<Attachable*>& attachable = mRoot.GetAttachable();
	Matrix4x4f currentMatrix = mTransform.GetTransformMatrix() * parentMatrix;
	for (int i = 0; i < mAttachable.size(); i++){
		ShaderProgram* shader = dynamic_cast<Renderable*>(mAttachable[i])->GetRenderObject(0)->Material->GetShaderProgram();

		Transform* translate = mAttachable[i]->GetTramsform();
		GLfloat vBlack[] = { .6f, 0.6f, 0.6f, 1.0f };
		DefaultShaderData shaderData;
		shaderData.ColorVector = vBlack;

		shaderData.MMatrix = translate->GetTransformMatrix() * currentMatrix;
		shaderData.VMatrix = viewMatrix; //GetTransform()->GetInverseTransformMatrix();
		shaderData.PMatrix = Engine::GetInstance()->GetScene()->GetCurrentCamera()->GetProjectMatrix();

		shader->Begin();
		shader->UpdateShaderData(&shaderData);

		RenderEngine::GetInstance()->Render(dynamic_cast<Renderable*>(mAttachable[i]));
		//shader->End();
	}

	for (uint i = 0; i < mNodes.size(); i++){
		mNodes[i]->Render(currentMatrix, viewMatrix);
	}
}

void SceneNode::UpdateAndCulling(const Matrix4x4f& parentMatrix)
{
	Matrix4x4f currentMatrix = mTransform.GetTransformMatrix() * parentMatrix;
	for each(auto attachable in mAttachable) {
		attachable->UpdateWorldMatrix(currentMatrix);
		RenderQueue::GetInstance()->PushRenderable(dynamic_cast<const Renderable*>(attachable));
	}

	for each(auto node in mNodes) 
	{
		node->UpdateAndCulling(currentMatrix);
	}
}

Matrix4x4f SceneNode::CalcWorldTransformMatrix() {
	Matrix4x4f matrix;
	std::stack<Matrix4x4f> s;
	s.push(this->mTransform.GetTransformMatrix());
	SceneNode* parent = mParent;
	while (parent != NULL) {
		s.push(parent->GetTransform()->GetTransformMatrix());
		parent = parent->GetParent();
	}

	while (!s.empty()) {
		matrix = matrix * s.top();
		s.pop();
	}

	return matrix;
}

Scene::Scene(){}

void Scene::StartUp() {
	Window window = Engine::GetInstance()->GetMainWindow();
	mCamera.SetParameters(60.f, float(window.Width) / float(window.Height), 1.0f, 10000.0f);
}

Scene::~Scene(){
	//for(unsigned int i = 0; i < mAttachable.size(); i++){
	//	delete mAttachable[i];
	//}
}

void Scene::AttachCameraOnNode(SceneNode* node) { 
	mCamera.SetParent(node);
}

/*Renderable* Scene::LoadMesh(){
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
*/
void Scene::Render(){

	Matrix4x4f viewMatrix = mCamera.CalcViewMatrix();

	RenderEngine::GetInstance()->Begin();
	/*for (int i = 0; i < mAttachable.size(); i++){
		ShaderProgram* shader = dynamic_cast<Renderable*>(mAttachable[i])->GetShader();
		//dynamic_cast<Renderable*>(mAttachable[i])->GetTexture()->Use();
		Transform* translate = mAttachable[i]->GetTramsform();
		GLfloat vBlack[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		Shader shaderData;
		shaderData.ColorVector = vBlack;

		shaderData.MMatrix = translate->GetTransformMatrix();
		shaderData.VMatrix = viewMatrix;// mCamera.GetTransform()->GetInverseTransformMatrix();
		shaderData.PMatrix = mCamera.GetProjectMatrix();

		shader->Begin(&shaderData);

		RenderEngine::GetInstance()->Render(dynamic_cast<Renderable*>(mAttachable[i]));
	}
	*/
	mRoot.Render(Matrix4x4f::Identity, viewMatrix);

	RenderEngine::GetInstance()->End();
}

void Scene::UpdateAndCulling()
{
	mRoot.UpdateAndCulling(Matrix4x4f::Identity);
}
