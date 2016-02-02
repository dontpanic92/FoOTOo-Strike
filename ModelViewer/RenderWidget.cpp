#include "RenderWidget.h"
#include <qmessagebox.h>

RenderWidget::RenderWidget(QWidget *parent)
	: QWidget(parent)
{

}

RenderWidget::~RenderWidget()
{

}

void RenderWidget::paintEvent(QPaintEvent *e)
{
	//QMessageBox::information(nullptr, "adf", "asdf");
	emit onRefresh();
	//QWidget::paintEvent(e);
}
