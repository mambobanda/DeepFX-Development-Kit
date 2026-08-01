#include "DeepFXNodeEditor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DeepFXNodeEditor w;
	w.show();
	return a.exec();
}
