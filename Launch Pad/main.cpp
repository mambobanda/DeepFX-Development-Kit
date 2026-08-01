#include <QtWidgets/QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
  
	QApplication app(argc, argv);

	QApplication::setOrganizationName("DeepFX");
	QApplication::setOrganizationDomain("deepfxworld.com");
	QApplication::setApplicationName("Launch Pad");

	MainWindow mwindow;
	mwindow.setMinimumWidth(900);
	mwindow.setMinimumHeight(600);
	mwindow.show();

    return app.exec();
}
