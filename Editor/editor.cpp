#include <qmessagebox.h>
#include <qdir.h>
#include <qfiledialog.h>
#include <qdatetime.h>
#include <qtpropertybrowser\qtvariantproperty.h>

#include "editor.h"
#include "EScene.h"
#include "MetaLevel.h"
#include "mdlimporterdlg.h"
#include "newprojectdialog.h"

//using namespace AGE;

Editor::Editor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	UIInitHelper();

	mEngine = AGE::Engine::GetInstance();
	mEngine->StartUp((HWND)ui.widget->winId());
	AGE::GetLevelManager()->LoadLevel<MetaLevel>(ui.widget);

	connect(ui.widget, SIGNAL(onRefresh()), this, SLOT(UpdateEngine()));

	connect(ui.widget, &RenderWidget::onMousePress, [this](QPoint point){
		if (AGE::GetScene()) {
			AGE::SceneNode* node = AGE::GetScene()->GetCurrentCamera()->PickAt(point.x(), point.y());
			printf("pick %p\n", node);
		}
	});

	connect(ui.widget, &RenderWidget::onResize, [this](QSize newSize, QSize oldSize){
		AGE::RenderEngine::GetInstance()->ResizeToFit();
		ui.widget->update();
	});

	connect(ui.widget, &RenderWidget::onDrag, [this](QPoint delta){
		if (GetMetaLevel()) {
			GetMetaLevel()->RotateCamera(delta.x(), delta.y());
			ui.widget->update();
		}
	});

	connect(ui.widget, &RenderWidget::onWheel, [this](int delta){
		if (GetMetaLevel()) {
			GetMetaLevel()->AdjustDistance(delta);
			ui.widget->update();
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
		GetMetaLevel()->AddPrimitive(PrimitiveType::Plane);
	});

	connect(ui.icosphereButton, &QPushButton::clicked, [this](){
		GetMetaLevel()->AddPrimitive(PrimitiveType::IcoSphere);
	});

	connect(ui.projectTree, &QTreeWidget::itemDoubleClicked, [this](QTreeWidgetItem *item, int column){
		if (item->parent() != mScenesTreeNode)
			return;

		OpenScene(item->text(0));
	});

	showMaximized();
}

