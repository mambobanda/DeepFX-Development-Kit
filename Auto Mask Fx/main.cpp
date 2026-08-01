#include <QTabWidget>
#include <DfxCoreApplication.h>
#include "MainMenu.h"
#include "ViewTabs.h"


int main(int argc, char *argv[])
{
	DfxCoreApplication::initialViewport = false;
	DfxCoreApplication::initQApplication(argc, argv);
	DfxCoreApplication::setFusionDarkTheme();

	DfxCoreApplication::setOrganizationName("DeepFX");
	DfxCoreApplication::setOrganizationDomain("deepfxworld.com");
	DfxCoreApplication::setApplicationName("Auto Mask FX");

	MainMenu *mainMenu = new MainMenu();
	ViewTabs *viewtabs = new ViewTabs();

	DfxCoreApplication::getInstance()->setMenuBar(mainMenu);
	//DfxCoreApplication::getInstance()->setCentralWidget(nullptr);
	DfxCoreApplication::getInstance()->setCentralWidget(viewtabs);

	return DfxCoreApplication::startAppMaximised();
}
