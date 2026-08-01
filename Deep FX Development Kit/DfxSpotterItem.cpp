/*
* The Mailo Today Solutions Software License
*
* Copyright 2016 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#include "DfxSpotterItem.h"


DfxSpotterItem::DfxSpotterItem(DfxGlobalEntityName uniquename) : DfxViewItem(DFX_CUSTOM_ITEM, uniquename) {
	using namespace DfxEntityPropertyTypes;

	setLogicalName("Spotter");

	((Float3*)getProperty(outPosition))->x = 0.f;
	((Float3*)getProperty(outPosition))->y = 0.f;
	((Float3*)getProperty(outPosition))->z = 0.f;

	setPropertyState(outPosition, DfxEntityPropertyState::OUTPUT_STATE);

	glInitialized = false;

	getDataInstance()->makeClean();

	size = .5f;
}

DfxSpotterItem::~DfxSpotterItem() {

}

void DfxSpotterItem::draw() {
	initializeOpenGLFunctions();

	using namespace DfxEntityPropertyTypes;

	glm::vec3 xUp(size, 0.0, 0.0), xDown(-(size), 0.0, 0.0);
	glm::vec3 yUp(0.0, size, 0.0), yDown(0.0, -(size), 0.0);
	glm::vec3 zUp(0.0, 0.0, size), zDown(0.0, 0.0, -(size));

	glm::vec3 _pos = getPosition();

	xUp   = _pos + xUp;
	xDown = _pos + xDown;

	yUp   = _pos + yUp;
	yDown = _pos + yDown;

	zUp   = _pos + zUp;
	zDown = _pos + zDown;

	double _xu[3], _xd[3], _yu[3], _yd[3], _zu[3], _zd[3];

	_xu[0] = xUp.x;
	_xu[1] = xUp.y;
	_xu[2] = xUp.z;

	_xd[0] = xDown.x;
	_xd[1] = xDown.y;
	_xd[2] = xDown.z;

	_yu[0] = yUp.x;
	_yu[1] = yUp.y;
	_yu[2] = yUp.z;

	_yd[0] = yDown.x;
	_yd[1] = yDown.y;
	_yd[2] = yDown.z;

	_zu[0] = zUp.x;
	_zu[1] = zUp.y;
	_zu[2] = zUp.z;

	_zd[0] = zDown.x;
	_zd[1] = zDown.y;
	_zd[2] = zDown.z;

	
	glDisable(GL_LIGHTING);
	glLineWidth(3.f);
	glColor3f(1.f, .0f, 1.f);
	
	glBegin(GL_LINES);

	glVertex3f(_xd[0], _xd[1], _xd[2]);
	glVertex3f(_xu[0], _xu[1], _xu[2]);

	glVertex3f(_yd[0], _yd[1], _yd[2]);
	glVertex3f(_yu[0], _yu[1], _yu[2]);

	glVertex3f(_zd[0], _zd[1], _zd[2]);
	glVertex3f(_zu[0], _zu[1], _zu[2]);

	glEnd();

	glLineWidth(1.0f);
	
	glEnable(GL_LIGHTING);
	
}

void DfxSpotterItem::drawSelection() {
	using namespace DfxEntityPropertyTypes;

	glm::vec3 xUp(size, 0.0, 0.0), xDown(-(size), 0.0, 0.0);
	glm::vec3 yUp(0.0, size, 0.0), yDown(0.0, -(size), 0.0);
	glm::vec3 zUp(0.0, 0.0, size), zDown(0.0, 0.0, -(size));

	glm::vec3 _pos = getPosition();

	xUp = _pos + xUp;
	xDown = _pos + xDown;

	yUp = _pos + yUp;
	yDown = _pos + yDown;

	zUp = _pos + zUp;
	zDown = _pos + zDown;

	double _xu[3], _xd[3], _yu[3], _yd[3], _zu[3], _zd[3];

	_xu[0] = xUp.x;
	_xu[1] = xUp.y;
	_xu[2] = xUp.z;

	_xd[0] = xDown.x;
	_xd[1] = xDown.y;
	_xd[2] = xDown.z;

	_yu[0] = yUp.x;
	_yu[1] = yUp.y;
	_yu[2] = yUp.z;

	_yd[0] = yDown.x;
	_yd[1] = yDown.y;
	_yd[2] = yDown.z;

	_zu[0] = zUp.x;
	_zu[1] = zUp.y;
	_zu[2] = zUp.z;

	_zd[0] = zDown.x;
	_zd[1] = zDown.y;
	_zd[2] = zDown.z;


	glDisable(GL_LIGHTING);
	glDisable(GL_DITHER);
	glShadeModel(GL_FLAT);

	glLineWidth(10.f);
	glColor3f(colorID.redf(), colorID.greenf(), colorID.bluef());

	glBegin(GL_LINES);

	glVertex3f(_xd[0], _xd[1], _xd[2]);
	glVertex3f(_xu[0], _xu[1], _xu[2]);

	glVertex3f(_yd[0], _yd[1], _yd[2]);
	glVertex3f(_yu[0], _yu[1], _yu[2]);

	glVertex3f(_zd[0], _zd[1], _zd[2]);
	glVertex3f(_zu[0], _zu[1], _zu[2]);

	glEnd();

	glLineWidth(1.0f);

	glEnable(GL_LIGHTING);
	glEnable(GL_DITHER);
	glShadeModel(GL_FLAT);
}

DfxBounds DfxSpotterItem::bounds() {

	glm::vec3 xUp(size, 0.0, 0.0), xDown(-(size), 0.0, 0.0);
	glm::vec3 yUp(0.0, size, 0.0), yDown(0.0, -(size), 0.0);
	glm::vec3 zUp(0.0, 0.0, size), zDown(0.0, 0.0, -(size));

	glm::vec3 _pos = getPosition();

	xUp = _pos + xUp;
	xDown = _pos + xDown;

	yUp = _pos + yUp;
	yDown = _pos + yDown;

	zUp = _pos + zUp;
	zDown = _pos + zDown;

	double _xu[3], _xd[3], _yu[3], _yd[3], _zu[3], _zd[3];

	_xu[0] = xUp.x;
	_xu[1] = xUp.y;
	_xu[2] = xUp.z;

	_xd[0] = xDown.x;
	_xd[1] = xDown.y;
	_xd[2] = xDown.z;

	_yu[0] = yUp.x;
	_yu[1] = yUp.y;
	_yu[2] = yUp.z;

	_yd[0] = yDown.x;
	_yd[1] = yDown.y;
	_yd[2] = yDown.z;

	_zu[0] = zUp.x;
	_zu[1] = zUp.y;
	_zu[2] = zUp.z;

	_zd[0] = zDown.x;
	_zd[1] = zDown.y;
	_zd[2] = zDown.z;

	DfxBounds bounds;
	bounds.empty();
	bounds.add(_xu[0], _xu[1], _xu[2]);
	bounds.add(_xd[0], _xd[1], _xd[2]);
	bounds.add(_yu[0], _yu[1], _yu[2]);
	bounds.add(_yd[0], _yd[1], _yd[2]);
	bounds.add(_zu[0], _zu[1], _zu[2]);
	bounds.add(_zd[0], _zd[1], _zd[2]);
	
	return bounds;
}

bool DfxSpotterItem::selectable() {
	return true;
}

void DfxSpotterItem::compute() {
	using namespace DfxEntityPropertyTypes;

	((Float3*)getProperty(outPosition))->x = getPosition().x;
	((Float3*)getProperty(outPosition))->y = getPosition().y;
	((Float3*)getProperty(outPosition))->z = getPosition().z;

	makePropertyBindingDirty(outPosition);
	makeChildrenDirty();
	getDataInstance()->makeClean();
}

void DfxSpotterItem::calculateBounds() {
	// Do nothing
}

bool DfxSpotterItem::isExportable() {
	return true;
}

void *DfxSpotterItem::creator(DfxGlobalEntityName uniquename) {
	return new DfxSpotterItem(uniquename);
}

void DfxSpotterItem::importData(std::vector<std::pair<std::string, std::string>> data) {

}

std::vector<std::pair<std::string, std::string>> DfxSpotterItem::exportData() {
	return mapEntityProperties(this);
}

void DfxSpotterItem::load(std::ifstream &) {
	using namespace DfxEntityPropertyTypes;

	((Float3*)getProperty(outPosition))->x = getPosition().x;
	((Float3*)getProperty(outPosition))->y = getPosition().y;
	((Float3*)getProperty(outPosition))->z = getPosition().z;
}

void DfxSpotterItem::initialize() {

	DfxViewItem::initialize();

	DfxEntityDB::createProperty(outPosition, "position", new DfxEntityPropertyTypes::Float3PropertyType());
	
}