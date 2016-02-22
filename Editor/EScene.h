#ifndef __EDITOR_SCENE_HEADER__
#define __EDITOR_SCENE_HEADER__

#include <qstring.h>
#include <memory>
using namespace std;

class Project;
class EScenePrivate;
class EScene
{
public:
	~EScene();

	static shared_ptr<EScene> Load(const QString& name, Project* project);
	static shared_ptr<EScene> New(const QString& name, Project* project);
	bool Save();

private:
	EScene(Project* project);
	EScenePrivate* d;
};

#endif
