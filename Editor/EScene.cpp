#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "EScene.h"
#include "Project.h"

using namespace boost::property_tree;

#define PROP_DECL(type, name) \
type name;\
const wchar_t* key_##name = L"scene."L#name

struct EScenePrivate
{
	Project* project;
	EScene* p;

	PROP_DECL(QString, name);

	EScenePrivate(Project* project, EScene* p) :project(project), p(p) {}
	
	bool Load(const QString& name);
	bool Save();

	QString GetSceneFilePath();
};

QString EScenePrivate::GetSceneFilePath()
{
	return project->GetScenesDir().filePath(name + ".ascene");
}

bool EScenePrivate::Load(const QString& name)
{
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
		pt.put(L"test", L"hahaha");
		write_xml(path.toStdString(), pt);

	} catch (exception&) {
		return false;
	}
	return true;
}

EScene::EScene(Project* project) : d(new EScenePrivate(project, this))
{
}

EScene::~EScene()
{
	delete d;
}

shared_ptr<EScene> EScene::Load(const QString& name, Project* project)
{
	shared_ptr<EScene> p(new EScene(project));
	if (!p->d->Load(name))
		return nullptr;
	return p;
}

shared_ptr<EScene> EScene::New(const QString& name, Project* project)
{
	shared_ptr<EScene> p(new EScene(project));
	p->d->name = name;
	return p;
}

bool EScene::Save()
{
	return d->Save();
}
