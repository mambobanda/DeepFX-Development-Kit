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
#include "DfxLuxCoreRenderSession.h"
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

class DfxLuxCoreRenderNode: public QObject, public DfxRenderNodeBase {
	Q_OBJECT
private:
	static std::string _uniqueName;

private:
	QWidget *propertiesForm();
	DfxLuxCoreRenderSession *_renderSession;
	DfxRenderOutput *_renderOutput;
	
	bool _renderingStill;
	bool _renderingSequence;

private:

	std::vector<std::pair<std::string, unsigned int>> deviceQControlenums;
	std::vector<std::pair<std::string, unsigned int>> engineQControlenums;

	std::vector<std::pair<std::string, unsigned int>> samplerQControlenums;

	std::vector<std::pair<std::string, unsigned int>> pixelFilterQControlenums;

	std::vector<std::pair<std::string, unsigned int>> haltQControlenums;

	std::vector<std::pair<std::string, unsigned int>> denoiserQControlenums;

	std::vector<std::pair<std::string, unsigned int>> tonemapTypeQControlenums;

	std::vector<std::pair<std::string, unsigned int>> imageNamingConventionTypeQControlenums;

	std::vector<std::pair<std::string, unsigned int>> imageFileFormatTypeQControlenums;

public:

	struct Device { enum { CPU = 0, OPENCL = 1 }; };
	struct Engine { enum { PATH = 0, BIDIR= 1 }; };

	struct Sampler { enum { SOBOL = 0, METROPOLIS = 1, RANDOM = 3}; };

	struct PixelFilter { enum { BLACKMAN = 0, GAUSSIAN = 1, MITCHELL = 2, NONE = 3 }; };

	struct Halt { enum { SAMPLES = 0, TIME = 1 }; };

	struct Denoiser { enum { OIDN = 0, }; };

	struct TonemapType { enum { LINEAR = 0, CAMERA_SETTINGS = 1, REINHARD = 2 }; };

	struct ImageNamingConvention { enum { PREFIX_BUFFER_FRAME = 0, BUFFER_FRAME = 1 }; };

	struct ImageFileFormat { enum { PNG = 0, JPEG = 1 }; };

private:

	DfxLxCameraHandler getCamera();
	void saveStill();
	void saveSequence(int frame);

	float _varianceEstimate;
public:

	DfxLuxCoreRenderNode();
	virtual ~DfxLuxCoreRenderNode();

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
	void debugExport();
	void renderSequence();
	void stopAllRendering();
};