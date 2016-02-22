#include <qmessagebox.h>
#include <qdir.h>
#include <qfiledialog.h>
#include "newprojectdialog.h"

NewProjectDialog::NewProjectDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.browseButton, &QPushButton::clicked, [this](){
		QString dir = QFileDialog::getExistingDirectory(this, tr("选择目录"),
			"", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		ui.pathEdit->setText(dir);
	});
	
	connect(ui.buttonBox, &QDialogButtonBox::accepted, [this](){
		mPath = ui.pathEdit->text();
		if (mPath.isEmpty()) {
			QMessageBox::information(this, tr("提示"), tr("请选择工程目录"));
			return;
		}
		if (ui.nameEdit->text().isEmpty()) {
			QMessageBox::information(this, tr("提示"), tr("请填写项目名称"));
			return;
		}
		if (!QDir(mPath).exists()) {
			QMessageBox::information(this, tr("提示"), tr("目录不存在，请重新选择"));
			return;
		}

		if (QDir(mPath).entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries).count() != 0) {
			QMessageBox::information(this, tr("提示"), tr("目录不为空，请重新选择"));
			return;
		}

		
		mName = ui.nameEdit->text();
		mIsAccepted = true;
		this->close();
	});

	connect(ui.buttonBox, &QDialogButtonBox::rejected, [this](){
		this->close();
	});
}

NewProjectDialog::~NewProjectDialog()
{
}

bool NewProjectDialog::IsAccepted()
{
	return mIsAccepted;
}

void NewProjectDialog::GetParameters(QString& path, QString& name)
{
	path = mPath;
	name = mName;
}
