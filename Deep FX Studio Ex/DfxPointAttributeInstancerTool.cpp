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


#include "DfxUtil.h"
#include "DfxPointAttributeInstancerTool.h"

std::string DfxPointAttributeInstancerTool::_uniqueName = "UID_POINT_ATTRIBUTE_INSTANCER_TOOL";

DfxPointAttributeInstancerTool::DfxPointAttributeInstancerTool() {

	_proxyType = DfxInstancerBase::SHAPE_PROXY;
	_limit = 0;

	_enable = true;
	_dispatcher = nullptr;
	_pointCloud = nullptr;

	position = nullptr;
	positionOverride = nullptr;

	rotationOverride = nullptr;
	scaleOverride = nullptr;

	useTransformMatrixOverride = nullptr;

	hasRotationOverride = nullptr;
	hasTransformOverride = nullptr;

	instanceShapePath = nullptr;
	transformOverride = nullptr;

	_hidden = nullptr;

	useBasePosition = false;

	renderModeEnums.push_back(std::pair<std::string, unsigned int>("AABB", RenderMode::AABB));
	renderModeEnums.push_back(std::pair<std::string, unsigned int>("Shape", RenderMode::SHAPES));

	DfxInstanceManager::getInstance()->addInstancer(this);
}

DfxPointAttributeInstancerTool::~DfxPointAttributeInstancerTool() {
	DfxInstanceManager::getInstance()->removeInstancer(this);
}

std::string DfxPointAttributeInstancerTool::uniqueName() {
	return _uniqueName;
}

std::string DfxPointAttributeInstancerTool::getUniqueName() {
	return _uniqueName;
}

// Tool
bool DfxPointAttributeInstancerTool::isReady() {
	return true;
}

bool DfxPointAttributeInstancerTool::evaluateOnUpdate() {
	return false;
}

bool DfxPointAttributeInstancerTool::evaluateOnNewFrame() {
	return false;
}


bool DfxPointAttributeInstancerTool::evaluateOnValueChanged() {
	return true;
}

void DfxPointAttributeInstancerTool::evaluate(SgNode *parent) {
	if (parent->getType() == deepsg::SgNodeType::POINT_CLOUD) {
		_pointCloud = ((deepsg::SgPointCloudNode*)parent)->geo();
	}
	else {
		_pointCloud = nullptr;
	}
}

