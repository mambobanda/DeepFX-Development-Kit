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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include <SgPointCloudNode.h>
#include <DfxPointCloudNode.h>
#include <DfxGroupNode.h>
#include <Octree.hpp>
#include <set>
#include <DfxUpdater.h>
#include "DfxBounds.h"
#include "DfxQPushButtonStyles.h"
#include "DfxPointProximityKillTool.h"
#include "DfxPointProximityKillHandler.h"

std::string DfxPointProximityKillTool::_uniqueName = "UID_POINT_PROXIMITY_KILL_TOOL";

DfxPointProximityKillTool::DfxPointProximityKillTool() {

	_dispatcher = nullptr;

	proximityDomainEnums.push_back(std::pair<std::string, unsigned int>("Sphere", ProximityDomain::RADIUS));
	proximityDomainEnums.push_back(std::pair<std::string, unsigned int>("Shape Bounding Box", ProximityDomain::SHAPE_AABB));
	proximityDomainEnums.push_back(std::pair<std::string, unsigned int>("Shape Bounding Sphere", ProximityDomain::SHAPE_BS));

	skipEvaluateOnUpdate = false;
}

DfxPointProximityKillTool::~DfxPointProximityKillTool() {

}

std::string DfxPointProximityKillTool::uniqueName() {
	return _uniqueName;
}

std::string DfxPointProximityKillTool::getUniqueName() {
	return _uniqueName;
}

bool DfxPointProximityKillTool::isReady() {
	return true;
}

void DfxPointProximityKillTool::onSetReferencePath(std::string path) {
	if (_dispatcher != nullptr) {
		delete _dispatcher;
		_dispatcher = nullptr;
	}

	auto any = DfxSpace::getInstance()->findAbsolute(path);

	if (any != nullptr) {
		_dispatcher = new DfxToolFlDispatcher(this);
		_dispatcher->attachBranch(any, std::vector<std::string> {"live"});
	}

	DfxToolNode::onSetReferencePath(path);
}

void DfxPointProximityKillTool::create(SgNode *parent) {
}

void DfxPointProximityKillTool::destroy() {
}

bool DfxPointProximityKillTool::evaluateOnUpdate() {

	if (skipEvaluateOnUpdate) return false;

	if (deepsg::SgNode::parent() != nullptr) {
		DfxPointProximityKillHandler handler;

		if (handler.fetch(referencePath())) {
			auto dao = handler.dataObject()->rawData<fl::DataObject>();

			if (dao->property("live")->data<bool>()) {
				if (dao->property("accumulate")->data<bool>()) return true;
				else {
					skipEvaluateOnUpdate = true;
					deepsg::SgNode::parent()->evaluateTools();
					skipEvaluateOnUpdate = false;
					return false;
				}
			}
		}
	}

	return false;
}

bool DfxPointProximityKillTool::evaluateOnNewFrame() {
	return true;
}

bool DfxPointProximityKillTool::evaluateOnValueChanged() {
	return true;
}

