/********************************************************************************
** Form generated from reading UI file 'modelviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODELVIEWER_H
#define UI_MODELVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <renderwidget.h>

QT_BEGIN_NAMESPACE

class Ui_ModelViewerClass
{
public:
    QAction *action_Open;
    QAction *action_Exit;
    QAction *actionOpen_animation;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    RenderWidget *widget;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ModelViewerClass)
    {
        if (ModelViewerClass->objectName().isEmpty())
            ModelViewerClass->setObjectName(QStringLiteral("ModelViewerClass"));
        ModelViewerClass->resize(625, 418);
        action_Open = new QAction(ModelViewerClass);
        action_Open->setObjectName(QStringLiteral("action_Open"));
        action_Exit = new QAction(ModelViewerClass);
        action_Exit->setObjectName(QStringLiteral("action_Exit"));
        actionOpen_animation = new QAction(ModelViewerClass);
        actionOpen_animation->setObjectName(QStringLiteral("actionOpen_animation"));
        centralWidget = new QWidget(ModelViewerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(2);
        sizePolicy.setVerticalStretch(2);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        widget = new RenderWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(2);
        sizePolicy1.setVerticalStretch(2);
        sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy1);
        widget->setMouseTracking(false);
        widget->setFocusPolicy(Qt::StrongFocus);
        widget->setContextMenuPolicy(Qt::DefaultContextMenu);
        widget->setAutoFillBackground(false);

        horizontalLayout->addWidget(widget);

        ModelViewerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ModelViewerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 625, 23));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QStringLiteral("menu_File"));
        ModelViewerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ModelViewerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ModelViewerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ModelViewerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ModelViewerClass->setStatusBar(statusBar);

        menuBar->addAction(menu_File->menuAction());
        menu_File->addAction(action_Open);
        menu_File->addAction(actionOpen_animation);
        menu_File->addAction(action_Exit);

        retranslateUi(ModelViewerClass);
        QObject::connect(action_Exit, SIGNAL(triggered()), ModelViewerClass, SLOT(close()));

        QMetaObject::connectSlotsByName(ModelViewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *ModelViewerClass)
    {
        ModelViewerClass->setWindowTitle(QApplication::translate("ModelViewerClass", "ModelViewer", 0));
        action_Open->setText(QApplication::translate("ModelViewerClass", "&Open mesh...", 0));
        action_Exit->setText(QApplication::translate("ModelViewerClass", "&Exit", 0));
        actionOpen_animation->setText(QApplication::translate("ModelViewerClass", "&Open animation..", 0));
        menu_File->setTitle(QApplication::translate("ModelViewerClass", "&File", 0));
    } // retranslateUi

};

namespace Ui {
    class ModelViewerClass: public Ui_ModelViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELVIEWER_H
