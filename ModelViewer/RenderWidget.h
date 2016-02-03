#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QWidget>

class RenderWidget : public QWidget
{
	Q_OBJECT

public:
	RenderWidget(QWidget *parent);
	~RenderWidget();
	QPaintEngine* paintEngine() const override{ return nullptr; }

	void paintEvent(QPaintEvent *) override;
	//bool event(QEvent *) override;
	void mouseMoveEvent(QMouseEvent *) override;
	void mousePressEvent(QMouseEvent *) override;
	void resizeEvent(QResizeEvent*) override;

signals:
	void onRefresh();
	void onDrag(QPoint delta);
	void onResize(QSize newSize, QSize oldSize);

private:
	
	QPoint mDragStartPoint;
};

#endif // RENDERWIDGET_H
