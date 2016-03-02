#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <qfileinfo.h>
#include <qdir.h>
#include <vector>
#include "Project.h"
#include "EScene.h"

using namespace boost::property_tree;
using namespace std;

#define PROP_DECL(type, name) \
type name;\
const wchar_t* key_##name = L"project."L#name

#define PROP_DECL_CONTAINER(type, name) \
type name;\
const wchar_t* key_load_##name = L"project."L#name;\
const wchar_t* key_save_##name = L"project."L#name L".item"

const char* SCENES_FOLDER = "Scenes";

struct ProjectPrivate
{
	typedef vector<QString> ScenesType;

	Project* p;
	shared_ptr<EScene> currentScene;


	// Properties that will be saved in project file.

	PROP_DECL(QString, name);
	PROP_DECL_CONTAINER(ScenesType, scenes);

	QString dirPath;
	QString fileName;

	ProjectPrivate(Project* pub) : p(pub) {}
	
	bool Load(const QString& filePath);
	bool New(const QString& n, const QString& p);
	bool Save();
};

bool ProjectPrivate::Load(const QString& filePath)
{

	try {
		wptree pt;
		read_xml(filePath.toStdString(), pt);

		name = QString::fromStdWString(pt.get<wstring>(key_name));

		auto s = pt.get_child_optional(key_load_scenes);
		if (s) {
			for (wptree::value_type &v : s.get()) {
				scenes.push_back(QString::fromStdWString(v.second.data()));
			}
		}

		fileName = QFileInfo(filePath).fileName();
		dirPath = QFileInfo(filePath).absolutePath();
	} catch (exception&) {
		return false;
	}
	return true;
}

bool ProjectPrivate::New(const QString& n, const QString& p)
{
	name = n;
	dirPath = p;
	fileName = n + ".aproj";
	return true;
}

bool ProjectPrivate::Save()
{
	try {
		wptree pt;

		pt.put(key_name, name.toStdWString());
		for (auto& v : scenes) {
			pt.add(key_save_scenes, v.toStdWString());
		}

		write_xml(p->GetProjectFilePath().toStdString(), pt);

		p->GetDir().mkpath(SCENES_FOLDER);
	} catch (exception&) {
		return false;
	}
	return true;
}

shared_ptr<Project> Project::Load(const QString& filePath)
{
	shared_ptr<Project> p(new Project);
	if (!p->d->Load(filePath))
		return nullptr;
	return p;
}

shared_ptr<Project> Project::New(const QString& name, const QString& dirPath)
{
	shared_ptr<Project> p(new Project);
	p->d->New(name, dirPath);
	return p;
}

bool Project::Save()
{
	return d->Save();
}

Project::Project() :d(new ProjectPrivate(this))
{
}

Project::~Project()
{
	delete d;
}

QString Project::GetName()
{
	return d->name;
}

void Project::SetName(const QString& name)
{
	d->name = name;
}

QDir Project::GetDir()
{
	return QDir(d->dirPath);
}

QString Project::GetProjectFilePath()
{
	return GetDir().filePath(d->fileName);
}

QString Project::GetProjectFileName()
{
	return d->fileName;
}

EResult Project::AddScene(const QString& name)
{
	if (find(d->scenes.begin(), d->scenes.end(), name) != d->scenes.end())
		return EResult::EXIST;

	auto p = EScene::New(name, this);
	p->Save();
	d->scenes.push_back(name);
	return EResult::OK;
}

EResult Project::OpenScene(const QString& name)
{
	if (find(d->scenes.begin(), d->scenes.end(), name) == d->scenes.end())
		return EResult::NOT_FOUND;

	d->currentScene = EScene::Load(name, this);

	return EResult::OK;
}

vector<QString> Project::GetSceneNames()
{
	return d->scenes;
}

QDir Project::GetScenesDir()
{
	return QDir(GetDir().filePath(SCENES_FOLDER));
}

EScene* Project::GetCurrentScene()
{
	return d->currentScene.get();
}
