/********************************************************************************
** Form generated from reading UI file 'editor.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITOR_H
#define UI_EDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <qtpropertybrowser/qttreepropertybrowser.h>
#include "renderwidget.h"

QT_BEGIN_NAMESPACE

class Ui_EditorClass
{
public:
    QAction *actionExit;
    QAction *actionCS;
    QAction *action_N;
    QAction *action_O;
    QAction *actionTest;
    QAction *action_S;
    QAction *actionSave;
    QAction *actionNewSceneNode;
    QAction *actionNewPhysicsNode;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    RenderWidget *widget;
    QMenuBar *menuBar;
    QMenu *menuAaa;
    QMenu *menu_I;
    QMenu *menu;
    QMenu *menu_P;
    QMenu *menu_S;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *projectDock;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout;
    QTreeWidget *projectTree;
    QDockWidget *primitiveDock;
    QWidget *dockWidgetContents;
    QPushButton *planeButton;
    QPushButton *icosphereButton;
    QDockWidget *propertyDock;
    QWidget *dockWidgetContents_3;
    QVBoxLayout *verticalLayout_2;
    QtTreePropertyBrowser *propertyWidget;

    void setupUi(QMainWindow *EditorClass)
    {
        if (EditorClass->objectName().isEmpty())
            EditorClass->setObjectName(QStringLiteral("EditorClass"));
        EditorClass->resize(645, 502);
        actionExit = new QAction(EditorClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionCS = new QAction(EditorClass);
        actionCS->setObjectName(QStringLiteral("actionCS"));
        action_N = new QAction(EditorClass);
        action_N->setObjectName(QStringLiteral("action_N"));
        action_O = new QAction(EditorClass);
        action_O->setObjectName(QStringLiteral("action_O"));
        actionTest = new QAction(EditorClass);
        actionTest->setObjectName(QStringLiteral("actionTest"));
        action_S = new QAction(EditorClass);
        action_S->setObjectName(QStringLiteral("action_S"));
        actionSave = new QAction(EditorClass);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionNewSceneNode = new QAction(EditorClass);
        actionNewSceneNode->setObjectName(QStringLiteral("actionNewSceneNode"));
        actionNewPhysicsNode = new QAction(EditorClass);
        actionNewPhysicsNode->setObjectName(QStringLiteral("actionNewPhysicsNode"));
        centralWidget = new QWidget(EditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        widget = new RenderWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));

        gridLayout->addWidget(widget, 0, 0, 1, 1);

        EditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(EditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 645, 23));
        menuAaa = new QMenu(menuBar);
        menuAaa->setObjectName(QStringLiteral("menuAaa"));
        menu_I = new QMenu(menuBar);
        menu_I->setObjectName(QStringLiteral("menu_I"));
        menu = new QMenu(menu_I);
        menu->setObjectName(QStringLiteral("menu"));
        menu_P = new QMenu(menuBar);
        menu_P->setObjectName(QStringLiteral("menu_P"));
        menu_S = new QMenu(menuBar);
        menu_S->setObjectName(QStringLiteral("menu_S"));
        EditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(EditorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        EditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(EditorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        EditorClass->setStatusBar(statusBar);
        projectDock = new QDockWidget(EditorClass);
        projectDock->setObjectName(QStringLiteral("projectDock"));
        projectDock->setEnabled(true);
        projectDock->setStyleSheet(QLatin1String("QDockWidget::title\n"
"{\n"
"display:none;\n"
"}"));
        projectDock->setFloating(false);
        projectDock->setFeatures(QDockWidget::DockWidgetFloatable);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QStringLiteral("dockWidgetContents_2"));
        verticalLayout = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        projectTree = new QTreeWidget(dockWidgetContents_2);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        projectTree->setHeaderItem(__qtreewidgetitem);
        projectTree->setObjectName(QStringLiteral("projectTree"));
        projectTree->setStyleSheet(QStringLiteral(""));
        projectTree->setFrameShape(QFrame::StyledPanel);
        projectTree->setColumnCount(1);
        projectTree->header()->setVisible(false);
        projectTree->header()->setDefaultSectionSize(100);

        verticalLayout->addWidget(projectTree);

        projectDock->setWidget(dockWidgetContents_2);
        EditorClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), projectDock);
        primitiveDock = new QDockWidget(EditorClass);
        primitiveDock->setObjectName(QStringLiteral("primitiveDock"));
        primitiveDock->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        planeButton = new QPushButton(dockWidgetContents);
        planeButton->setObjectName(QStringLiteral("planeButton"));
        planeButton->setGeometry(QRect(20, 20, 75, 23));
        icosphereButton = new QPushButton(dockWidgetContents);
        icosphereButton->setObjectName(QStringLiteral("icosphereButton"));
        icosphereButton->setGeometry(QRect(120, 20, 75, 23));
        primitiveDock->setWidget(dockWidgetContents);
        EditorClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), primitiveDock);
        propertyDock = new QDockWidget(EditorClass);
        propertyDock->setObjectName(QStringLiteral("propertyDock"));
        propertyDock->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QStringLiteral("dockWidgetContents_3"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents_3);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        propertyWidget = new QtTreePropertyBrowser(dockWidgetContents_3);
        propertyWidget->setObjectName(QStringLiteral("propertyWidget"));

        verticalLayout_2->addWidget(propertyWidget);

        propertyDock->setWidget(dockWidgetContents_3);
        EditorClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), propertyDock);

        menuBar->addAction(menuAaa->menuAction());
        menuBar->addAction(menu_P->menuAction());
        menuBar->addAction(menu_S->menuAction());
        menuBar->addAction(menu_I->menuAction());
        menuAaa->addAction(action_N);
        menuAaa->addAction(action_O);
        menuAaa->addAction(actionSave);
        menuAaa->addAction(actionTest);
        menuAaa->addSeparator();
        menuAaa->addAction(actionExit);
        menu_I->addAction(menu->menuAction());
        menu->addAction(actionCS);
        menu_P->addAction(action_S);
        menu_S->addAction(actionNewSceneNode);
        menu_S->addAction(actionNewPhysicsNode);

        retranslateUi(EditorClass);
        QObject::connect(actionExit, SIGNAL(triggered()), EditorClass, SLOT(close()));

        QMetaObject::connectSlotsByName(EditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *EditorClass)
    {
        EditorClass->setWindowTitle(QApplication::translate("EditorClass", "AGE Editor", 0));
        actionExit->setText(QApplication::translate("EditorClass", "\351\200\200\345\207\272(&E)", 0));
        actionCS->setText(QApplication::translate("EditorClass", "CS 1.6 MDL\346\226\207\344\273\266 ...", 0));
        action_N->setText(QApplication::translate("EditorClass", "\346\226\260\345\273\272\345\267\245\347\250\213(&N)...", 0));
        action_O->setText(QApplication::translate("EditorClass", "\346\211\223\345\274\200\345\267\245\347\250\213(&O)...", 0));
        actionTest->setText(QApplication::translate("EditorClass", "test", 0));
        action_S->setText(QApplication::translate("EditorClass", "\346\267\273\345\212\240\345\234\272\346\231\257(&S)...", 0));
        actionSave->setText(QApplication::translate("EditorClass", "\344\277\235\345\255\230(&S)", 0));
        actionNewSceneNode->setText(QApplication::translate("EditorClass", "\346\267\273\345\212\240\345\234\272\346\231\257\350\212\202\347\202\271(&N)...", 0));
        actionNewPhysicsNode->setText(QApplication::translate("EditorClass", "\346\267\273\345\212\240\347\211\251\347\220\206\350\212\202\347\202\271(&P)...", 0));
        menuAaa->setTitle(QApplication::translate("EditorClass", "\346\226\207\344\273\266(&F)", 0));
        menu_I->setTitle(QApplication::translate("EditorClass", "\350\265\204\346\272\220(&R)", 0));
        menu->setTitle(QApplication::translate("EditorClass", "\345\257\274\345\205\245", 0));
        menu_P->setTitle(QApplication::translate("EditorClass", "\345\267\245\347\250\213(&P)", 0));
        menu_S->setTitle(QApplication::translate("EditorClass", "\345\234\272\346\231\257(&S)", 0));
        projectDock->setWindowTitle(QApplication::translate("EditorClass", "Project", 0));
        primitiveDock->setWindowTitle(QApplication::translate("EditorClass", "Primitive", 0));
        planeButton->setText(QApplication::translate("EditorClass", "Plane", 0));
        icosphereButton->setText(QApplication::translate("EditorClass", "IcoSphere", 0));
        propertyDock->setWindowTitle(QApplication::translate("EditorClass", "Property", 0));
    } // retranslateUi

};

namespace Ui {
    class EditorClass: public Ui_EditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITOR_H
