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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include <SgPointCloudNode.h>
#include "DfxPointCloudMesherNode.h"
#include "DfxPointCloudMesher.h"
#include "DfxPointSampling.h"
#include "DfxOpenGLCacheQueue.h"

std::string DfxPointCloudMesherNode::_uniqueName = "UID_POINT_CLOUD_MESHER_NODE";

DfxPointCloudMesherNode::DfxPointCloudMesherNode() {

	_parent = nullptr;
	_dispatcher = nullptr;
	_active = false;

	unitTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Resolution", Units::RESOLUTION));
	unitTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Discreet", Units::DISCREET));

	smoothingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("None", Smoothing::NONE));
	smoothingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Mean", Smoothing::MEAN));
	smoothingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Median", Smoothing::MEDIAN));
	smoothingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Gaussian", Smoothing::GAUSSIAN));
	smoothingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Laplacian", Smoothing::LAPLACIAN));
	smoothingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Mean Curvature", Smoothing::MEAN_CURVATURE));

	gridScalingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("1/1", GridSpacing::_1_1));
	gridScalingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("1/1.1", GridSpacing::_1_2));
	gridScalingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("1/1.2", GridSpacing::_1_3));
	gridScalingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("1/1.3", GridSpacing::_1_4));
	gridScalingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("1/1.4", GridSpacing::_1_5));
	gridScalingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("1/1.5", GridSpacing::_1_6));
	gridScalingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("1/1.6", GridSpacing::_1_7));
	gridScalingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("1/1.7", GridSpacing::_1_8));
	gridScalingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("1/1.8", GridSpacing::_1_9));
	gridScalingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("1/1.9", GridSpacing::_1_10));
	gridScalingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("1/2", GridSpacing::_1_11));
	gridScalingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("1/2.5", GridSpacing::_1_12));
	gridScalingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("1/3", GridSpacing::_1_13));
	gridScalingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("1/4", GridSpacing::_1_14));
	gridScalingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("1/5", GridSpacing::_1_15));

	_generate = false;

}

DfxPointCloudMesherNode::~DfxPointCloudMesherNode() {

}

std::string DfxPointCloudMesherNode::uniqueName() {
	return _uniqueName;
}

std::string DfxPointCloudMesherNode::getUniqueName() {
	return _uniqueName;
}

void DfxPointCloudMesherNode::onSetReferencePath(std::string path) {
	if (_dispatcher != nullptr) {
		delete _dispatcher;
		_dispatcher = nullptr;
	}

	auto any = DfxSpace::getInstance()->findAbsolute(path);

	if (any != nullptr) {
		_dispatcher = new DfxToolFlDispatcher(this);
		_dispatcher->attachBranch(any);
	}

	DfxToolNode::onSetReferencePath(path);
}

bool DfxPointCloudMesherNode::isReady() {
	return true;
}

bool DfxPointCloudMesherNode::evaluateOnUpdate() {
	return false;
}

bool DfxPointCloudMesherNode::evaluateOnNewFrame() {
	return true;
}

bool DfxPointCloudMesherNode::evaluateOnValueChanged() {
	return true;
}

void DfxPointCloudMesherNode::create(SgNode *parent) {
	if (parent->getType() == deepsg::SgNodeType::GEOMETRY) {
		auto geo = (deepsg::SgGeoNode*)parent;

		if (geo->requestDynaMesh()) {
			_parent = geo;
			_active = true;
		}
	}
}

void DfxPointCloudMesherNode::destroy() {
	if (_active) {
		_parent->releaseDynaMesh();
		_parent = nullptr;
		_active = false;
	}
}

float DfxPointCloudMesherNode::gridScalingFraction(int scaling) {

	switch (scaling)
	{
	case 0:
		return 1.f / 1.f;
		break;
	case 1:
		return 1.f / 1.1f;
		break;
	case 2:
		return 1.f / 1.2f;
		break;
	case 3:
		return 1.f / 1.3f;
		break;
	case 4:
		return 1.f / 1.4f;
		break;
	case 5:
		return 1.f / 1.5f;
		break;
	case 6:
		return 1.f / 1.6f;
		break;
	case 7:
		return 1.f / 1.7f;
		break;
	case 8:
		return 1.f / 1.8f;
		break;
	case 9:
		return 1.f / 1.9f;
		break;
	case 10:
		return 1.f / 2.f;
		break;
	case 11:
		return 1.f / 2.5f;
		break;
	case 12:
		return 1.f / 3.f;
		break;
	case 13:
		return 1.f / 4.f;
		break;
	case 14:
		return 1.f / 5.f;
		break;
	default:
		return 1.f;
		break;
	}

	return 1.f;
}

