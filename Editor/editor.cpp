#include "editor.h"
#include "SimpleLevel.h"
#include "mdlimporterdlg.h"
#include <qmessagebox.h>


Editor::Editor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	mEngine = AGE::Engine::GetInstance();

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(OnButton1Clicked()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(OnButton2Clicked()));
	connect(ui.widget, SIGNAL(onEvent()), this, SLOT(OnButton2Clicked()));
	connect(ui.actionCS, &QAction::triggered, [](){
		MDLImporterDlg w;
		w.exec(); 
	});

}

void Editor::OnButton1Clicked()
{
	mEngine->StartUp((HWND)ui.widget->winId());
	GetLevelManager()->LoadLevel<SimpleLevel>();
}

void Editor::OnButton2Clicked()
{
	mEngine->Update();
}

Editor::~Editor()
{
	mEngine->ShutDown();
}
