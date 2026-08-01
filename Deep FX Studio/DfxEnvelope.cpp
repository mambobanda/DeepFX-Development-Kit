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

#include "DfxEnvelope.h"


DfxEnvelope::DfxEnvelope(DfxGlobalEntityName uniquename, DfxEntity *entity, size_t propertyID): DfxEntity(uniquename) {
	
	this->entity = entity;
	this->propertyID = propertyID;
	this->type = entity->getPropertyType(propertyID);

	DfxTime::getInstance()->addOnActionEntity(this);

	*(int*)getProperty(_id) = getInstanceID();

	currentFrame = 0;
	size = 0;

	int frameCount = DfxTime::getInstance()->getTotalFrames();
	resizeToFit(frameCount);

	active = false;
	null = false;

	if (this->type == DfxEntityPropertyTypes::iFLOAT_TYPE ||
		this->type == DfxEntityPropertyTypes::iFLOAT_TYPE) {
		null = true;
	}
	else {
		null = false;
	}
}

DfxEnvelope::~DfxEnvelope() {
	DfxTime::getInstance()->removeOnActionEntity(getInstanceID());
}

size_t DfxEnvelope::getType() {
	return type;
}

void DfxEnvelope::resizeToFit(unsigned int sz) {

	if (size != sz) {
		if (size < sz) {

			if (type == DfxEntityPropertyTypes::iFLOAT_TYPE) {
				floatVector.resize(sz + 1);
			}
			else if (type == DfxEntityPropertyTypes::iFLOAT3_TYPE) {
				float3Vector.resize(sz + 1);
			}

			size = sz;
		}
	}
}

size_t DfxEnvelope::keyCount() {
	return size;
}

void DfxEnvelope::setValue(size_t index, DfxEntityPropertyTypes::Float3 f3) {
	if (index >= 0 && index <= float3Vector.size()) {
		float3Vector[index].value = f3;
		float3Vector[index].isSet = true;
	}
}

void DfxEnvelope::setValue(size_t index, float f) {
	if (index >= 0 && index <= floatVector.size()) {
		floatVector[index].value = f;
		floatVector[index].isSet = true;
	}
}

void DfxEnvelope::getValue(size_t index, DfxEntityPropertyTypes::Float3 &f3) {
	if (index >= 0 && index <= float3Vector.size()) {
		f3 = float3Vector[index].value;
	}
}

void DfxEnvelope::getValue(size_t index, float &f) {
	if (index >= 0 && index <= floatVector.size()) {
		f = floatVector[index].value;
	}
}

void DfxEnvelope::clearValue(size_t index) {

	if (type == DfxEntityPropertyTypes::iFLOAT_TYPE) {
		if (index >= 0 && index <= floatVector.size()) {

			floatVector[index].value = 0.0f;
			floatVector[index].isSet = false;
		}
	}
	else if (type == DfxEntityPropertyTypes::iFLOAT3_TYPE) {
		if (index >= 0 && index <= float3Vector.size()) {
			float3Vector[index].value = DfxEntityPropertyTypes::Float3();
			float3Vector[index].isSet = false;

		}
	}
}

std::string DfxEnvelope::getPropertyName() {
	return "";
}

size_t DfxEnvelope::getEntityPropID() {
	return propertyID;
}

size_t DfxEnvelope::getEntityPropType() {
	return entity->getPropertyType(propertyID);
}

DfxEntity *DfxEnvelope::getEntity() {
	return entity;
}

bool DfxEnvelope::isActive() {
	return active;
}

void DfxEnvelope::setActive(bool b) {
	active = b;
}

void DfxEnvelope::compute() {

	if (!isNull()) {
		int frame = DfxTime::getInstance()->getCurrentFrame();
		int frameCount = DfxTime::getInstance()->getTotalFrames();
	
		resizeToFit(frameCount);

		updateProperty(frame);
	}

	/*std::string propertyName = entity->getPropertyName(propertyID);
	DfxGlobal::console::println("Envelope: " + propertyName + " Frame: " + std::to_string(frame));*/
}

void DfxEnvelope::onDataChanged(DfxEntityDataInstance *instance) {

}

void DfxEnvelope::importData(std::vector<std::pair<std::string, std::string>> data) {

}

