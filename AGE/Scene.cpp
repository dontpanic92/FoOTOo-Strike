#include "Engine.h"
#include "ResourceManager.h"
#include "Primitive.h"
#include "Math.h"
#include "RenderEngine.h"
#include "RenderQueue.h"
#include <stack>

using namespace AGE;

//void SceneNode::Render(const Matrix4x4f& parentMatrix, const Matrix4x4f & viewMatrix)
//{

	//const vector<Attachable*>& attachable = mRoot.GetAttachable();
	/*Matrix4x4f currentMatrix = mTransform.GetTransformMatrix() * parentMatrix;
	for (int i = 0; i < mRenderables.size(); i++) {
		Shader* shader = mRenderables[i]->GetRenderObject(0)->Shader;

		Transform* translate = mRenderables[i]->GetTransform();
		GLfloat vBlack[] = { .6f, 0.6f, 0.6f, 1.0f };
		DefaultShaderData shaderData;
		shaderData.ColorVector = vBlack;

		///shaderData.MMatrix = translate->GetTransformMatrix() * currentMatrix;
		///shaderData.VMatrix = viewMatrix; //GetTransform()->GetInverseTransformMatrix();
		///shaderData.PMatrix = Engine::GetInstance()->GetScene()->GetCurrentCamera()->GetProjectMatrix();

		shader->Use();
		shader->UpdateShaderData(shaderData);

		//RenderEngine::GetInstance()->Render(dynamic_cast<Renderable*>(mAttachable[i]));
		//shader->End();
	}

	for (uint i = 0; i < mNodes.size(); i++) {
		mNodes[i]->Render(currentMatrix, viewMatrix);
	}*/
//}


/*Matrix4x4f SceneNode::CalcWorldTransformMatrix()
{
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
}*/

Scene::Scene():mRoot(nullptr) {}

void Scene::StartUp()
{
	UpdateCameraAspectRatio();
}

void Scene::UpdateCameraAspectRatio()
{
	Window window = Engine::GetInstance()->GetMainWindow();
	float aspectRatio = float(window.Width) / float(window.Height);
	mCamera.SetParameters(60.0f, aspectRatio, 1.0f, 10000.0f);
}

Scene::~Scene()
{
	for each(auto light in mLights)
	{
		delete light;
	}

	if (mSkyBox) {
		delete mSkyBox;
	}
}

void Scene::AttachCameraOnNode(SceneNode* node)
{
	mCamera.SetParent(node);
}


void Scene::Render()
{

	Matrix4x4f viewMatrix = mCamera.CalcViewMatrix();

	//RenderEngine::GetInstance()->Begin();
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
	//mRoot.Render(Matrix4x4f::MakeIdentity(), viewMatrix);

	//RenderEngine::GetInstance()->End();
}

void Scene::UpdateAndCulling()
{
	mRoot.UpdateAndCulling(Matrix4x4f::MakeIdentity());
}

SceneNode* Scene::CreateSceneNode(SceneNode* parent)
{
	if (parent == NULL)
		parent = &mRoot;
	SceneNode* node = new SceneNode();
	parent->Attach(node);
	return node;
}

Light* Scene::CreateLight(SceneNode* parent)
{
	if (parent == NULL)
		parent = &mRoot;
	Light* light = new Light;
	//parent->Attach(light);
	mLights.push_back(light);
	return light;
}
