#ifndef EDITOR_H
#define EDITOR_H

#include <QtWidgets/QMainWindow>
#include "ui_editor.h"
#include <AGE.h>

class Editor : public QMainWindow
{
	Q_OBJECT

public:
	Editor(QWidget *parent = 0);
	~Editor();

public slots:
	
	void OnButton1Clicked();

	void OnButton2Clicked();

private:
	Ui::EditorClass ui;

	AGE::Engine* mEngine;
};

#endif // EDITOR_H