void DfxPointCloudMesherNode::evaluate(SgNode *parent) {

	if (parent->getType() == deepsg::SgNodeType::GEOMETRY) {

		auto geo = (deepsg::SgGeoNode*)parent;
		auto adapter = geo->getGeometryAdapter();

		if (adapter->type() == DfxGeoAdapterType::DYNA_MESH) {

			auto any = DfxSpace::getInstance()->findAbsolute(referencePath());
			auto path = any->rawData<fl::DataObject>()->property("point_cloud")->data<std::string>();

			auto dynaMesh = (DfxDynaMesh*)geo->getDynaMesh();
			dynaMesh->cache()->clear();
			dynaMesh->clearAll();

			if (any != nullptr) {
				auto pcloudAny = DfxSpace::getInstance()->findAbsolute(path);

				if (pcloudAny != nullptr) {
					if (pcloudAny->type() == fl::type::DATA_OBJECT) {
						
						auto dao = DfxSpace::getInstance()->findAbsolute(referencePath())->rawData<fl::DataObject>();
						auto pcloud_dao = pcloudAny->rawData<fl::DataObject>();

						auto pcloudNodeAny = pcloud_dao->property("sg_point_cloud_node");

						auto units = dao->property("units")->data<unsigned int>();
						auto groupid = dao->property("group_id")->data<int>();
						auto resolution = dao->property("resolution")->data<unsigned int>();
						auto grid_scaling = dao->property("grid_scaling")->data<unsigned int>();
						auto grid_size  = dao->property("grid_size")->data<float>();
						auto point_diameter = dao->property("point_diameter")->data<float>();

						auto auto_mesh = dao->property("auto_generate_mesh")->data<bool>();
						auto dilate_offset = dao->property("dilate_filter_offset")->data<float>();
						auto erode_offset = dao->property("erode_filter_offset")->data<float>();

						auto stype = dao->property("smoothing_filter")->data<unsigned int>();

						auto mean_width = dao->property("mean_filter_width")->data<float>();
						auto mean_iter = dao->property("mean_filter_iterations")->data<unsigned int>();

						auto median_width = dao->property("median_filter_width")->data<float>();
						auto median_iter = dao->property("median_filter_iterations")->data<unsigned int>();

						auto gaussian_width = dao->property("gaussian_filter_width")->data<float>();
						auto gaussian_iter  = dao->property("gaussian_filter_iterations")->data<unsigned int>();

						auto laplacian_iter = dao->property("laplacian_filter_iterations")->data<unsigned int>();
						auto mean_curve_iter = dao->property("mean_curvature_filter_iterations")->data<unsigned int>();

						float g_size = 0.5f;
						float p_size = 0.5f;

						std::vector<DfxPointCloudMesherFilters::Filter*> filters;

						if (units == Units::RESOLUTION) {
							p_size = DfxPointSampling::particleRadiusFromResolution(std::max((unsigned int)1, resolution));
							g_size = p_size * gridScalingFraction(grid_scaling);
						}else if(units == Units::DISCREET){
							g_size = grid_size;
							p_size = point_diameter * .5f;
						}
					
						if (dilate_offset > 0) {
							auto filter = new DfxPointCloudMesherFilters::DilateFilter;
							filter->iterations = 1;
							filter->offset = -1.f * dilate_offset;
							filters.push_back(filter);
						}

						if (stype == 1) {
							auto filter = new DfxPointCloudMesherFilters::MeanFilter;
							filter->iterations = mean_iter;
							filter->width = mean_width;
							filters.push_back(filter);
						}
						else if (stype == 2) {
							auto filter = new DfxPointCloudMesherFilters::MedianFilter;
							filter->iterations = median_iter;
							filter->width = median_width;
							filters.push_back(filter);
						}
						else if (stype == 3) {
							auto filter = new DfxPointCloudMesherFilters::GaussianFilter;
							filter->iterations = gaussian_iter;
							filter->width = gaussian_width;
							filters.push_back(filter);
						}
						else if (stype == 4) {
							auto filter = new DfxPointCloudMesherFilters::LaplacianFilter;
							filter->iterations = laplacian_iter;
							filters.push_back(filter);
						}
						else if (stype == 5) {
							auto filter = new DfxPointCloudMesherFilters::MeanCurvatureFilter;
							filter->iterations = mean_curve_iter;
							filters.push_back(filter);
						}

						if (erode_offset > 0) {
							auto filter = new DfxPointCloudMesherFilters::ErodeFilter;
							filter->iterations = 1;
							filter->offset = erode_offset;
							filters.push_back(filter);
						}

						if (_generate || auto_mesh) {
							_generate = false;

							if (pcloudNodeAny != nullptr) {
								auto pcloudNode = (DfxPointCloudNode*)pcloudNodeAny->rawData<deepsg::SgPointCloudNode>();

								auto pcloud = pcloudNode->geo();

								if (pcloud->size() > 0) {

									DfxPointCloudMesher mesher;
									mesher.setGroup(groupid);
									auto outMesh = mesher.meshParticles(pcloudNode, g_size, p_size, filters);

									if (outMesh != nullptr) {

										if (outMesh->nbVerts >= 3 && outMesh->nbFaces > 0) {
											dynaMesh->copyMesh(outMesh, parent->referencePath());
											DfxOpenGLCacheQueue::getInstance()->queueCache(dynaMesh->cache());
										}

										outMesh->clear();
										delete outMesh;
									}

								}
							}
						}
					}
				}
			}
		}
	}
}

