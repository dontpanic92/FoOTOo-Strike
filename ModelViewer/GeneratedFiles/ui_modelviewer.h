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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
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
    RenderWidget *widget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ModelViewerClass)
    {
        if (ModelViewerClass->objectName().isEmpty())
            ModelViewerClass->setObjectName(QStringLiteral("ModelViewerClass"));
        ModelViewerClass->resize(600, 400);
        action_Open = new QAction(ModelViewerClass);
        action_Open->setObjectName(QStringLiteral("action_Open"));
        action_Exit = new QAction(ModelViewerClass);
        action_Exit->setObjectName(QStringLiteral("action_Exit"));
        actionOpen_animation = new QAction(ModelViewerClass);
        actionOpen_animation->setObjectName(QStringLiteral("actionOpen_animation"));
        centralWidget = new QWidget(ModelViewerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        widget = new RenderWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 20, 461, 311));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(490, 90, 75, 23));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(490, 130, 75, 23));
        ModelViewerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ModelViewerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
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
        pushButton->setText(QApplication::translate("ModelViewerClass", "PushButton", 0));
        pushButton_2->setText(QApplication::translate("ModelViewerClass", "PushButton", 0));
        menu_File->setTitle(QApplication::translate("ModelViewerClass", "&File", 0));
    } // retranslateUi

};

namespace Ui {
    class ModelViewerClass: public Ui_ModelViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELVIEWER_H
