#include <qmessagebox.h>
#include <qdir.h>
#include <qfiledialog.h>
#include "editor.h"
#include "SimpleLevel.h"
#include "mdlimporterdlg.h"
#include "newprojectdialog.h"

using namespace AGE;

Editor::Editor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	UIInitHelper();

	mEngine = AGE::Engine::GetInstance();
	mEngine->StartUp((HWND)ui.widget->winId());

	connect(ui.widget, SIGNAL(onRefresh()), this, SLOT(UpdateEngine()));

	connect(ui.widget, &RenderWidget::onMousePress, [this](QPoint point){
		if (GetScene()) {
			SceneNode* node = GetScene()->GetCurrentCamera()->PickAt(point.x(), point.y());
			printf("pick %p\n", node);
		}
	});

	connect(ui.widget, &RenderWidget::onResize, [this](QSize newSize, QSize oldSize){
		RenderEngine::GetInstance()->ResizeToFit();
		ui.widget->update();
	});

	connect(ui.widget, &RenderWidget::onDrag, [this](QPoint delta){
		if (GetLevel<SimpleLevel>()) {
			GetLevel<SimpleLevel>()->RotateCamera(delta.x(), delta.y());
			ui.widget->update();
		}
	});

	connect(ui.widget, &RenderWidget::onWheel, [this](int delta){
		if (GetLevel<SimpleLevel>()) {
			GetLevel<SimpleLevel>()->AdjustDistance(delta);
			ui.widget->update();
		}
	});

	connect(ui.actionCS, &QAction::triggered, [](){
		MDLImporterDlg w;
		w.exec();
	});
	connect(ui.action_N, &QAction::triggered, this, &Editor::OnActionNewProject);
	connect(ui.action_O, &QAction::triggered, [this](){
		QString path = QFileDialog::getOpenFileName(this, tr("打开工程文件"), "", tr("AGE Project (*.aproj)"));
		if (path.isEmpty())
			return;

		mProject = Project::Load(path);
		if (!mProject) {
			QMessageBox::information(this, tr("提示"), tr("读取项目文件失败"));
			return;
		}
		ProjectContentChanged();
	});

	connect(ui.actionTest, &QAction::triggered, [this](){
		mProject = Project::Load("D:\\Projects\\AGEProjects\\test\\123.aproj");
		OpenScene("test");
		UpdateProjectView();
	});

	connect(ui.action_S, &QAction::triggered, [this](){
		mProject->AddScene("good");
		UpdateProjectView();
	});

	connect(ui.planeButton, &QPushButton::clicked, [this](){
		GetLevel<SimpleLevel>()->AddPrimitive(PrimitiveType::Plane);
	});

	connect(ui.icosphereButton, &QPushButton::clicked, [this](){
		GetLevel<SimpleLevel>()->AddPrimitive(PrimitiveType::IcoSphere);
	});

	showMaximized();
}

void Editor::UIInitHelper()
{
	tabifyDockWidget(ui.projectDock, ui.primitiveDock);
	ui.projectDock->raise();
}

void Editor::UpdateEngine()
{
	mEngine->Update();
}

void Editor::OnActionNewProject()
{
	NewProjectDialog npd;
	npd.exec();
	if (npd.IsAccepted()) {
		QString path, name;
		npd.GetParameters(path, name);

		mProject = Project::New(name, path);
		ProjectContentChanged();
	}
}

Editor::~Editor()
{
	mEngine->ShutDown();
}

void Editor::UpdateTitle()
{
	if (mProject) {
		this->setWindowTitle(mProject->GetName() + " - AGE Editor");
	} else {
		this->setWindowTitle("AGE Editor");
	}
}

void Editor::ProjectContentChanged()
{
	UpdateTitle();
	UpdateProjectView();
}

void Editor::UpdateProjectView()
{
	ui.projectTree->clear();
	auto topLevelNode = new QTreeWidgetItem(QStringList("Project - " + mProject->GetName()));
	auto scenesNode = new QTreeWidgetItem(QStringList("Scenes"));
	topLevelNode->addChild(scenesNode);
	for (auto& name : mProject->GetSceneNames()) {
		scenesNode->addChild(new QTreeWidgetItem(QStringList(name)));
	}

	ui.projectTree->addTopLevelItem(topLevelNode);
	ui.projectTree->expandAll();
}

void Editor::OpenScene(const QString& name)
{
	GetLevelManager()->UnloadLevel();
	GetLevelManager()->LoadLevel<SimpleLevel>(ui.widget);
	ui.widget->update();
}