void DfxPointCloudMesherNode::processSignal(size_t signal) {

	if (signal == deepsg::SgNode::Signal::UPDATED) {
		if (_parent != nullptr) {
			_parent->evaluateTools();
		}
	}

}

void DfxPointCloudMesherNode::generateMesh() {
	if (_parent != nullptr) {
		_generate = true;
		_parent->evaluateTools();
		DfxUpdater::getInstance()->sceneUpdated();
	}
}

void DfxPointCloudMesherNode::pcloudPathUpdated(std::string path) {
	auto any = DfxSpace::getInstance()->findAbsolute(path);

	ignoreAll();
	if (any != nullptr) {
		if (any->type() == fl::type::DATA_OBJECT) {
			auto dao = any->rawData<fl::DataObject>();

			auto pcloudAny = dao->property("sg_point_cloud_node");

			if (pcloudAny != nullptr) {
				auto pcloudNode = pcloudAny->rawData<deepsg::SgPointCloudNode>();
				listenToNode(pcloudNode);
			}
			
		}
	}
}

std::vector<std::pair<QWidget*, QString>> DfxPointCloudMesherNode::propertyForms() {
	std::pair<QWidget*, QString> properties(propertyForm(), "Properties");

	return std::vector<std::pair<QWidget*, QString>> { properties };
}

