#include "QtScreenShared.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtScreenShared w;
	w.show();
	return a.exec();
}