void Editor::UIInitHelper()
{
	tabifyDockWidget(ui.projectDock, ui.primitiveDock);
	tabifyDockWidget(ui.primitiveDock, ui.propertyDock);
	ui.projectDock->raise();


	QtVariantPropertyManager *variantManager = new QtVariantPropertyManager();

	int i = 0;
	QtProperty *topItem = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
		QString::number(i++) + QLatin1String(" Group Property"));

	QtVariantProperty *item = variantManager->addProperty(QVariant::Bool, QString::number(i++) + QLatin1String(" Bool Property"));
	item->setValue(true);
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::Int, QString::number(i++) + QLatin1String(" Int Property"));
	item->setValue(20);
	item->setAttribute(QLatin1String("minimum"), 0);
	item->setAttribute(QLatin1String("maximum"), 100);
	item->setAttribute(QLatin1String("singleStep"), 10);
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::Int, QString::number(i++) + QLatin1String(" Int Property (ReadOnly)"));
	item->setValue(20);
	item->setAttribute(QLatin1String("minimum"), 0);
	item->setAttribute(QLatin1String("maximum"), 100);
	item->setAttribute(QLatin1String("singleStep"), 10);
	item->setAttribute(QLatin1String("readOnly"), true);
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::Double, QString::number(i++) + QLatin1String(" Double Property"));
	item->setValue(1.2345);
	item->setAttribute(QLatin1String("singleStep"), 0.1);
	item->setAttribute(QLatin1String("decimals"), 3);
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::Double, QString::number(i++) + QLatin1String(" Double Property (ReadOnly)"));
	item->setValue(1.23456);
	item->setAttribute(QLatin1String("singleStep"), 0.1);
	item->setAttribute(QLatin1String("decimals"), 5);
	item->setAttribute(QLatin1String("readOnly"), true);
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::String, QString::number(i++) + QLatin1String(" String Property"));
	item->setValue("Value");
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::String, QString::number(i++) + QLatin1String(" String Property (Password)"));
	item->setAttribute(QLatin1String("echoMode"), QLineEdit::Password);
	item->setValue("Password");
	topItem->addSubProperty(item);

	// Readonly String Property
	item = variantManager->addProperty(QVariant::String, QString::number(i++) + QLatin1String(" String Property (ReadOnly)"));
	item->setAttribute(QLatin1String("readOnly"), true);
	item->setValue("readonly text");
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::Date, QString::number(i++) + QLatin1String(" Date Property"));
	item->setValue(QDate::currentDate().addDays(2));
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::Time, QString::number(i++) + QLatin1String(" Time Property"));
	item->setValue(QTime::currentTime());
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::DateTime, QString::number(i++) + QLatin1String(" DateTime Property"));
	item->setValue(QDateTime::currentDateTime());
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::KeySequence, QString::number(i++) + QLatin1String(" KeySequence Property"));
	item->setValue(QKeySequence(Qt::ControlModifier | Qt::Key_Q));
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::Char, QString::number(i++) + QLatin1String(" Char Property"));
	item->setValue(QChar(386));
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::Locale, QString::number(i++) + QLatin1String(" Locale Property"));
	item->setValue(QLocale(QLocale::Polish, QLocale::Poland));
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::Point, QString::number(i++) + QLatin1String(" Point Property"));
	item->setValue(QPoint(10, 10));
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::PointF, QString::number(i++) + QLatin1String(" PointF Property"));
	item->setValue(QPointF(1.2345, -1.23451));
	item->setAttribute(QLatin1String("decimals"), 3);
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::Size, QString::number(i++) + QLatin1String(" Size Property"));
	item->setValue(QSize(20, 20));
	item->setAttribute(QLatin1String("minimum"), QSize(10, 10));
	item->setAttribute(QLatin1String("maximum"), QSize(30, 30));
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::SizeF, QString::number(i++) + QLatin1String(" SizeF Property"));
	item->setValue(QSizeF(1.2345, 1.2345));
	item->setAttribute(QLatin1String("decimals"), 3);
	item->setAttribute(QLatin1String("minimum"), QSizeF(0.12, 0.34));
	item->setAttribute(QLatin1String("maximum"), QSizeF(20.56, 20.78));
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::Rect, QString::number(i++) + QLatin1String(" Rect Property"));
	item->setValue(QRect(10, 10, 20, 20));
	topItem->addSubProperty(item);
	item->setAttribute(QLatin1String("constraint"), QRect(0, 0, 50, 50));

	item = variantManager->addProperty(QVariant::RectF, QString::number(i++) + QLatin1String(" RectF Property"));
	item->setValue(QRectF(1.2345, 1.2345, 1.2345, 1.2345));
	topItem->addSubProperty(item);
	item->setAttribute(QLatin1String("constraint"), QRectF(0, 0, 50, 50));
	item->setAttribute(QLatin1String("decimals"), 3);

	item = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(),
		QString::number(i++) + QLatin1String(" Enum Property"));
	QStringList enumNames;
	enumNames << "Enum0" << "Enum1" << "Enum2";
	item->setAttribute(QLatin1String("enumNames"), enumNames);
	item->setValue(1);
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QtVariantPropertyManager::flagTypeId(),
		QString::number(i++) + QLatin1String(" Flag Property"));
	QStringList flagNames;
	flagNames << "Flag0" << "Flag1" << "Flag2";
	item->setAttribute(QLatin1String("flagNames"), flagNames);
	item->setValue(5);
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::SizePolicy, QString::number(i++) + QLatin1String(" SizePolicy Property"));
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::Font, QString::number(i++) + QLatin1String(" Font Property"));
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::Cursor, QString::number(i++) + QLatin1String(" Cursor Property"));
	topItem->addSubProperty(item);

	item = variantManager->addProperty(QVariant::Color, QString::number(i++) + QLatin1String(" Color Property"));
	topItem->addSubProperty(item);

	QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory();

	ui.propertyWidget->setFactoryForManager(variantManager, variantFactory);
	ui.propertyWidget->addProperty(topItem);
	ui.propertyWidget->setPropertiesWithoutValueMarked(true);
	ui.propertyWidget->setHeaderVisible(false);
	ui.propertyWidget->setRootIsDecorated(false);
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
	GetMetaLevel()->UnloadScene();
	mProject->OpenScene(name);
	GetMetaLevel()->LoadScene(mProject->GetCurrentScene());
	ui.widget->update();
}
