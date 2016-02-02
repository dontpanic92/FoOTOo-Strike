#include "modelviewer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ModelViewer w;
	w.show();
	return a.exec();
}
