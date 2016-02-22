#include <qmessagebox.h>
#include <qdir.h>
#include <qfiledialog.h>
#include "newprojectdialog.h"

NewProjectDialog::NewProjectDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.browseButton, &QPushButton::clicked, [this](){
		QString dir = QFileDialog::getExistingDirectory(this, tr("ѡ��Ŀ¼"),
			"", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		ui.pathEdit->setText(dir);
	});
	
	connect(ui.buttonBox, &QDialogButtonBox::accepted, [this](){
		mPath = ui.pathEdit->text();
		if (mPath.isEmpty()) {
			QMessageBox::information(this, tr("��ʾ"), tr("��ѡ�񹤳�Ŀ¼"));
			return;
		}
		if (ui.nameEdit->text().isEmpty()) {
			QMessageBox::information(this, tr("��ʾ"), tr("����д��Ŀ����"));
			return;
		}
		if (!QDir(mPath).exists()) {
			QMessageBox::information(this, tr("��ʾ"), tr("Ŀ¼�����ڣ�������ѡ��"));
			return;
		}

		if (QDir(mPath).entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries).count() != 0) {
			QMessageBox::information(this, tr("��ʾ"), tr("Ŀ¼��Ϊ�գ�������ѡ��"));
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
