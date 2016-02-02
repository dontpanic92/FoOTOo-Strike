#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include <QtWidgets/QMainWindow>

#include "ui_modelviewer.h"
#include <AGE.h>

class ModelViewer : public QMainWindow
{
	Q_OBJECT

public:
	ModelViewer(QWidget *parent = 0);
	~ModelViewer();

public slots:

	void OnButton1Clicked();

	void OnButton2Clicked();

private:
	Ui::ModelViewerClass ui;

	AGE::Engine* mEngine;
};

#endif // MODELVIEWER_H