void DfxPointAttributeInstancerTool::onSetReferencePath(std::string path) {

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

void DfxPointAttributeInstancerTool::create(SgNode *parent) {}
void DfxPointAttributeInstancerTool::destroy() {}

// Instancing 
void DfxPointAttributeInstancerTool::begin() {
	bool enable = false;
	int proxyType = -1;

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());
	fl::DataObject *dao = nullptr;
	
	if (any != nullptr) {
		dao = any->rawData<fl::DataObject>();
		enable = dao->property("enable")->data<bool>();
		proxyType = dao->property("render_mode")->data<unsigned int>();
		_limit = dao->property("limit")->data<unsigned int>();
	}

	_enable = enable;

	if (_pointCloud != nullptr && enable) {

		std::unordered_map<std::string, std::pair<deepsg::SgNode*, DfxBoundsMinMax>> tempMap;
		_shapeNodes.clear();
		_shapeAABBs.clear();

		if (proxyType == 0) _proxyType = DfxInstancerBase::AABB_PROXY;
		else _proxyType = DfxInstancerBase::SHAPE_PROXY;

		if (_pointCloud->size() > 0) _shapeNodes.resize(_pointCloud->size());
		if (_pointCloud->size() > 0) _shapeAABBs.resize(_pointCloud->size());

		position = _pointCloud->channel("@position");
		if (position != nullptr) if (position->cType() != ds::type::VEC3) position = nullptr;

		positionOverride = _pointCloud->channel("@positionOverride");
		if (positionOverride != nullptr) if (positionOverride->cType() != ds::type::VEC3) positionOverride = nullptr;

		rotationOverride = _pointCloud->channel("@rotationOverride");
		if (rotationOverride != nullptr) if (rotationOverride->cType() != ds::type::VEC3) rotationOverride = nullptr;

		scaleOverride = _pointCloud->channel("@scaleOverride");
		if (scaleOverride != nullptr) if (scaleOverride->cType() != ds::type::VEC3) scaleOverride = nullptr;

		instanceShapePath = _pointCloud->channel("@instanceShapePath");
		if (instanceShapePath != nullptr) if (instanceShapePath->cType() != ds::type::STRING) instanceShapePath = nullptr;

		transformOverride = _pointCloud->channel("@transformOverride");
		if (transformOverride != nullptr) if (transformOverride->cType() != ds::type::MAT4) transformOverride = nullptr;

		useTransformMatrixOverride = _pointCloud->channel("@useTransformMatrixOverride");
		if (useTransformMatrixOverride != nullptr) if (useTransformMatrixOverride->cType() != ds::type::INT) useTransformMatrixOverride = nullptr;

		hasRotationOverride  = _pointCloud->channel("@hasRotationOverride");
		if (hasRotationOverride != nullptr) if (hasRotationOverride->cType() != ds::type::INT) hasRotationOverride = nullptr;

		hasTransformOverride = _pointCloud->channel("@hasTransformOverride");
		if (hasTransformOverride != nullptr) if (hasTransformOverride->cType() != ds::type::INT) hasTransformOverride = nullptr;

		hasScaleOverride = _pointCloud->channel("@hasScaleOverride");
		if (hasScaleOverride != nullptr) if (hasScaleOverride->cType() != ds::type::INT) hasScaleOverride = nullptr;

		_hidden = _pointCloud->channel("@hidden");
		if (_hidden != nullptr) if (_hidden->cType() != ds::type::INT) _hidden = nullptr;

		for (int i = 0; i < _pointCloud->size(); i++) {

			if (instanceHasShape(i)) {
				auto path = instanceShapePath->at<std::string>()[i];

				auto itr = tempMap.find(path);

				if (itr != tempMap.end()) {
					_shapeNodes[i] = itr->second.first;
					_shapeAABBs[i] = itr->second.second;
				}
				else {
					auto any = DfxSpace::getInstance()->findAbsolute(path);

					if (any != nullptr) {

						if (any->type() == fl::type::DATA_OBJECT) {
							auto dao = any->rawData<fl::DataObject>();

							if (dao->hasProperty("sg_geo_node")) {
								auto node = dao->property("sg_geo_node")->rawData<deepsg::SgGeoNode>();
								auto aabb = util::boundingBox(node);
								_shapeNodes[i] = node;
								_shapeAABBs[i] = aabb;
								tempMap[path]  = std::pair<deepsg::SgNode*, DfxBoundsMinMax>(node, aabb);;
							}
						}
					}
				}
			}
			else {
				_shapeNodes[i] = nullptr;
			}
		}

	}
}

void DfxPointAttributeInstancerTool::end() {

	position = nullptr;
	positionOverride = nullptr;

	rotationOverride = nullptr;
	scaleOverride = nullptr;

	useTransformMatrixOverride = nullptr;

	hasRotationOverride = nullptr;
	hasTransformOverride = nullptr;
	hasScaleOverride = nullptr;

	instanceShapePath = nullptr;
	transformOverride = nullptr;

	_shapeNodes.clear();
}

bool DfxPointAttributeInstancerTool::isEnabled() {
	return _enable;
}

size_t DfxPointAttributeInstancerTool::instanceCount() {

	if (_pointCloud != nullptr) {
		return _pointCloud->size();
	}
	
	return 0;
}

unsigned int DfxPointAttributeInstancerTool::limit() {
	return _limit;
}

bool DfxPointAttributeInstancerTool::useTransformMatrix(size_t index) {

	if (_pointCloud != nullptr) {
		
		if (useTransformMatrixOverride != nullptr) {
			bool val = useTransformMatrixOverride->at<int>()[index];
			return val;
		}
	}

	return false;
}

glm::vec3 DfxPointAttributeInstancerTool::instancePosition(size_t index) {

	if (_pointCloud != nullptr) {

		if (positionOverride == nullptr) {
			return position->at<glm::vec3>()[index];
		}
		else{
			return positionOverride->at<glm::vec3>()[index];
		}

	}

	return glm::vec3(0.f);
}