std::vector<std::pair<std::string, std::string>> DfxEnvelope::exportData() {
	return std::vector<std::pair<std::string, std::string>>();
}

bool DfxEnvelope::isExportable() { return false; }

void DfxEnvelope::initialize() {
	DfxEntityDB::createProperty(_id , "internal_id", new DfxEntityPropertyTypes::Size_TPropertyType());
}

void DfxEnvelope::updateProperty(int frame) {
	using namespace DfxEntityPropertyTypes;

	auto prop = entity->getDataInstance()->getPropertyInstance(propertyID);

	if (type == DfxEntityPropertyTypes::iFLOAT_TYPE) {

		if (floatVector[frame].isSet) {

			*(float*)entity->getProperty(propertyID) = floatVector[frame].value;

			entity->makeDirty();
			prop->dirtyParent();

		}
		else {

			int previous_frame = findPreviousValueFloat(frame);
			int next_frame = findNextValueFloat(frame);

			if (previous_frame != -1) {

				if (next_frame != -1) {
					glm::vec2 a(floatVector[previous_frame].value, 0.f);
					glm::vec2 b(floatVector[next_frame].value, 0.f);

					float range_diff = next_frame - previous_frame;
					float relative_diff = frame - previous_frame;
					float t = relative_diff / range_diff;

					glm::vec2 interpo = glm::lerp(a, b, t);

					*(float*)entity->getProperty(propertyID) = interpo.x;

					entity->makeDirty();
					prop->dirtyParent();
				}
			}
			
		}
	}
	else if (type == DfxEntityPropertyTypes::iFLOAT3_TYPE) {

		if (float3Vector[frame].isSet) {

			*(Float3*)entity->getProperty(propertyID) = float3Vector[frame].value;

			entity->makeDirty();
			prop->dirtyParent();

		}
		else {

			int previous_frame = findPreviousValueFloat3(frame);
			int next_frame = findNextValueFloat3(frame);

			if (previous_frame != -1) {

				if (next_frame != -1) {

					glm::vec3 a(float3Vector[previous_frame].value.x, float3Vector[previous_frame].value.y, float3Vector[previous_frame].value.z);
					glm::vec3 b(float3Vector[next_frame].value.x, float3Vector[next_frame].value.y, float3Vector[next_frame].value.z);

					float range_diff = next_frame - previous_frame;
					float relative_diff = frame - previous_frame;
					float t = relative_diff / range_diff;

					glm::vec3 interpo = glm::lerp(a, b, t);
					Float3 _interpo;
					_interpo.x = interpo.x;
					_interpo.y = interpo.y;
					_interpo.z = interpo.z;

					*(Float3*)entity->getProperty(propertyID) = _interpo;

					entity->makeDirty();
					prop->dirtyParent();
				}
			}

		}
	}
}

int DfxEnvelope::findNextValueFloat3(int current_frame) {
	
	for (int i = current_frame + 1; i < float3Vector.size(); i++) {

		if (float3Vector[i].isSet) return i;
	}

	return -1;
}

int DfxEnvelope::findNextValueFloat(int current_frame) {

	for (int i = current_frame + 1; i < floatVector.size(); i++) {

		if (floatVector[i].isSet) return i;
	}

	return -1;
}

bool DfxEnvelope::isNull() {
	return null;
}

int DfxEnvelope::findPreviousValueFloat3(int current_frame) {


	for (int i = current_frame - 1; i >= 1; i--) {

		if (float3Vector[i].isSet) return i;
	}

	return -1;
}

int DfxEnvelope::findPreviousValueFloat(int current_frame) {

	for (int i = current_frame - 1; i >= 1; i--) {

		if (floatVector[i].isSet) return i;
	}

	return -1;
}

bool DfxEnvelope::hasKey(size_t index) {
	if (type == DfxEntityPropertyTypes::iFLOAT_TYPE) {
		if (index >= 0 && index <= floatVector.size()) {
			return floatVector[index].isSet;
		}
	}
	else if (type == DfxEntityPropertyTypes::iFLOAT3_TYPE) {
		if (index >= 0 && index <= float3Vector.size()) {
			return float3Vector[index].isSet;
		}
	}

	return false;
}