void DfxPointProximityKillTool::evaluate(SgNode *parent) {
	DfxPointProximityKillHandler handler;

	if (parent->getType() == deepsg::SgNodeType::POINT_CLOUD) {

		auto pcloud = ((deepsg::SgPointCloudNode*)parent)->geo();

		if (handler.fetch(referencePath())) {

			auto dao = handler.dataObject()->rawData<fl::DataObject>();

			auto reference = dao->property("reference")->data<std::string>();
			auto domain = dao->property("domain")->data<unsigned int>();
			auto radius = dao->property("radius")->data<float>();
			auto padding = dao->property("padding")->data<float>();
			auto threshold = dao->property("threshold")->data<float>();

			std::random_device rd;
			std::mt19937 g(rd());

			bool _group = false;
			bool _pcloud = false;
			int ref_size = 0;
			deepsg::SgNode *snode = nullptr;
			ds::PointCloud *pcloud_ref = nullptr;
			std::vector<deepsg::SgNode*> shapes;
			auto any = DfxSpace::getInstance()->findAbsolute(reference);

			if (any != nullptr) {

				if (any->type() == fl::type::DATA_OBJECT) {
					auto dao = any->rawData<fl::DataObject>();

					if (dao->hasProperty("sg_group_node")) {
						snode = any->data<fl::DataObject>().property("sg_group_node")->rawData<DfxGroupNode>();

						if (snode != nullptr) {
							_group = true;
							for (auto child : snode->children()) {
								shapes.push_back(child);	
							}
						}
					}
					else if(dao->hasProperty("sg_point_cloud_node")){
						auto geo = any->data<fl::DataObject>().property("sg_point_cloud_node")->rawData<DfxPointCloudNode>();

						if (geo != nullptr) {
							pcloud_ref = geo->geo();
							if (pcloud_ref != nullptr) {
								_pcloud = true;
							}
						}
					}
				}

			}

			if (_group) ref_size = shapes.size();
			else if (_pcloud) ref_size = pcloud_ref->size();

			if ((pcloud->size() > 0) && (_group || _pcloud) && (ref_size > 0)) {
			
				ds::Channel *cloudPosA = nullptr;
				ds::Channel *shapeChannel = nullptr;
				ds::Channel *cloudPosB = pcloud->channel("@position");
				std::unordered_map<std::string, DfxBoundsMinMax> boundsMap;

				if (_pcloud) {
					cloudPosA = pcloud_ref->channel("@position");
					shapeChannel = pcloud_ref->channel("@instanceShapePath");
				}

				auto hidden = pcloud->channel("@hidden");

				if (hidden == nullptr) {
					pcloud->addAttribute<int>("@hidden");
					hidden = pcloud->channel("@hidden");
					pcloud->fill<int>("@hidden", 0);
				}

				std::vector<glm::vec3> point_seta;
				std::vector<glm::vec3> point_setb;

				if (_group) {
					for (auto shp : shapes) {
						point_seta.push_back(shp->position());
					}
				}
				else if (_pcloud) {
					for (int i = 0; i < pcloud_ref->size(); i++) {
						point_seta.push_back(cloudPosA->at<glm::vec3>()[i]);
					}
				}

				for (int i = 0; i < pcloud->size(); i++) {
					point_setb.push_back(cloudPosB->at<glm::vec3>()[i]);
				}

				unibn::Octree<glm::vec3> octree;
				octree.initialize(point_setb);

				if (_pcloud) {
					if (domain == ProximityDomain::SHAPE_AABB || domain == ProximityDomain::SHAPE_BS) {
						for (int i = 0; i < pcloud_ref->size(); i++) {

							if (shapeChannel != nullptr) {
								auto path = shapeChannel->at<std::string>()[i];

								auto itr = boundsMap.find(path);

								if (itr == boundsMap.end()) {
									auto any = DfxSpace::getInstance()->findAbsolute(path);

									if (any != nullptr) {

										if (any->type() == fl::type::DATA_OBJECT) {
											auto dao = any->rawData<fl::DataObject>();

											if (dao->hasProperty("sg_geo_node")) {
												auto node = dao->property("sg_geo_node")->rawData<deepsg::SgGeoNode>();
												auto aabb = util::boundingBox(node);

												boundsMap[path] = aabb;
											}
										}
									}
								}
							}
						}
					}
				}

				for (unsigned int i = 0; i < point_seta.size(); ++i) {

					if (domain == ProximityDomain::RADIUS) {
						std::vector<uint32_t> results;
						octree.radiusNeighbors<unibn::L2Distance<glm::vec3>>(point_seta[i], radius + padding, results);

						std::shuffle(results.begin(), results.end(), g);

						int count = std::max(1.f, (results.size() * (std::min(threshold, 100.f) / 100.f)));
						for (unsigned int j = 0; j < count; ++j) {
							hidden->at<int>()[results[j]] = 1;
						}
					}
					else if (domain == ProximityDomain::SHAPE_AABB) {

						if (shapeChannel != nullptr) {
							auto path = shapeChannel->at<std::string>()[i];

							auto itr = boundsMap.find(path);

							if (itr != boundsMap.end()) {

								auto xmax = itr->second.xmax + padding;
								auto ymax = itr->second.ymax + padding;
								auto zmax = itr->second.zmax + padding;

								auto xmin = itr->second.xmin - padding;
								auto ymin = itr->second.ymin - padding;
								auto zmin = itr->second.zmin - padding;

								float halfx = (xmax - xmin)  * 0.5f;
								float halfy = (ymax - ymin)  * 0.5f;
								float halfz = (zmax - zmin)  * 0.5f;

								auto mrad = std::max(halfx, std::max(halfy, halfz));

								std::vector<uint32_t> results;
								octree.radiusNeighbors<unibn::L2Distance<glm::vec3>>(point_seta[i], mrad, results);

								std::shuffle(results.begin(), results.end(), g);

								auto pos = cloudPosA->at<glm::vec3>()[i];
								int count = std::max(1.f, (results.size() * (std::min(threshold, 100.f) / 100.f)));
								for (unsigned int j = 0; j < count; ++j) {
									auto pos00 = cloudPosB->at<glm::vec3>()[results[j]];

									auto _xmax = xmax + pos.x;
									auto _ymax = ymax + pos.y;
									auto _zmax = zmax + pos.z;

									auto _xmin = xmin + pos.x;
									auto _ymin = ymin + pos.y;
									auto _zmin = zmin + pos.z;

									if (pos00.x > _xmin &&
										pos00.y > _ymin &&
										pos00.z > _zmin &&
										pos00.x < _xmax &&
										pos00.y < _ymax &&
										pos00.z < _zmax) {

										hidden->at<int>()[results[j]] = 1;
									}

								}
							}
						}

					}
					else if (domain == ProximityDomain::SHAPE_BS) {
						if (shapeChannel != nullptr) {
							auto path = shapeChannel->at<std::string>()[i];

							auto itr = boundsMap.find(path);

							if (itr != boundsMap.end()) {

								auto xmax = itr->second.xmax + padding;
								auto ymax = itr->second.ymax + padding;
								auto zmax = itr->second.zmax + padding;

								auto xmin = itr->second.xmin - padding;
								auto ymin = itr->second.ymin - padding;
								auto zmin = itr->second.zmin - padding;

								float halfx = (xmax - xmin)  * 0.5f;
								float halfy = (ymax - ymin)  * 0.5f;
								float halfz = (zmax - zmin)  * 0.5f;

								auto mrad = std::max(halfx, std::max(halfy, halfz));

								std::vector<uint32_t> results;
								octree.radiusNeighbors<unibn::L2Distance<glm::vec3>>(point_seta[i], mrad, results);

								std::shuffle(results.begin(), results.end(), g);

								int count = std::max(1.f, (results.size() * (std::min(threshold, 100.f) / 100.f)));
								for (unsigned int j = 0; j < count; ++j) {
									hidden->at<int>()[results[j]] = 1;
								}

							}
						}
					}
				}

			}

			parent->emitSignal(deepsg::SgNode::Signal::UPDATED);
		}
	}
}

