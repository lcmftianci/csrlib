#include "QtVideoShow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtVideoShow w;
	w.show();
	return a.exec();
}
