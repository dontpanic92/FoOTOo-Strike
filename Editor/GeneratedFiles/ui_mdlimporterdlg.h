/********************************************************************************
** Form generated from reading UI file 'mdlimporterdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MDLIMPORTERDLG_H
#define UI_MDLIMPORTERDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MDLImporterDlg
{
public:

    void setupUi(QWidget *MDLImporterDlg)
    {
        if (MDLImporterDlg->objectName().isEmpty())
            MDLImporterDlg->setObjectName(QStringLiteral("MDLImporterDlg"));
        MDLImporterDlg->setWindowModality(Qt::ApplicationModal);
        MDLImporterDlg->resize(478, 328);

        retranslateUi(MDLImporterDlg);

        QMetaObject::connectSlotsByName(MDLImporterDlg);
    } // setupUi

    void retranslateUi(QWidget *MDLImporterDlg)
    {
        MDLImporterDlg->setWindowTitle(QApplication::translate("MDLImporterDlg", "MDLImporterDlg", 0));
    } // retranslateUi

};

namespace Ui {
    class MDLImporterDlg: public Ui_MDLImporterDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MDLIMPORTERDLG_H
