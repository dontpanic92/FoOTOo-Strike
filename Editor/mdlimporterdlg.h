#ifndef MDLIMPORTERDLG_H
#define MDLIMPORTERDLG_H

#include <QDialog>
#include "ui_mdlimporterdlg.h"

class MDLImporterDlg : public QDialog
{
	Q_OBJECT

public:
	MDLImporterDlg(QWidget *parent = 0);
	~MDLImporterDlg();

private:
	Ui::MDLImporterDlg ui;
};

#endif // MDLIMPORTERDLG_H
