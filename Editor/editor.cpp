#include <qmessagebox.h>
#include <qpainter.h>
#include <qdir.h>
#include <qfiledialog.h>
#include <qdatetime.h>
#include <qtpropertybrowser\qtvariantproperty.h>

#include "editor.h"
#include "EScene.h"
#include "EditorLevel.h"
#include "mdlimporterdlg.h"
#include "newprojectdialog.h"
#include "PropertySceneNode.h"
#include <Renderable.h>
#include <AGEMeshImporter.h>

//using namespace AGE;

Editor::Editor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	UIInitHelper();

	mEngine = AGE::Engine::GetInstance();
	mEngine->StartUp((HWND)ui.widget->winId());
	AGE::GetLevelManager()->LoadLevel<EditorLevel>(ui.widget);

	connect(ui.widget, &RenderWidget::onRefresh, this, &Editor::RenderWidgetUpdate);

	connect(ui.widget, &RenderWidget::onMousePress, [this](QPoint point){
		if (AGE::GetScene()) {
			AGE::SceneNode* node = AGE::GetScene()->GetCurrentCamera()->PickAt(point.x(), point.y());
			printf("pick %p\n", node);
			if (!node)
				return;

			if (auto p = node->GetUserData<PropertySceneNode>(0)) {
				printf("good\n");
				ui.propertyWidget->clear();
				QtVariantPropertyManager *variantManager = new QtVariantPropertyManager();
				QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory();
				ui.propertyWidget->setFactoryForManager(variantManager, variantFactory);
				p->AddProperties(ui.propertyWidget, variantManager);
			}
		}
	});

	connect(ui.widget, &RenderWidget::onResize, [this](QSize newSize, QSize oldSize){
		AGE::RenderEngine::GetInstance()->ResizeToFit();
		ui.widget->update();
	});

	connect(ui.widget, &RenderWidget::onDrag, [this](QPoint delta){
		if (GetEditorLevel()) {
			GetEditorLevel()->RotateCamera(delta.x(), delta.y());
		}
	});

	connect(ui.widget, &RenderWidget::onWheel, [this](int delta){
		if (GetEditorLevel()) {
			GetEditorLevel()->AdjustDistance(delta);
		}
	});

	connect(ui.actionSave, &QAction::triggered, [this](){
		if (mProject) {
			mProject->Save();
			if (mProject->GetCurrentScene())
				mProject->GetCurrentScene()->Save();
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
		mProject->OpenScene("good");
		UpdateProjectView();
	});

	connect(ui.action_S, &QAction::triggered, [this](){
		mProject->AddScene("good");
		UpdateProjectView();
	});

	connect(ui.planeButton, &QPushButton::clicked, [this](){
		GetEditorLevel()->AddPrimitive(PrimitiveType::Plane);
	});

	connect(ui.icosphereButton, &QPushButton::clicked, [this](){
		GetEditorLevel()->AddPrimitive(PrimitiveType::IcoSphere);
	});

	connect(ui.projectTree, &QTreeWidget::itemDoubleClicked, [this](QTreeWidgetItem *item, int column){
		if (item->parent() != mScenesTreeNode)
			return;

		OpenScene(item->text(0));
	});

	connect(ui.actionNewSceneNode, &QAction::triggered, [this](){
		QString path = QFileDialog::getOpenFileName(this, tr("打开"), "", tr("AGE Mesh (*.AMESH)"));
		if (path.isEmpty())
			return;
		AGE::AGEMeshImporter importer;
		GetEditorLevel()->AddSceneNode(importer.LoadFromFile(path.toStdString().c_str(), true));
	});

	connect(ui.actionNewPhysicsNode, &QAction::triggered, [this](){
		QString path = QFileDialog::getOpenFileName(this, tr("打开"), "", tr("AGE Mesh (*.AMESH)"));
		if (path.isEmpty())
			return;
		AGE::AGEMeshImporter importer;
		GetEditorLevel()->AddPhysicsNode(importer.LoadFromFile(path.toStdString().c_str(), true));
	});

	showMaximized();
}

void Editor::UIInitHelper()
{
	tabifyDockWidget(ui.projectDock, ui.primitiveDock);
	tabifyDockWidget(ui.primitiveDock, ui.propertyDock);
	ui.projectDock->raise();

	ui.propertyWidget->setPropertiesWithoutValueMarked(true);
	ui.propertyWidget->setHeaderVisible(false);
	ui.propertyWidget->setRootIsDecorated(false);
}

void Editor::RenderWidgetUpdate()
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
	mProjectTreeNode = new QTreeWidgetItem(QStringList("Project - " + mProject->GetName()));
	mScenesTreeNode = new QTreeWidgetItem(QStringList("Scenes"));
	mProjectTreeNode->addChild(mScenesTreeNode);
	for (auto& name : mProject->GetSceneNames()) {
		mScenesTreeNode->addChild(new QTreeWidgetItem(QStringList(name)));
	}

	ui.projectTree->addTopLevelItem(mProjectTreeNode);
	ui.projectTree->expandAll();
}

void Editor::OpenScene(const QString& name)
{
	GetEditorLevel()->SetScene(nullptr);
	mProject->OpenScene(name);
	GetEditorLevel()->SetScene(mProject->GetCurrentScene());
}
