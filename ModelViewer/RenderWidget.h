#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QWidget>

class RenderWidget : public QWidget
{
	Q_OBJECT

public:
	RenderWidget(QWidget *parent);
	~RenderWidget();

	void paintEvent(QPaintEvent *);

signals:
	void onRefresh();

private:
	
};

#endif // RENDERWIDGET_H
