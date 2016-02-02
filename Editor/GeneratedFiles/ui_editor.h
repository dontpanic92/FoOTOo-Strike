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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <../ModelViewer/RenderWidget.h>

QT_BEGIN_NAMESPACE

class Ui_EditorClass
{
public:
    QAction *actionExit;
    QAction *actionCS;
    QWidget *centralWidget;
    RenderWidget *widget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QMenuBar *menuBar;
    QMenu *menuAaa;
    QMenu *menu_I;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *EditorClass)
    {
        if (EditorClass->objectName().isEmpty())
            EditorClass->setObjectName(QStringLiteral("EditorClass"));
        EditorClass->resize(622, 400);
        actionExit = new QAction(EditorClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionCS = new QAction(EditorClass);
        actionCS->setObjectName(QStringLiteral("actionCS"));
        centralWidget = new QWidget(EditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        widget = new RenderWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 10, 411, 291));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(440, 100, 75, 23));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(440, 140, 75, 23));
        EditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(EditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 622, 23));
        menuAaa = new QMenu(menuBar);
        menuAaa->setObjectName(QStringLiteral("menuAaa"));
        menu_I = new QMenu(menuBar);
        menu_I->setObjectName(QStringLiteral("menu_I"));
        menu = new QMenu(menu_I);
        menu->setObjectName(QStringLiteral("menu"));
        EditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(EditorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        EditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(EditorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        EditorClass->setStatusBar(statusBar);

        menuBar->addAction(menuAaa->menuAction());
        menuBar->addAction(menu_I->menuAction());
        menuAaa->addAction(actionExit);
        menu_I->addAction(menu->menuAction());
        menu->addAction(actionCS);

        retranslateUi(EditorClass);
        QObject::connect(actionExit, SIGNAL(triggered()), EditorClass, SLOT(close()));

        QMetaObject::connectSlotsByName(EditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *EditorClass)
    {
        EditorClass->setWindowTitle(QApplication::translate("EditorClass", "Editor", 0));
        actionExit->setText(QApplication::translate("EditorClass", "\351\200\200\345\207\272(&E)", 0));
        actionCS->setText(QApplication::translate("EditorClass", "CS 1.6 MDL\346\226\207\344\273\266 ...", 0));
        pushButton->setText(QApplication::translate("EditorClass", "Button1", 0));
        pushButton_2->setText(QApplication::translate("EditorClass", "Button2", 0));
        menuAaa->setTitle(QApplication::translate("EditorClass", "\346\226\207\344\273\266(&F)", 0));
        menu_I->setTitle(QApplication::translate("EditorClass", "\350\265\204\346\272\220(&R)", 0));
        menu->setTitle(QApplication::translate("EditorClass", "\345\257\274\345\205\245", 0));
    } // retranslateUi

};

namespace Ui {
    class EditorClass: public Ui_EditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITOR_H
