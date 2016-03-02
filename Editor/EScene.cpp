#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <ScenePrivate.h>
#include <SceneNode.h>
#include "EScene.h"
#include "Project.h"

using namespace boost::property_tree;

#define PROP_DECL(type, name) \
type name;\
const wchar_t* key_##name = L"scene."L#name

struct EScenePrivate : public AGE::ScenePrivate
{
	Project* project;
	EScene* p;

	PROP_DECL(QString, name);

	EScenePrivate(Project* project, EScene* p) :project(project), p(p) {}

	bool Load(const QString& name);
	bool Save();

	std::wstring GetMatrixString(AGE::SceneNode* node);
	QString GetSceneFilePath();
};

std::wstring EScenePrivate::GetMatrixString(AGE::SceneNode* node)
{
	float* m = node->GetTransform()->GetTransformMatrix();
	std::wstringstream ss;
	for (int i = 0; i < 16; i++) {
		ss << m[i] << ',';
	}
	std::wstring s;
	ss >> s;

	return s;
}

QString EScenePrivate::GetSceneFilePath()
{
	return project->GetScenesDir().filePath(name + ".ascene");
}

bool EScenePrivate::Load(const QString& name)
{
	this->name = name;
	QString path = GetSceneFilePath();
	try {
		wptree pt;
		read_xml(path.toStdString(), pt);

	} catch (exception&) {
		return false;
	}
	return true;
}

bool EScenePrivate::Save()
{
	QString path = GetSceneFilePath();
	printf("%s\n", path.toStdString().c_str());
	try {
		wptree pt;
		pt.put(L"test", L"bbbbb");

		std::function<void(wptree&, AGE::SceneNode*)> f = [&](wptree& pt, AGE::SceneNode* parentNode){
			for (AGE::SceneNode* n : parentNode->GetChildren()) {
				auto& c = pt.add(L"sceneNode", L"");
				c.put(L"Name", L"");
				c.put(L"Matrix", GetMatrixString(n));
				f(c, n);
			}
		};

		auto& c = pt.put(L"sceneNode", L"");
		f(c, &root);

		write_xml(path.toStdString(), pt);

	} catch (exception&) {
		return false;
	}
	return true;
}

EScene::EScene(Project* project) : Scene(new EScenePrivate(project, this))
{
}

EScene::~EScene()
{
}

shared_ptr<EScene> EScene::Load(const QString& name, Project* project)
{
	shared_ptr<EScene> p(new EScene(project));
	if (!p->dptr()->Load(name))
		return nullptr;
	return p;
}

shared_ptr<EScene> EScene::New(const QString& name, Project* project)
{
	shared_ptr<EScene> p(new EScene(project));
	p->dptr()->name = name;
	return p;
}

bool EScene::Save()
{
	return dptr()->Save();
}

EScenePrivate* EScene::dptr()
{
	return (EScenePrivate*)Scene::dptr();
}