glm::vec3 DfxPointAttributeInstancerTool::instanceRotation(size_t index) {

	if (_pointCloud != nullptr) {

		if (rotationOverride != nullptr) {
			return rotationOverride->at<glm::vec3>()[index];
		}
	}

	return glm::vec3(0.f);
}

glm::vec3 DfxPointAttributeInstancerTool::instanceScale(size_t index) {

	if (_pointCloud != nullptr) {

		if (scaleOverride != nullptr) {
			return scaleOverride->at<glm::vec3>()[index];
		}
	}

	return glm::vec3(1.f);
}

bool DfxPointAttributeInstancerTool::instanceHasRotation(size_t index) {

	if (_pointCloud != nullptr) {

		if (hasRotationOverride != nullptr) {
			return true;
		}
	}

	return false;
}

bool DfxPointAttributeInstancerTool::instanceHasScale(size_t index) {

	if (_pointCloud != nullptr) {

		if (hasScaleOverride != nullptr) {
			return true;
		}
	}

	return false;
}

bool DfxPointAttributeInstancerTool::instanceHasTransform(size_t index) {

	if (_pointCloud != nullptr) {

		if (hasTransformOverride != nullptr) {
			return true;
		}	
	}

	return false;
}

bool DfxPointAttributeInstancerTool::instanceHasShape(size_t index) {

	if (_pointCloud != nullptr) {

		if (instanceShapePath != nullptr) {
			return true;
		}
	}

	return false;
}

glm::mat4 DfxPointAttributeInstancerTool::instanceTransform(size_t index) {

	if (_pointCloud != nullptr) {

		if (transformOverride != nullptr) {
			return transformOverride->at<glm::mat4>()[index];
		}
	}

	return glm::mat4(1.f);
}

glm::mat4 DfxPointAttributeInstancerTool::finalTransform(size_t index) {

	glm::mat4 transMat = glm::translate(glm::mat4(1.f), instancePosition(index));

	transMat *= deepsg::SgTransform::rotationMatrix(instanceRotation(index), deepsg::XYZ);

	transMat *= glm::scale(glm::mat4(1.f), instanceScale(index));

	return transMat;
}

bool DfxPointAttributeInstancerTool::hidden(size_t index) {

	if (_pointCloud != nullptr) {

		if (_hidden != nullptr) {
			return (_hidden->at<int>()[index] == 0) ? false : true;
		}
	}

	return false;
}

unsigned int DfxPointAttributeInstancerTool::instanceProxyType() {
	return _proxyType;
}

deepsg::SgNode *DfxPointAttributeInstancerTool::instanceShape(size_t index) {

	if (_pointCloud != nullptr) {
		return _shapeNodes[index];
	}

	return nullptr;
}


DfxBoundsMinMax DfxPointAttributeInstancerTool::instanceShapeAABB(size_t index) {

	if (_pointCloud != nullptr) {
		if (instanceShapePath != nullptr) {
			return _shapeAABBs[index];
		}
	}

	return DfxBoundsMinMax();
}

std::vector<std::pair<QWidget*, QString>> DfxPointAttributeInstancerTool::propertyForms() {
	std::pair<QWidget*, QString> props(form(), "Properties");
	return std::vector<std::pair<QWidget*, QString>> { props };
}

QWidget *DfxPointAttributeInstancerTool::form() {
	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto generalControlPanel = new DfxControlPanel();

	DfxQControlArray *generalControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	// General Controls
	auto enable = new DfxQCheckBoxControl(path + "enable", "", false);;
	auto render_mode = new DfxQComboBoxControl(path + "render_mode", "", renderModeEnums, false);
	auto limit = new DfxQIntegerSpinnerControl(path + "limit", "", false);;


	// Distribution
	generalControlArray->addControl("Enable", enable);
	generalControlArray->addSpacer(0, 3);
	generalControlArray->addControl("Preview Mode", render_mode);
	generalControlArray->addSpacer(0, 3);
	generalControlArray->addControl("Preview Limit", limit);

	generalControlPanel->setTitle("General");
	generalControlPanel->setWidget(generalControlArray);

	// Main layout
	main_layout->addWidget(generalControlPanel);

	widget->setLayout(main_layout);

	return widget;
}