#ifndef EDITOR_H
#define EDITOR_H

#include <QtWidgets/QMainWindow>
#include <AGE.h>
#include "ui_editor.h"
#include "Project.h"

#pragma execution_character_set("utf-8")

class Editor : public QMainWindow
{
	Q_OBJECT

public:
	Editor(QWidget *parent = 0);
	~Editor();

	void OpenScene(const QString& name);

public slots:
	void OnActionNewProject();
	void UpdateEngine();

private:
	void ProjectContentChanged();
	void UpdateTitle();
	void UpdateProjectView();

	void UIInitHelper();

	Ui::EditorClass ui;
	AGE::Engine* mEngine;
	shared_ptr<Project> mProject;

	QTreeWidgetItem* mProjectTreeNode;
	QTreeWidgetItem* mScenesTreeNode;
};

#endif // EDITOR_H
