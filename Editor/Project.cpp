#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <qfileinfo.h>
#include <qdir.h>
#include <map>
#include "Project.h"
#include "EScene.h"

using namespace boost::property_tree;
using namespace std;

#define PROP_DECL(type, name) \
type name;\
const wchar_t* key_##name = L"project."L#name

const char* SCENES_FOLDER = "Scenes";

struct ProjectPrivate
{
	typedef map<QString, shared_ptr<EScene>> ScenesType;

	Project* p;

	PROP_DECL(QString, name);
	PROP_DECL(ScenesType, scenes);

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

		write_xml(p->GetProjectFilePath().toStdString(), pt);
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

bool Project::AddScene(const QString& name)
{
	if (d->scenes.find(name) != d->scenes.end())
		return false;

	d->scenes.insert(make_pair(name, EScene::New(name, this)));
}

shared_ptr<EScene> Project::GetScene(const QString& name)
{
	return d->scenes.find(name)->second;
}

vector<QString> Project::GetSceneNames()
{
	vector<QString> v;
	for (auto& k : d->scenes) {
		v.push_back(k.first);
	}
	return v;
}

QDir Project::GetScenesDir()
{
	return QDir(GetDir().filePath(SCENES_FOLDER));
}