QWidget *DfxPointCloudMesherNode::propertyForm() {
	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	DfxQStackControl *unitWidgetStack = new DfxQStackControl;
	DfxQStackControl *smoothingWidgetStack = new DfxQStackControl;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto propertiesControlPanel = new DfxControlPanel();
	auto filtersControlPanel = new DfxControlPanel();

	DfxQControlArray *propertiesControlArray = new DfxQControlArray();
	DfxQControlArray *filtersControlArray = new DfxQControlArray();

	DfxQControlArray *resolutionControlArray = new DfxQControlArray();
	DfxQControlArray *discreetControlArray = new DfxQControlArray();

	DfxQControlArray *meanControlArray = new DfxQControlArray();
	DfxQControlArray *medianControlArray = new DfxQControlArray();
	DfxQControlArray *gaussianControlArray = new DfxQControlArray();
	DfxQControlArray *laplacianControlArray = new DfxQControlArray();
	DfxQControlArray *meanCurvatureControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	// Controls
	QPushButton *generate = new QPushButton("Generate Mesh");
	generate->setStyleSheet(generate->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());

	auto pcloud = new DfxQFluentPathControl(path + "point_cloud", "", false);

	auto group = new DfxQIntegerSpinnerControl(path + "group_id", "", false);

	auto units = new DfxQComboBoxControl(path + "units", "", unitTypeQControlenums, false);
	auto resolution = new DfxQIntegerSpinnerControl(path + "resolution", "", false);
	auto grid_scaling = new DfxQComboBoxControl(path + "grid_scaling", "", gridScalingTypeQControlenums, false);;
	auto grid_size = new DfxQDecimalSpinnerControl(path + "grid_size", "", false);
	auto point_diameter = new DfxQDecimalSpinnerControl(path + "point_diameter", "", false);

	auto auto_gen_mesh = new DfxQCheckBoxControl(path + "auto_generate_mesh", "", false);

	auto dilate_offset = new DfxQDecimalSpinnerControl(path + "dilate_filter_offset", "", false);

	auto erode_offset = new DfxQDecimalSpinnerControl(path + "erode_filter_offset", "", false);

	auto smoothing = new DfxQComboBoxControl(path + "smoothing_filter", "",smoothingTypeQControlenums, false);

	auto mean_width = new DfxQIntegerSpinnerControl(path + "mean_filter_width", "", false); 
	auto mean_iter = new DfxQIntegerSpinnerControl(path + "mean_filter_iterations", "", false);

	auto median_width = new DfxQIntegerSpinnerControl(path + "median_filter_width", "", false);
	auto median_iter = new DfxQIntegerSpinnerControl(path + "median_filter_iterations", "", false);

	auto gaussian_width = new DfxQIntegerSpinnerControl(path + "gaussian_filter_width", "", false);
	auto gaussian_iter  = new DfxQIntegerSpinnerControl(path + "gaussian_filter_iterations", "", false);

	auto laplacian_iter = new DfxQIntegerSpinnerControl(path + "laplacian_filter_iterations", "", false);

	auto mean_curv_iter = new DfxQIntegerSpinnerControl(path + "mean_curvature_filter_iterations", "", false);

	// Properties
	propertiesControlArray->addControl(generate);
	propertiesControlArray->addSpacer(1, 2);
	propertiesControlArray->addControl(auto_gen_mesh, "Auto Generate");
	propertiesControlArray->addSpacer(1, 10);
	propertiesControlArray->addControl("Point Cloud", pcloud);
	propertiesControlArray->addSpacer(1, 2); 
	propertiesControlArray->addControl("Group", group);
	propertiesControlArray->addSpacer(1, 2);
	propertiesControlArray->addControl("Units", units);
	propertiesControlArray->addControl(unitWidgetStack);

	resolutionControlArray->addControl("Resolution", resolution);
	resolutionControlArray->addControl("Grid Scaling", grid_scaling);

	discreetControlArray->addControl("Grid Size", grid_size);
	discreetControlArray->addSpacer(1, 2);
	discreetControlArray->addControl("Point Diameter", point_diameter);

	unitWidgetStack->addPage(resolutionControlArray);
	unitWidgetStack->addPage(discreetControlArray);

	unitWidgetStack->changeCurrentPage(units->comboBox()->currentIndex());

	propertiesControlPanel->setTitle("General");
	propertiesControlPanel->setWidget(propertiesControlArray);

	// Filters
	filtersControlArray->addControl("Dilate Offset", dilate_offset);
	filtersControlArray->addSpacer(1, 2);
	filtersControlArray->addControl("Erode Offset", erode_offset);
	filtersControlArray->addSpacer(1, 5);
	filtersControlArray->addControl("Smoothing", smoothing);
	filtersControlArray->addControl(smoothingWidgetStack);

	meanControlArray->addControl("Width", mean_width);
	meanControlArray->addSpacer(1, 2);
	meanControlArray->addControl("Iterations", mean_iter);

	medianControlArray->addControl("Width", median_width);
	medianControlArray->addSpacer(1, 2);
	medianControlArray->addControl("Iterations", median_iter);

	gaussianControlArray->addControl("Width", gaussian_width);
	gaussianControlArray->addSpacer(1, 2);
	gaussianControlArray->addControl("Iterations", gaussian_iter);

	laplacianControlArray->addControl("Iterations", laplacian_iter);

	meanCurvatureControlArray->addControl("Iterations", mean_curv_iter);

	filtersControlPanel->setTitle("Smoothing");
	filtersControlPanel->setWidget(filtersControlArray);

	smoothingWidgetStack->addPage(new DfxQEmptyWidget());
	smoothingWidgetStack->addPage(meanControlArray);
	smoothingWidgetStack->addPage(medianControlArray);
	smoothingWidgetStack->addPage(gaussianControlArray);
	smoothingWidgetStack->addPage(laplacianControlArray);
	smoothingWidgetStack->addPage(meanCurvatureControlArray);

	smoothingWidgetStack->changeCurrentPage(smoothing->comboBox()->currentIndex());

	main_layout->addWidget(propertiesControlPanel);
	main_layout->addWidget(filtersControlPanel);
	widget->setLayout(main_layout);

	QObject::connect(generate, &QPushButton::clicked, this, &DfxPointCloudMesherNode::generateMesh);
	QObject::connect(pcloud, &DfxQFluentPathControl::fluentPathUpdated, this, &DfxPointCloudMesherNode::pcloudPathUpdated);

	QObject::connect(units->comboBox(), QOverload<int>::of(&QComboBox::activated), unitWidgetStack, &DfxQStackControl::changeCurrentPage);
	QObject::connect(smoothing->comboBox(), QOverload<int>::of(&QComboBox::activated), smoothingWidgetStack, &DfxQStackControl::changeCurrentPage);

	return widget;
}