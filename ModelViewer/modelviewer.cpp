#include <qmessagebox.h>
#include <qfiledialog.h>
#include "modelviewer.h"
#include "ModelViewLevel.h"


ModelViewer::ModelViewer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	mEngine = AGE::Engine::GetInstance();
	mEngine->StartUp((HWND)ui.widget->winId());
	GetLevelManager()->LoadLevel<ModelViewLevel>();

	GetLevel<ModelViewLevel>()->Load("../Resources/Models/leet.AMESH");

	connect(ui.widget, SIGNAL(onRefresh()), this, SLOT(UpdateRenderView()));

	connect(ui.action_Open, &QAction::triggered, [this](){
		QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open Mesh"), "", tr("Mesh Files (*.AMESH)"));
		if (!fileName.isEmpty()) {
			GetLevel<ModelViewLevel>()->Load(fileName.toLocal8Bit());
			ui.widget->update();
		}
	});

	connect(ui.widget, &RenderWidget::onResize, [this](QSize newSize, QSize oldSize){
		RenderEngine::GetInstance()->ResizeToFit();
		ui.widget->update();
	});

	connect(ui.widget, &RenderWidget::onDrag, [this](QPoint delta){
		GetLevel<ModelViewLevel>()->RotateCamera(delta.x(), delta.y());
		ui.widget->update();
	});

	connect(ui.widget, &RenderWidget::onWheel, [this](int delta){
		GetLevel<ModelViewLevel>()->AdjustDistance(delta);
		ui.widget->update();
	});
}

void ModelViewer::UpdateRenderView()
{
	mEngine->Update();
}

ModelViewer::~ModelViewer()
{
	mEngine->ShutDown();
}
