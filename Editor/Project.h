#ifndef __EDITOR_PROJECT_HEADER__
#define __EDITOR_PROJECT_HEADER__

#include <qstring.h>
#include <qdir.h>
#include <memory>
#include <AGE.h>
using namespace std;

enum class EResult
{
	OK,
	EXIST,
	NOT_FOUND,
	FAILED
};

class EScene;
class ProjectPrivate;
class Project
{
public:
	~Project();

	static shared_ptr<Project> Load(const QString& filePath);
	static shared_ptr<Project> New(const QString& name, const QString& dirPath);
	bool Save();

	QString GetName();
	void SetName(const QString& name);

	QDir GetDir();
	QString GetProjectFilePath();
	QString GetProjectFileName();

	QDir GetScenesDir();

	EResult AddScene(const QString& name);
	EResult OpenScene(const QString& name);
	EScene* GetCurrentScene();
	vector<QString> GetSceneNames();

private:
	Project();
	ProjectPrivate* d;
};

#endif