void DfxPointProximityKillTool::revaluate() {
	if (deepsg::SgNode::parent() != nullptr) {
		DfxPointProximityKillHandler handler;

		if (handler.fetch(referencePath())) {
			auto dao = handler.dataObject()->rawData<fl::DataObject>();

			if (dao->property("accumulate")->data<bool>()) {
				evaluate(deepsg::SgNode::parent());
				DfxUpdater::getInstance()->sceneUpdated();
			}
			else {
				deepsg::SgNode::parent()->evaluateTools();
				DfxUpdater::getInstance()->sceneUpdated();
			}
		}
	}
}

std::vector<std::pair<QWidget*, QString>> DfxPointProximityKillTool::propertyForms() {
	std::pair<QWidget*, QString> grid(form(), "Properties");

	return std::vector<std::pair<QWidget*, QString>> {grid};
}

QWidget *DfxPointProximityKillTool::form() {

	QWidget *widget = new QWidget;

	QWidget *general = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto generalControlPanel = new DfxControlPanel();

	DfxQControlArray *generalControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	// Type Controls
	QPushButton *revaluateButton = new QPushButton("Revaluate");

	revaluateButton->setStyleSheet(revaluateButton->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());

	auto live = new DfxQCheckBoxControl(path + "live", "", false);
	auto accumulate = new DfxQCheckBoxControl(path + "accumulate", "", false);
	auto reference = new DfxQFluentPathControl(path + "reference", "", false);
	auto domain = new DfxQComboBoxControl(path + "domain", "", proximityDomainEnums, false);
	auto radius = new DfxQDecimalSpinnerControl(path + "radius", "", false);
	auto padding = new DfxQDecimalSpinnerControl(path + "padding", "", false);
	auto threshold = new DfxQDecimalSpinnerControl(path + "threshold", "", false);

	// General
	generalControlArray->addControl(revaluateButton);
	generalControlArray->addSpacer(0, 5);
	generalControlArray->addControl("Live Update", live);
	generalControlArray->addSpacer(0, 5);
	generalControlArray->addControl("Accumulate", accumulate);
	generalControlArray->addSpacer(0, 5);
	generalControlArray->addControl("Reference", reference);
	generalControlArray->addSpacer(0, 5);
	generalControlArray->addControl("Search Domain", domain);
	generalControlArray->addSpacer(0, 5);
	generalControlArray->addControl("Radius", radius);
	generalControlArray->addSpacer(0, 5);
	generalControlArray->addControl("Padding", padding);
	generalControlArray->addSpacer(0, 5);
	generalControlArray->addControl("Threshold(%)", threshold);

	generalControlPanel->setTitle("General");
	generalControlPanel->setWidget(generalControlArray);

	QObject::connect(revaluateButton, &QPushButton::clicked, this, &DfxPointProximityKillTool::revaluate);

	// Main layout
	main_layout->addWidget(generalControlPanel);

	widget->setLayout(main_layout);

	return widget;
}