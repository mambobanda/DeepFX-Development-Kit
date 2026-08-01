/*
* The Mailo Today Solutions Software License
*
* Copyright 2020 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#include <DfxCoreApplication.h>
#include "DfxPlayBlastBuilder.h"
#include "DfxTimelineWidget.h"

std::vector<QImage*> DfxPlayBlastBuilder::buildPlayBlast() {
	std::vector<QImage*> images;

	int fcount = DfxTimelineWidget::getInstance()->getFrameCount();

	QProgressDialog progress("Building Play Blast...", "Abort", 0, fcount, 0, Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
	progress.setWindowModality(Qt::ApplicationModal);
	progress.setWindowTitle("Play Blast");

	for (int i = 1; i < fcount + 1; i++) {
		progress.setValue(i);

		if (progress.wasCanceled())
			break;
		
		DfxTimelineWidget::getInstance()->setCurrentFrame(i);

		auto img = DfxCoreApplication::getInstance()->captureViewport();

		images.push_back(img);
	}

	progress.setValue(fcount);

	return images;
}