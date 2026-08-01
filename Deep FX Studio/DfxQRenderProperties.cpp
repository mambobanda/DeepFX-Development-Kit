/*
* The Mailo Today Solutions Software License
*
* Copyright 2019 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/


#include "DfxRenderRegistry.h"
#include "DfxQRenderProperties.h"

DfxQRenderProperties::DfxQRenderProperties(QWidget *parent) : QWidget(parent) {

	setWindowTitle("Render Properties");

	QWidget *central = new QWidget;
	scrollArea = new QScrollArea();

	groupLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom, central);
	mainLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);

	/*
	mainLayout->setMargin(0);
	groupLayout->setMargin(0);
	*/

	renderer = new QGroupBox("Render Target");
	renRefresh = new QWidget();
	rendererLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
	rendererForm = new QFormLayout();
	rendererList = new QComboBox();
	refresh = new QPushButton(QIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload)), "");

	rendererLayout->addWidget(rendererList);
	rendererLayout->addWidget(refresh);
	renRefresh->setLayout(rendererLayout);
	rendererForm->addRow( "Renderer", renRefresh);
	renderer->setLayout(rendererForm);

	resolution = new QGroupBox("Resolution");
	resolutionForm = new QFormLayout();
	width  = new QSpinBox();
	height = new QSpinBox();

	resolutionForm->addRow("Width:", width);
	resolutionForm->addRow("Height:", height);
	resolution->setLayout(resolutionForm);

	frames = new QGroupBox("Frames");
	framesForm = new QFormLayout();
	startFrame = new QSpinBox();
	endFrame = new QSpinBox();

	framesForm->addRow("Start Frame:", startFrame);
	framesForm->addRow("End Frame:", endFrame);
	frames->setLayout(framesForm);

	renderButtonLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
	
	renderFrame = new QPushButton("Render");
	renderAnimation = new QPushButton("Render Animation");

	renderButtonLayout->addWidget(renderFrame);
	renderButtonLayout->addWidget(renderAnimation);

	groupLayout->addWidget(renderer);
	//groupLayout->addWidget(resolution);
	//groupLayout->addWidget(frames);

	scrollArea->setWidget(central);
	scrollArea->setWidgetResizable(true);

	mainLayout->addWidget(scrollArea);
	mainLayout->addLayout(renderButtonLayout);

	setLayout(mainLayout);

	QObject::connect(refresh, &QPushButton::clicked, this, &DfxQRenderProperties::refreshRendererList);
	QObject::connect(renderFrame, &QPushButton::clicked, this, &DfxQRenderProperties::renderSlot);
	QObject::connect(renderAnimation, &QPushButton::clicked, this, &DfxQRenderProperties::renderAnimationSlot);
}

DfxQRenderProperties::~DfxQRenderProperties() {

}

void DfxQRenderProperties::refreshRendererList() {

	rendererList->clear();

	auto list = DfxRenderRegistry::getIntstance()->findAllTypes(DfxRenderRegistry::RENDERER);

	for (auto ls : list) {
		rendererList->addItem(ls->getLogicalName().c_str(), ls->getInstanceID());
	}

}

void DfxQRenderProperties::renderSlot() {

	if (!renderViewer.isFinished()) {
		DfxGlobal::console::println("Render: Already Rendering...");
		return;
	}

	auto qv = rendererList->itemData(rendererList->currentIndex());
	size_t uid = qv.value<size_t>();

	auto node = DfxRenderRegistry::getIntstance()->find(uid);

	if (node != nullptr ) {

		if (node->available()) {
			renderViewer.setRenderInterface(node);
			renderViewer.startRender();
		}
		/*std::vector<size_t> uids;
		auto views = DfxLayerManager::getInstance()->findAllViewItems();

		for (auto vit : views) {
			if (vit->isRenderable() || 
				vit->UniqueName().compare("2F2X252X_DFX_CAMERA") == 0 ||
				vit->UniqueName().compare("2F2X252X_DFX_LIGHT") == 0) {

				uids.push_back(vit->getInstanceID());
			}
		}
		
		node->newFrame(uids);
		node->render();

		saveImage("c:/test/final_image.png", node->getWidth(), node->getHeight(), node->getFinalImageBuffer(), 0);

		node->cleanUp();*/
	}
	else {
		DfxGlobal::console::println("Render: Couldn't find render node.");
	}
}

void DfxQRenderProperties::renderAnimationSlot() {

}

void DfxQRenderProperties::saveImage(std::string path, int width, int height, unsigned char *data, unsigned int size) {
	
	if (data != nullptr) {
		QImage image(width, height, QImage::Format::Format_RGBA8888);

		for (int i = 0; i < width; i +=4 )
		{
			for (int u = 0; u < height; u +=4)
			{
				image.setPixel(i, u, qRgb(data[i], data[i+1], data[i+2]));
			}
		}

		bool rc = image.save(QString(path.c_str()), "PNG");

		if (!rc) DfxGlobal::console::println("Render Failed to Save Image");
	}
	else {
		DfxGlobal::console::println("Render Failed to Save Image: NULL Data");
	}
}