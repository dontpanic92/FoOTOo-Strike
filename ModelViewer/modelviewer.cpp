#include <qmessagebox.h>
#include <qfiledialog.h>
#include "modelviewer.h"
#include "ModelViewLevel.h"


ModelViewer::ModelViewer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	mEngine = AGE::Engine::GetInstance();

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(OnButton1Clicked()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(OnButton2Clicked()));
	connect(ui.widget, SIGNAL(onRefresh()), this, SLOT(OnButton2Clicked()));

	connect(ui.action_Open, &QAction::triggered, [this](){
		QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open Mesh"), "", tr("Mesh Files (*.AMESH)"));
	});
}

void ModelViewer::OnButton1Clicked()
{
	mEngine->StartUp((HWND)ui.widget->winId());
	GetLevelManager()->LoadLevel<ModelViewLevel>();
}

void ModelViewer::OnButton2Clicked()
{
	mEngine->Update();
}

ModelViewer::~ModelViewer()
{
	mEngine->ShutDown();
}
