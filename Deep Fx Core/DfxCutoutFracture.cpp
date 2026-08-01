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

#include "DfxCutoutFracture.h"

namespace dfxcore {

	DfxCutoutFracture::DfxCutoutFracture(DfxGlobalEntityName uniquename) :DfxEntity(uniquename) {
		setLogicalName("Cutout Fracture");

		DfxEntityPropertyTypes::UID *uid01 = (DfxEntityPropertyTypes::UID*)getProperty(outRef);
		uid01->setRootID(getInstanceID());

		setPropertyState(proxies, DfxEntityPropertyState::DATA_STORE_STATE);

		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inImagePath, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inPlaneNormal, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inPlanePosition, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inPosition, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inRotation, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inScale, DfxEntityPropertyState::INPUT_STATE);
	
		setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);

		((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->x = 1.f;
		((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->y = 1.f;
		((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->z = 1.f;

		*(std::string*)getProperty(inImagePath) = "< Path To Cutout Image >";
	}

	DfxCutoutFracture::~DfxCutoutFracture() {

	}

	void DfxCutoutFracture::compute() {
		using namespace DfxEntityPropertyTypes;

		ProxyObjectArray *parray = (ProxyObjectArray*)getProperty(proxies);
		parray->deleteArray();

		DfxGlobal::info::message("Cutout Fracturing Mesh...");

		if (propertyHasBinding(inRef)) {

			dfxutil::traitUIDLinkRelated(this, outRef);

			UID *uid = (UID*)getProperty(inRef);

			BITMAP bitmap;
			bitmap.bitmap = nullptr;

			std::string img_path = *(std::string*)getProperty(inImagePath);
			glm::vec3 pnormal(((Float3*)getProperty(inPlaneNormal))->x, 
				              ((Float3*)getProperty(inPlaneNormal))->y, 
							  ((Float3*)getProperty(inPlaneNormal))->z);

			glm::vec3 pposition(((Float3*)getProperty(inPlanePosition))->x,
								((Float3*)getProperty(inPlanePosition))->y,
								((Float3*)getProperty(inPlanePosition))->z);

			Dfx3dMeshData *in_mesh = dfxutil::findMeshData(uid);
			ProxyObjectArray *pxs = dfxutil::findProxyMeshData(uid);

			if (loadImage(img_path, bitmap)) {

				if (in_mesh != nullptr || pxs != nullptr) {

					std::vector<Dfx3dMeshData *> fragments;

					bool result;

					if (in_mesh != nullptr) {
						fragments = fructureMesh(in_mesh, getAbsoluteTransform(), pnormal, pposition, bitmap.bitmap, bitmap.width, bitmap.height, result);
					}
					else {
						if (!pxs->isNull()) {
							for (int pinx = 0; pinx < pxs->size(); pinx++) {

								ProxyObject *proxPtr = &(*pxs)[pinx];;

								if (proxPtr != nullptr) {

									auto trans = getAbsoluteTransform(proxPtr);
									std::vector<Dfx3dMeshData *> output = fructureMesh(in_mesh, getAbsoluteTransform(), pnormal, pposition, bitmap.bitmap, bitmap.width, bitmap.height, result);

									if (output.size() > 0) {

										std::vector<Dfx3dMeshData *> newVec;
										newVec.reserve(output.size() + fragments.size());

										newVec.insert(newVec.end(), fragments.begin(), fragments.end());
										newVec.insert(newVec.end(), output.begin(), output.end());

										fragments = newVec;
									}
								}
							}
						}
					}

					if (fragments.size() > 0) {
						parray->resize(fragments.size());

						for (size_t i = 0; i < fragments.size(); i++) {
							(*parray)[i].meshData.copy(*fragments[i]);
							(*parray)[i].meshData.calculateVertexNormals();

							glm::vec3 pos = dfxutil::geo::moveMeshToOrigin((*parray)[i].meshData);

							(*parray)[i].position.x = pos.x;
							(*parray)[i].position.y = pos.y;
							(*parray)[i].position.z = pos.z;

							(*parray)[i].scale.x = 1.0f;
							(*parray)[i].scale.y = 1.0f;
							(*parray)[i].scale.z = 1.0f;

							fragments[i]->clearIndexBuffer();
							fragments[i]->clearVertexBuffer();

							delete fragments[i];
						}

					}

				}

				delete[] bitmap.bitmap;
			}
		}

		DfxGlobal::info::message("");

		cleanAllProperties();
		makeChildrenDirty();
		getDataInstance()->makeClean();
	}

	void DfxCutoutFracture::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void DfxCutoutFracture::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> DfxCutoutFracture::exportData() {
		return mapEntityProperties(this);
	}

	bool DfxCutoutFracture::isExportable() {
		return true;
	}

	void *DfxCutoutFracture::creator(DfxGlobalEntityName uniquename) {
		return new DfxCutoutFracture(uniquename);
	}

	void DfxCutoutFracture::initialize() {

		DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(proxies, "@proxy_mesh_data", new DfxEntityPropertyTypes::ProxyObjectArrayPropertyType());
		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inImagePath, "in_img_path", new DfxEntityPropertyTypes::FilePathPropertyType());
		DfxEntityDB::createProperty(inPlaneNormal, "in_plane_normal", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(inPlanePosition, "in_plane_position", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(inPosition, "in_position", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(inRotation, "in_rotation", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(inScale, "in_scale", new DfxEntityPropertyTypes::Float3PropertyType());
		
	}

	glm::mat4 DfxCutoutFracture::getAbsoluteTransform() {

		DfxEntityPropertyTypes::Float3 position = *(DfxEntityPropertyTypes::Float3*)getProperty(inPosition);
		DfxEntityPropertyTypes::Float3 rotation = *(DfxEntityPropertyTypes::Float3*)getProperty(inRotation);
		DfxEntityPropertyTypes::Float3 scale = *(DfxEntityPropertyTypes::Float3*)getProperty(inScale);

		glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z));

		trans *= glm::rotate(glm::mat4(), glm::radians(rotation.x), glm::vec3(1.f, .0f, .0f));
		trans *= glm::rotate(glm::mat4(), glm::radians(rotation.y), glm::vec3(0.f, 1.f, .0f));
		trans *= glm::rotate(glm::mat4(), glm::radians(rotation.z), glm::vec3(0.f, .0f, 1.f));

		trans *= glm::scale(glm::mat4(), glm::vec3(scale.x, scale.y, scale.z));

		return trans;
	}

	glm::mat4 DfxCutoutFracture::getAbsoluteTransform(DfxEntityPropertyTypes::ProxyObject *prox) {

		DfxEntityPropertyTypes::Float3 position = prox->position;
		DfxEntityPropertyTypes::Float3 rotation = prox->rotation;
		DfxEntityPropertyTypes::Float3 scale = prox->scale;

		glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z));

		trans *= glm::rotate(glm::mat4(), glm::radians(rotation.x), glm::vec3(1.f, .0f, .0f));
		trans *= glm::rotate(glm::mat4(), glm::radians(rotation.y), glm::vec3(0.f, 1.f, .0f));
		trans *= glm::rotate(glm::mat4(), glm::radians(rotation.z), glm::vec3(0.f, .0f, 1.f));

		trans *= glm::scale(glm::mat4(), glm::vec3(scale.x, scale.y, scale.z));

		return trans;
	}

	std::vector<Dfx3dMeshData*> DfxCutoutFracture::fructureMesh(Dfx3dMeshData *mesh, glm::mat4 trans, glm::vec3 plane_normal,
		glm::vec3 plane_point, uint8_t* bitmap, int32_t map_width, int32_t map_height, bool &success) {

		std::vector<Dfx3dMeshData *> fragments;

		if (mesh != nullptr) {
			if (mesh->getVertexCount() >= 3 && mesh->getIndexCount() >= 3) {

				dfxblast::CutoutOptions opts;
				opts.bitmap = bitmap;
				opts.plane_normal = plane_normal;
				opts.plane_point = plane_point;
				opts.map_width = map_width;
				opts.map_height = map_height;

				try {
					fragments = dfxblast::DfxNvBlastGlobal::getInstance()->cutoutFracture(mesh, opts, trans);
					success = true;
				}
				catch (DfxNvBlastException const &e) {
					DfxGlobal::console::println(e.what());
					success = false;
				}
			}
		}

		return fragments;
	}

	bool DfxCutoutFracture::loadImage(std::string path, BITMAP &bmp) {
		QImage fimage;

		if (fimage.load(path.c_str())) {

			QImage bmp_conv = fimage.convertToFormat(QImage::QImage::Format_Grayscale8);

			bmp.width = bmp_conv.width();
			bmp.height = bmp_conv.height();

			int byte_count = bmp_conv.width() * bmp_conv.height() * 3;
			bmp.bitmap = new uint8_t[byte_count];

			int byte_counter = 0;

			for (size_t y = 0; y < bmp_conv.height(); y++) {
				for (size_t x = 0; x < bmp_conv.width(); x++) {
					auto rgb = QColor(bmp_conv.pixel(x, y));
					bmp.bitmap[byte_counter] = (uint8_t)rgb.red();
					bmp.bitmap[byte_counter + 1] = (uint8_t)rgb.green();
					bmp.bitmap[byte_counter + 2] = (uint8_t)rgb.blue();
					
					byte_counter += 3;
				}
			}

			return true;
		}
		else {
			DfxGlobal::console::println("Failed to load Cutout Image: " + path);
			return false;
		}
	}
}