#include <stack>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "Engine.h"
#include "ResourceManager.h"
#include "Primitive.h"
#include "Math.h"
#include "RenderEngine.h"
#include "RenderQueue.h"
#include "ScenePrivate.h"


using namespace AGE;
using namespace boost::property_tree;

Scene::Scene(std::wstring name):Scene(new ScenePrivate)
{
	d->name = name;
}

Scene::Scene(ScenePrivate* d) :d(d)
{
	UpdateCameraAspectRatio();
}

void Scene::Load(const char* sceneFilePath)
{
	if (!sceneFilePath)
		return;

	std::function<void(wptree&, SceneNode*)> f = [&](wptree& pt,SceneNode* parentNode){
		auto s = pt.get_child_optional(L"sceneNode");
		if (s) {
			for (wptree::value_type &v : s.get()) {
				parentNode->Attach(this->CreateSceneNode(parentNode));
				f(v.second, nullptr);
			}
		}
	};

	//wptree pt;
	//read_xml(sceneFilePath, pt);
	//f(pt, &mRoot);
}

/*bool Scene::Save(const char* path)
{
	try {
		wptree pt;
		pt.put(L"Name", mName);

		std::function<void(wptree&, SceneNode*)> f = [&](wptree& pt, SceneNode* currentNode){
			
			pt.put(L"Name", mName);
			for (SceneNode* n : currentNode->GetChildren()) {
				auto c = pt.put(L"sceneNode,", L"");
				f(c, n);
			}
		};

		auto c = pt.put(L"sceneNode", L"");
		c.put(L"Name", L"Root");
		f(c, &mRoot);

		write_xml(path, pt);
	} catch (std::exception&) {
		return false;
	}
	return true;
}
*/
void Scene::UpdateCameraAspectRatio()
{
	Window window = Engine::GetInstance()->GetMainWindow();
	float aspectRatio = float(window.Width) / float(window.Height);
	d->camera.SetParameters(60.0f, aspectRatio, 1.0f, 10000.0f);
}

Scene::~Scene()
{
	for each(auto light in d->lights)
	{
		delete light;
	}

	if (d->skyBox) {
		delete d->skyBox;
	}

	delete d;
}

void Scene::AttachCameraOnNode(SceneNode* node)
{
	d->camera.SetParent(node);
}

void Scene::UpdateAndCulling()
{
	d->root.UpdateAndCulling(Matrix4x4f::MakeIdentity());
}

SceneNode* Scene::CreateSceneNode(SceneNode* parent)
{
	if (parent == NULL)
		parent = &d->root;
	SceneNode* node = new SceneNode();
	parent->Attach(node);
	return node;
}

Light* Scene::CreateLight(SceneNode* parent)
{
	if (parent == NULL)
		parent = &d->root;
	Light* light = new Light;
	//parent->Attach(light);
	d->lights.push_back(light);
	return light;
}

vector<Light*>& Scene::GetLights() 
{ 
	return d->lights; 
}

SkyBox* Scene::GetSkyBox()
{
	return d->skyBox;
}

void Scene::AddSkyBox()
{
	if (!d->skyBox) 
		d->skyBox = new SkyBox();
}

Camera* Scene::GetCurrentCamera()
{
	return &d->camera;
}

SceneNode* Scene::GetRoot()
{
	return &d->root;
}

ScenePrivate* Scene::dptr()
{
	return d;
}
