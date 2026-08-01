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

#include "DfxSpaceUtil.h"


namespace util {

	void clearAllSceneHandlers() {
		DfxObjectHandler::clearAllObjects();
		DfxLxCameraHandler::clearAllCameras();
		DfxLxPointLightHandler::clearAllPointLights();
		DfxLxEnvironmentLightHandler::clearAllEnvironmentLights();
		DfxLxSunSkyLightHandler::clearAllSunSkyLights();
	}

	bool areBindable(fl::Any *source, fl::Any *destination) {
		auto space = DfxSpace::getInstance();

		auto destLinks = space->findLinks(destination->absolutePath());

		if (destLinks.size() >= destination->cardinalality()) return false;

		if (source->type() != fl::type::ANY && source->type() != destination->type() && destination->type() != fl::type::ANY) return false;

		if (source->role() != fl::role::OUTPUT && source->role() != fl::role::COMPOUND_OUTPUT && source->role() != fl::role::CUSTOM_OUTPUT) return false;

		if (destination->role() != fl::role::INPUT && destination->role() != fl::role::COMPOUND_INPUT && destination->role() != fl::role::CUSTOM_INPUT) return false;

		return true;
	}

	bool areBindable(std::string source, std::string destination) {
		auto space = DfxSpace::getInstance();

		auto any00 = space->findAbsolute(source);
		auto any01 = space->findAbsolute(destination);

		if (any00 != nullptr && any01 != nullptr) {
			return areBindable(any00, any01);
		}

		return false;
	}

	fl::Any *duplicateState(fl::Any *any, bool forceArray, bool includeSpace) {
		auto space = DfxSpace::getInstance();

		fl::Any *newAny = nullptr;

		if (includeSpace) {
			newAny = space->any("_duplicate_", "_duplicate_");
		}
		else {
			newAny = new fl::Any;
			newAny->setName("_duplicate_");
			newAny->setLabel("_duplicate_");
		}

		if (any->type() == fl::type::FLOAT) {
			if (any->isArray() || forceArray) {
				newAny->newData<std::vector<float>>(fl::type::FLOAT);
				newAny->setIsArray(true);
			}
			else {
				newAny->newData<float>(fl::type::FLOAT);
				newAny->setData<float>(0.f);
			}
		}
		else if (any->type() == fl::type::FLOAT_3) {
			if (any->isArray() || forceArray) {
				newAny->newData<std::vector<glm::vec3>>(fl::type::FLOAT_3);
				newAny->setIsArray(true);
			}
			else {
				newAny->newData<glm::vec3>(fl::type::FLOAT_3);
				newAny->setData<glm::vec3>(glm::vec3(0.f));
			}
		}
		else if (any->type() == fl::type::SIZE_T) {
			if (any->isArray() || forceArray) {
				newAny->newData<std::vector<size_t>>(fl::type::SIZE_T);
				newAny->setIsArray(true);
			}
			else {
				newAny->newData<size_t>(fl::type::SIZE_T);
				newAny->setData<size_t>(0);
			}
		}
		else if (any->type() == fl::type::STD_STRING) {
			if (any->isArray() || forceArray) {
				newAny->newData<std::vector<std::string>>(fl::type::STD_STRING);
				newAny->setIsArray(true);
			}
			else {
				newAny->newData<std::string>(fl::type::STD_STRING);
				newAny->setData<std::string>("");
			}
		}
		else if (any->type() == fl::type::BOOL) {
			if (any->isArray() || forceArray) {
				newAny->newData<std::vector<bool>>(fl::type::BOOL);
				newAny->setIsArray(true);
			}
			else {
				newAny->newData<bool>(fl::type::BOOL);
				newAny->setData<bool>(false);
			}
		}
		else {
			newAny->setType(fl::type::NONE);
		}

		return newAny;
	}

	void pushArray(fl::Any *anyArray, std::vector<fl::Any *> values) {
		auto space = DfxSpace::getInstance();

		if (anyArray->type() == fl::type::FLOAT) {
			for (auto any : values) {
				anyArray->rawData<std::vector<float>>()->push_back(any->data<float>());
			}
		}
		else if (anyArray->type() == fl::type::FLOAT_3) {
			for (auto any : values) {
				anyArray->rawData<std::vector<glm::vec3>>()->push_back(any->data<glm::vec3>());
			}
		}
		else if (anyArray->type() == fl::type::SIZE_T) {
			for (auto any : values) {
				anyArray->rawData<std::vector<size_t>>()->push_back(any->data<size_t>());
			}
		}
		else if (anyArray->type() == fl::type::STD_STRING) {
			for (auto any : values) {
				anyArray->rawData<std::vector<std::string>>()->push_back(any->data<std::string>());
			}
		}
		else if (anyArray->type() == fl::type::BOOL) {
			for (auto any : values) {
				anyArray->rawData<std::vector<bool>>()->push_back(any->data<bool>());
			}
		}
		
	}

}