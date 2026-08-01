/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
 *
 * No Permission is granted, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software.
 */
 /**
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once


#include <DfxRenderNodeBase.h>
#include <QBoxLayout.h>
#include <QTabWidget>
#include <QObject>
#include <filesystem>
#include <QString>
#include <QLabel>
#include <DfxTimelineManager.h>
#include "DfxRenderHub.h"
#include "DfxOSPRayRenderSession.h"
#include "DfxRenderTarget.h"
#include "DfxQControlPanel.h"
#include "DfxQControlArray.h"
#include "DfxQBoolControl.h"
#include "DfxQDecimalSpinnerControl.h"
#include "DfxQIntegerSpinnerControl.h"
#include "DfxQComboBoxControl.h"
#include "DfxQFluentPathControl.h"
#include "DfxQFilePathControl.h"
#include "DfxQLineEditControl.h"
#include "DfxQEmptyWidget.h"
#include "DfxQStackControl.h"
#include "DfxQCheckBoxControl.h"
#include "DfxQPushButtonStyles.h"
#include "DeepFxStudioLicenseManager.h"
#include "DfxLicenseInfoDialog.h"

namespace filesystem = std::experimental::filesystem;

class DfxOSPRayRenderNode : public QObject, public DfxRenderNodeBase {
	Q_OBJECT
private:
	static std::string _uniqueName;

private:
	QWidget *propertiesForm();
	DfxOSPRayRenderSession *_renderSession;
	DfxRenderOutput *_renderOutput;

	bool _renderingStill;
	bool _renderingSequence;

private:

	std::vector<std::pair<std::string, unsigned int>> engineQControlenums;

	std::vector<std::pair<std::string, unsigned int>> pixelFilterQControlenums;

	std::vector<std::pair<std::string, unsigned int>> imageFileFormatTypeQControlenums;

	std::vector<std::pair<std::string, unsigned int>> imageNamingConventionTypeQControlenums;

	std::vector<std::pair<std::string, unsigned int>> haltQControlenums;


public:

	struct Engine { enum { SCI_VIS = 0, AO = 1, PATH_TRACER = 2 }; };

	struct PixelFilter { enum { POINT = 0, BOX = 1,  GAUSS = 2,  MITCHELL = 3, BLACKMAN = 4 }; };

	struct ImageNamingConvention { enum { PREFIX_BUFFER_FRAME = 0, BUFFER_FRAME = 1 }; };

	struct ImageFileFormat { enum { PNG = 0, JPEG = 1 }; };

	struct Halt { enum { VARIANCE = 0, TIME = 1 }; };

private:

	DfxOSPRayCameraHandler getCamera();
	void saveStill();
	void saveSequence(int frame);

	float _varianceEstimate;
public:

	DfxOSPRayRenderNode();
	virtual ~DfxOSPRayRenderNode();

	void initRenderNode();

	void renderFrame();
	void stopRendering() override;

	DfxRenderOutput *renderOutput() override;

	DfxRenderSessionInterface *renderSession() override;

	std::string uniqueName() override;
	static std::string getUniqueName();

	std::vector<std::pair<QWidget*, QString>> propertyForms();

signals:
	void varianceEstimateUpdated(QString);

public slots:

	void renderStill();
	void renderSequence();
	void stopAllRendering();
};