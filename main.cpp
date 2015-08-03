#include "ccx-main-window.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CCXMainWindow w;
	w.show();
	return a.exec();
}
