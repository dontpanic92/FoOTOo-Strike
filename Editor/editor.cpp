#include "editor.h"
#include "SimpleLevel.h"

#include <qmessagebox.h>


Editor::Editor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(OnButton1Clicked()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(OnButton2Clicked()));

	mEngine = AGE::Engine::GetInstance();
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
