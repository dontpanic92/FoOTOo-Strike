#include "editor.h"
#include <QtWidgets/QApplication>
#include <qtextcodec.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qstylefactory.h>

int main(int argc, char *argv[])
{
	AllocConsole();
	freopen("CONOUT$", "w+t", stdout);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("gb18030"));


	QApplication a(argc, argv);
	qApp->setStyle(QStyleFactory::create("Fusion"));

	Editor w;
	w.show();
	return a.exec();
}
