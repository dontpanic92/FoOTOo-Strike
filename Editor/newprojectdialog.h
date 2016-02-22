#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <QDialog>
#include <string>
#include "ui_newprojectdialog.h"

#pragma execution_character_set("utf-8")

class NewProjectDialog : public QDialog
{
	Q_OBJECT

public:
	NewProjectDialog(QWidget *parent = 0);
	~NewProjectDialog();
	bool IsAccepted();
	void GetParameters(QString& path, QString& name);

private:

	Ui::NewProjectDialog ui;
	QString mPath;
	QString mName;
	bool mIsAccepted = false;
};

#endif // NEWPROJECTDIALOG_H
