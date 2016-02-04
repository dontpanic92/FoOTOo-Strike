#include "RenderWidget.h"
#include <qmessagebox.h>
#include <qevent.h>

RenderWidget::RenderWidget(QWidget *parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);
}

RenderWidget::~RenderWidget()
{

}

void RenderWidget::mouseMoveEvent(QMouseEvent* e)
{
	QPoint d = e->pos();
	emit onDrag(d - mDragStartPoint);
	mDragStartPoint = d;
}

void RenderWidget::mousePressEvent(QMouseEvent *e)
{
	mDragStartPoint = e->pos();
}

void RenderWidget::paintEvent(QPaintEvent *e)
{
	emit onRefresh();
}

void RenderWidget::resizeEvent(QResizeEvent* e)
{
	emit onResize(e->size(), e->oldSize());
}

void RenderWidget::wheelEvent(QWheelEvent *e)
{
	emit onWheel(e->delta());
}
