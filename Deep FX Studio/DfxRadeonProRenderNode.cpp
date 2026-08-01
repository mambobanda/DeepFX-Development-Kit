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

#include "CompilationGlobals.h"

#if ENABLE_PROTOTYPE_FEATURES

#include <Math/mathutils.h>
#include <DfxViewItem.h>
#include <DfxCamera.h>
#include <DfxLight.h>
#include <experimental/filesystem>
#include "DfxRenderRegistry.h"
#include "DfxRadeonProRenderNode .h"

namespace filesystem = std::experimental::filesystem;

namespace radeon {

	rpr_int DfxRadeonProRenderNode::tahoePluginID = 0;
	size_t  DfxRadeonProRenderNode::pluginCount = 0;
	rpr_int DfxRadeonProRenderNode::plugins[1];
	bool	DfxRadeonProRenderNode::pluginFailedToLoad = false;
	bool	DfxRadeonProRenderNode::tahoePluginLoaded = false;

	DfxRadeonProRenderNode::DfxRadeonProRenderNode(DfxGlobalEntityName uniquename) : DfxRenderInterface(uniquename) {
		setLogicalName("Radeon Pro Render");

		setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);
		setPropertyState(inDevice, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inRenderType, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inCycles, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inCycleUpdate, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inRayDepth, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inDiffuseDepth, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inRefractionDepth, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inGlossyDepth, DfxEntityPropertyState::INPUT_STATE);

		((DfxEnum*)getProperty(inDevice))->add("CPU");
		((DfxEnum*)getProperty(inDevice))->add("GPU");
		//((DfxEnum*)getProperty(inDevice))->add("CPU + GPU");

		((DfxEnum*)getProperty(inRenderType))->add("Global Illumination");
		((DfxEnum*)getProperty(inRenderType))->add("Direct Illumination");
		((DfxEnum*)getProperty(inRenderType))->add("Direct Illumination, No Shadows");
		((DfxEnum*)getProperty(inRenderType))->add("Wireframe");
		((DfxEnum*)getProperty(inRenderType))->add("Ambient Occlusion");

		defaultSettings();

		framebuffer = nullptr;
		framebuffer01 = nullptr;
		context = nullptr;
		matsys = nullptr;
		matsys_rprx = nullptr;
		scene = nullptr;
		defaultMaterial = nullptr;
		defaultMaterial_rprx = nullptr;
		final_image = nullptr;
		final_image_size = 0;
		failedToCreateContext = false;

		setWidth(800);
		setHeight(800);

		DfxRenderRegistry::getIntstance()->addProp(getInstanceID(), this, DfxRenderRegistry::RENDERER);
	}

	DfxRadeonProRenderNode::~DfxRadeonProRenderNode() {
		DfxRenderRegistry::getIntstance()->removeProp(getInstanceID());
	}

	void DfxRadeonProRenderNode::compute() {
	}

	void DfxRadeonProRenderNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void DfxRadeonProRenderNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> DfxRadeonProRenderNode::exportData() {
		return mapEntityProperties(this);
	}

	bool DfxRadeonProRenderNode::isExportable() {
		return true;
	}

	void *DfxRadeonProRenderNode::creator(DfxGlobalEntityName uniquename) {
		return new DfxRadeonProRenderNode(uniquename);
	}

	void DfxRadeonProRenderNode::initialize() {
		DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inDevice, "in_device", new DfxEntityPropertyTypes::DfxEnumPropertyType());
		DfxEntityDB::createProperty(inRenderType, "in_render_type", new DfxEntityPropertyTypes::DfxEnumPropertyType());
		DfxEntityDB::createProperty(inCycles, "in_cycles", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inCycleUpdate, "in_cycle_update", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inRayDepth, "in_ray_depth", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inGlossyDepth, "in_glossy_depth", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inDiffuseDepth, "in_diffuse_depth", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inRefractionDepth, "in_refraction_depth", new DfxEntityPropertyTypes::FloatPropertyType());
	}

	void DfxRadeonProRenderNode::initPlugin() {

		if (!pluginFailedToLoad && !tahoePluginLoaded) {
			rpr_int tahoePluginID = rprRegisterPlugin("Tahoe64.dll");

			if (tahoePluginID != -1) {
				plugins[0] = tahoePluginID;
				pluginCount = sizeof(plugins) / sizeof(plugins[0]);

				tahoePluginLoaded = true;
			}
			else {
				pluginFailedToLoad = true;
				DfxGlobal::console::println("Failed to load RPR Plugin.");
			}
		}
	}

	void DfxRadeonProRenderNode::brandNewScene(std::vector<size_t> nodes) {

		for (auto uid : nodes) {
			auto dinstance = DfxEntityDB::getDataInstance(uid);

			if (dinstance != nullptr) {
				auto entity = dinstance->getDfxEntity();

				if (entity->UniqueName().compare("2F2X252X_DFX_CAMERA") == 0) {

					DfxCamera *camera = (DfxCamera*)entity;

					rprCamera rcamera;
					uint32_t c_width = (uint32_t)*(float*)camera->getProperty(DfxCamera::inWidth);
					uint32_t c_height = (uint32_t)*(float*)camera->getProperty(DfxCamera::inHeight);
					float c_fov_w = *(float*)camera->getProperty(DfxCamera::inHoriziontalFOV);
					float c_fov_h = *(float*)camera->getProperty(DfxCamera::inVerticalFOV);
					float c_flength = *(float*)camera->getProperty(DfxCamera::inFocalLength);

					minorErrorCode = rprContextCreateCamera(context, &rcamera.camera);

					if (minorErrorCode != RPR_SUCCESS) {
						DfxGlobal::console::println("Failed to create RPR Camera.");
					}
					else {
						cameras.push_back(rcamera);

						glm::mat4 mat = camera->prefferedTransform();

						minorErrorCode = rprCameraSetTransform(rcamera.camera, RPR_FALSE, &mat[0][0]);
						//minorErrorCode = rprCameraLookAt(rcamera.camera, 0, 5, 20, 0, 1, 0, 0, 1, 0);

						setWidth(c_width);
						setHeight(c_height);

						if (minorErrorCode != RPR_SUCCESS) {
							DfxGlobal::console::println("Failed to set RPR Camera transform.");
						}

						minorErrorCode = rprCameraSetFocalLength(rcamera.camera, c_flength);

						if (minorErrorCode != RPR_SUCCESS) {
							DfxGlobal::console::println("Failed to set RPR Camera focal length.");
						}

						minorErrorCode = rprCameraSetMode(rcamera.camera, RPR_CAMERA_MODE_PERSPECTIVE);

						if (minorErrorCode != RPR_SUCCESS) {
							DfxGlobal::console::println("Failed to set RPR perspective mode.");
						}

						minorErrorCode = rprCameraSetSensorSize(rcamera.camera, c_fov_w, c_fov_h);

						if (minorErrorCode != RPR_SUCCESS) {
							DfxGlobal::console::println("Failed to set RPR sensor size.");
						}

						minorErrorCode = rprSceneSetCamera(scene, rcamera.camera);

						if (minorErrorCode != RPR_SUCCESS) {
							DfxGlobal::console::println("Failed to attach RPR Camera tp scene.");
						}
					}
				}
				else if (entity->UniqueName().compare("2F2X252X_DFX_LIGHT") == 0) {
					DfxLight *light = (DfxLight*)entity;

					rprLight rlight;

					minorErrorCode = rprContextCreatePointLight(context, &rlight.light);


					if (minorErrorCode != RPR_SUCCESS) {
						DfxGlobal::console::println("Failed to create RPR Light.");
					}
					else {
						lights.push_back(rlight);

						glm::mat4 mat = light->transformationMatrix();
						RadeonProRender::matrix m = RadeonProRender::translation(RadeonProRender::float3(0, 2, 0));

						minorErrorCode = rprLightSetTransform(rlight.light, RPR_FALSE, &mat[0][0]);


						if (minorErrorCode != RPR_SUCCESS) {
							DfxGlobal::console::println("Failed to set RPR Light transform.");
						}

						minorErrorCode = rprPointLightSetRadiantPower3f(rlight.light, 1.5f, 1.5f, 1.5f);

						if (minorErrorCode != RPR_SUCCESS) {
							DfxGlobal::console::println("Failed to set RPR Light strength.");
						}

						minorErrorCode = rprSceneAttachLight(scene, rlight.light);

						if (minorErrorCode != RPR_SUCCESS) {
							DfxGlobal::console::println("Failed to attach RPR Light to scene.");
						}
					}
				}
				else {
					DfxViewItem *vitem = (DfxViewItem*)entity;

					if (entity->hasDataStoreProperty("@mesh_data")) {
						size_t _uid_ = entity->dataStorePropertyID("@mesh_data");

						Dfx3dMeshData *mdata = (Dfx3dMeshData*)vitem->getProperty(_uid_);

						rprMesh rmesh;
						rmesh.mesh = createShape(mdata);

						if (rmesh.mesh != nullptr) {
							shapes.push_back(rmesh);

							rmesh.nodeID = entity->getInstanceID();

							applyDefualtMatToObj(rmesh.mesh);

							glm::mat4 mat = vitem->transformationMatrix();

							minorErrorCode = rprSceneAttachShape(scene, rmesh.mesh);

							if (minorErrorCode != RPR_SUCCESS) {
								DfxGlobal::console::println("Failed to attach RPR Shape to scene.");
							}

							rprShapeSetTransform(rmesh.mesh, RPR_FALSE, &mat[0][0]);

							if (minorErrorCode != RPR_SUCCESS) {
								DfxGlobal::console::println("Failed to set RPR Shape tranpose.");
							}
						}
					}
				}

			}
		}

		// New Frame Buffer & Set Render Settings
		{
			newRPRFrameBuffer();

			int rMode = ((DfxEnum*)getProperty(inRenderType))->getCurrentIndex();
			int rRayDepth = *(float*)getProperty(inRayDepth);
			int rGlossyDepth = *(float*)getProperty(inGlossyDepth);
			int rDiffuseDepth = *(float*)getProperty(inDiffuseDepth);
			int rRefractionDepth = *(float*)getProperty(inRefractionDepth);

			setRenderMode(rMode);
			setRayDepth(rRayDepth);
			setGlossyDepth(rGlossyDepth);
			setDiffuseDepth(rDiffuseDepth);
			setRefractionDepth(rRefractionDepth);

			minorErrorCode = rprContextSetScene(context, scene);

			if (minorErrorCode != RPR_SUCCESS) {
				DfxGlobal::console::println("RPR failed to set scene context, aborting.");
				return;
			}
		}
	}

	void DfxRadeonProRenderNode::updateScene(std::vector<size_t> nodes) {

	}

	void DfxRadeonProRenderNode::newFrame(std::vector<size_t> nodes) {
		newRPRContext();
		brandNewScene(nodes);
	}

	void DfxRadeonProRenderNode::render() {	
	
		minorErrorCode = rprContextRender(context);

		if (minorErrorCode != RPR_SUCCESS) {
			DfxGlobal::console::println("RPR failed to render.");
		}
		
		/*extractFinalImage();
		if (minorErrorCode == RPR_SUCCESS) {*/

		if (!filesystem::is_directory("radeon_tmp") || !filesystem::exists("radeon_tmp")) { // Check if src folder exists
			filesystem::create_directory("radeon_tmp"); // create src folder
		}

		minorErrorCode = rprFrameBufferSaveToFile(framebuffer, "radeon_tmp\\renderstep12345.png");

		if (minorErrorCode != RPR_SUCCESS) {
			DfxGlobal::console::println("RPR failed to save render.");
		}
		else {
			q_final_image.load("radeon_tmp\\renderstep12345.png");
		}
		//}
	}

	unsigned char *DfxRadeonProRenderNode::getFinalImageBuffer() {
		return final_image;
	}

	unsigned int DfxRadeonProRenderNode::getFinalImageBufferSize() {
		return final_image_size;
	}

	void DfxRadeonProRenderNode::cleanUp() {

		if (defaultMaterial != nullptr) {

			rprObjectDelete(defaultMaterial);
			defaultMaterial = nullptr;

			if (minorErrorCode != RPR_SUCCESS) {
				DfxGlobal::console::println("RPR failed to destroy Material.");
			}
		}

		for (auto cam : cameras) {
			minorErrorCode = rprObjectDelete(cam.camera);

			if (minorErrorCode != RPR_SUCCESS) {
				DfxGlobal::console::println("RPR failed to destroy Camera.");
			}
		}

		for (auto light : lights) {
			minorErrorCode = rprObjectDelete(light.light);

			if (minorErrorCode != RPR_SUCCESS) {
				DfxGlobal::console::println("RPR failed to destroy Light.");
			}
		}

		for (auto shape : shapes) {
			minorErrorCode = rprObjectDelete(shape.mesh);

			if (minorErrorCode != RPR_SUCCESS) {
				DfxGlobal::console::println("RPR failed to destroy Mesh.");
			}
		}

		if (matsys != nullptr) {
			minorErrorCode = rprObjectDelete(matsys);
			matsys = nullptr;

			if (minorErrorCode != RPR_SUCCESS) {
				DfxGlobal::console::println("RPR failed to destroy Material System.");
			}
		}

		if (framebuffer != nullptr) {
			minorErrorCode = rprObjectDelete(framebuffer);
			framebuffer = nullptr;

			if (minorErrorCode != RPR_SUCCESS) {
				DfxGlobal::console::println("RPR failed to destroy Framebuffer.");
			}
		}

		if (scene != nullptr) {
			minorErrorCode = rprObjectDelete(scene);
			scene = nullptr;

			if (minorErrorCode != RPR_SUCCESS) {
				DfxGlobal::console::println("RPR failed to destroy Scene.");
			}
		}

		if (context != nullptr) {
			minorErrorCode = rprObjectDelete(context);
			context = nullptr;

			if (minorErrorCode != RPR_SUCCESS) {
				DfxGlobal::console::println("RPR failed to destroy context.");
			}
		}

		failedToCreateContext = false;

		cameras.clear();
		lights.clear();
		shapes.clear();
	}

	void DfxRadeonProRenderNode::newRPRContext() {
		initPlugin();

		int rc = ((DfxEnum*)getProperty(inDevice))->getCurrentIndex();
		
		uint device_flag = RPR_CREATION_FLAGS_ENABLE_CPU;
		if (rc == 1) {
			device_flag = RPR_CREATION_FLAGS_ENABLE_GPU0;
		}
		
		if (context == nullptr) {
			errorCode = rprCreateContext(RPR_API_VERSION, plugins, pluginCount, device_flag, NULL, NULL, &context);
			if (errorCode != RPR_SUCCESS) {
				failedToCreateContext = true;
				DfxGlobal::console::println("Failed to Create Context.");
				if (error()) return;
			}
		}

		if (!failedToCreateContext) {
			errorCode = rprContextSetActivePlugin(context, plugins[0]);

			if (error()) return;
		}

		if (!failedToCreateContext && matsys == nullptr) {
			errorCode = rprContextCreateMaterialSystem(context, 0, &matsys);

			if (error()) return;

			errorCode = rprxCreateContext(matsys, 0, &matsys_rprx);

			if (error()) return;
		}

		if (!failedToCreateContext && scene == nullptr) {
			errorCode = rprContextCreateScene(context, &scene);

			if (error()) return;
		}

		setupDefualtMat();
	}

	void DfxRadeonProRenderNode::newRPRFrameBuffer() {

		rpr_framebuffer_desc desc = { getWidth(), getHeight() };
		rpr_framebuffer_format fmt = { 4, RPR_COMPONENT_TYPE_FLOAT32 };
		minorErrorCode = rprContextCreateFrameBuffer(context, fmt, &desc, &framebuffer);
		//minorErrorCode = rprContextCreateFrameBuffer(context, fmt, &desc, &framebuffer01);

		if (errorCode != RPR_SUCCESS) {
			DfxGlobal::console::println("Failed to create RPR frame buffer.");
		}
		else {

			minorErrorCode = rprFrameBufferClear(framebuffer);

			if (errorCode != RPR_SUCCESS) {
				DfxGlobal::console::println("Failed to clear RPR frame buffer.");
			}


			//minorErrorCode = rprContextSetAOV(context, RPR_AOV_DIRECT_DIFFUSE, framebuffer);
			//minorErrorCode = rprContextSetAOV(context, RPR_AOV_INDIRECT_DIFFUSE, framebuffer);
			minorErrorCode = rprContextSetAOV(context, RPR_AOV_COLOR, framebuffer);
			//minorErrorCode = rprContextSetAOV(context, RPR_AOV_DIRECT_DIFFUSE, framebuffer01);

			if (errorCode != RPR_SUCCESS) {
				DfxGlobal::console::println("Failed to set RPR color output.");
			}
		}
	}

	bool DfxRadeonProRenderNode::error() {

		if (errorCode != RPR_SUCCESS) {
			printErrorCode();
			return true;
		}
		else {
			return false;
		}
	}

	void DfxRadeonProRenderNode::printErrorCode() {

		if (errorCode != RPR_SUCCESS && context) {
			rpr_int status = 0;
			size_t sizeParamA = 0;
			status = rprContextGetInfo(context, RPR_CONTEXT_LAST_ERROR_MESSAGE, 0, 0, &sizeParamA);
			if (status == RPR_SUCCESS && sizeParamA >= 1)
			{
				char* paramData = new char[sizeParamA];
				status = rprContextGetInfo(context, RPR_CONTEXT_LAST_ERROR_MESSAGE, sizeParamA, paramData, 0);
				if (status == RPR_SUCCESS) {
					DfxGlobal::console::println(std::string("ErrorMessage = ") + paramData);
				}
				delete[] paramData; paramData = 0;
			}
		}
	}

	rpr_shape DfxRadeonProRenderNode::createShape(Dfx3dMeshData *data) {

		rpr_shape newShape = nullptr;

		if (data != nullptr) {

			if (data->getVertexCount() >= 3) {

				rpr_float *verts = new rpr_float[data->getVertexCount() * 3];
				rpr_float *norms = new rpr_float[data->getVertexCount() * 3];
				rpr_float *uvs = new rpr_float[data->getVertexCount() * 3];
				rpr_int *indices = new rpr_int[data->getIndexCount()];
				rpr_int *num_face_verts = new rpr_int[data->getIndexCount() / 3];

				size_t vCounter = 0;
				for (size_t inx = 0; inx < data->getVertexCount(); inx++) {

					verts[vCounter] = data->getVertex(inx).x;
					verts[vCounter + 1] = data->getVertex(inx).y;
					verts[vCounter + 2] = data->getVertex(inx).z;

					glm::vec3 inv_norm = -1.f * data->getNormal(inx);

					norms[vCounter] = inv_norm.x;
					norms[vCounter + 1] = inv_norm.y;
					norms[vCounter + 2] = inv_norm.z;

					uvs[vCounter] = data->getUV(inx).x;
					uvs[vCounter + 1] = data->getUV(inx).y;
					uvs[vCounter + 2] = 0.0f;

					vCounter += 3;
				}

				for (size_t inx = 0; inx < data->getIndexCount(); inx++) {
					indices[inx] = data->getIndex(inx);
				}

				for (size_t inx = 0; inx < data->getIndexCount() / 3; inx++) {
					num_face_verts[inx] = 3;
				}

				minorErrorCode = rprContextCreateMesh(context,
					verts, data->getVertexCount(), sizeof(float) * 3,
					norms, data->getVertexCount(), sizeof(float) * 3,
					uvs, data->getVertexCount(), sizeof(float) * 3,
					indices, sizeof(rpr_int),
					indices, sizeof(rpr_int),
					indices, sizeof(rpr_int),
					num_face_verts, data->getIndexCount() / 3,
					&newShape);

				if (minorErrorCode != RPR_SUCCESS) {
					newShape = nullptr;
					DfxGlobal::console::println("Failed to create RPR Shape");
				}

				delete[] verts;
				delete[] norms;
				delete[] uvs;
				delete[] indices;
				delete[] num_face_verts;
			}
			else {
				DfxGlobal::console::println("Failed to create RPR Mesh: No triangles.");
			}
		}
		else {
			DfxGlobal::console::println("Failed to create RPR Mesh: Null mesh data.");
		}

		return newShape;
	}

	void DfxRadeonProRenderNode::setupDefualtMat() {

		minorErrorCode = rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &defaultMaterial);

		if (minorErrorCode != RPR_SUCCESS) {
			defaultMaterial = nullptr;
			DfxGlobal::console::println("Failed to create RPR Default Material");
		}
		else {

			minorErrorCode = rprMaterialNodeSetInputF(defaultMaterial, "color", 1.5f, 1.5f, 1.5f, 1.0f);

			if (minorErrorCode != RPR_SUCCESS) {
				DfxGlobal::console::println("Failed to set RPR defualt material diffuse color");
			}
		}

		minorErrorCode = rprxCreateMaterial(matsys_rprx, RPRX_MATERIAL_UBER, &defaultMaterial_rprx);

		if (minorErrorCode != RPR_SUCCESS) {
			defaultMaterial_rprx = nullptr;
			DfxGlobal::console::println("Failed to create RPR Default UBER Material");
		}
		else {

			minorErrorCode = rprxMaterialSetParameterF(matsys_rprx, defaultMaterial_rprx, RPRX_UBER_MATERIAL_DIFFUSE_COLOR, 255, 0.0f, 0.0f, 1.f);

			if (minorErrorCode != RPR_SUCCESS) {
				defaultMaterial_rprx = nullptr;
				DfxGlobal::console::println("Failed to set RPR Default UBER Material Diffuse Color");
			}

			minorErrorCode = rprxMaterialSetParameterF(matsys_rprx, defaultMaterial_rprx, RPRX_UBER_MATERIAL_DIFFUSE_WEIGHT, 1.f, 1.f, 1.f, 1.f);

			if (minorErrorCode != RPR_SUCCESS) {
				defaultMaterial_rprx = nullptr;
				DfxGlobal::console::println("Failed to set RPR Default UBER Material Diffuse Weight");
			}

			minorErrorCode = rprxMaterialSetParameterF(matsys_rprx, defaultMaterial_rprx, RPRX_UBER_MATERIAL_REFLECTION_COLOR, 0.5f, 0.5f, 0.5f, 1.f);

			if (minorErrorCode != RPR_SUCCESS) {
				defaultMaterial_rprx = nullptr;
				DfxGlobal::console::println("Failed to set RPR Default UBER Material Reflection Color");
			}

			minorErrorCode = rprxMaterialSetParameterF(matsys_rprx, defaultMaterial_rprx, RPRX_UBER_MATERIAL_REFLECTION_WEIGHT, 0.5f, 0.5f, 0.5f, 1.f);

			if (minorErrorCode != RPR_SUCCESS) {
				defaultMaterial_rprx = nullptr;
				DfxGlobal::console::println("Failed to set RPR Default UBER Material Reflection Weight");
			}

			minorErrorCode = rprxMaterialSetParameterF(matsys_rprx, defaultMaterial_rprx, RPRX_UBER_MATERIAL_REFLECTION_ROUGHNESS, 0.1f, 0.f, 0.f, 0.f);

			if (minorErrorCode != RPR_SUCCESS) {
				defaultMaterial_rprx = nullptr;
				DfxGlobal::console::println("Failed to set RPR Default UBER Material Reflection Roughness");
			}

			minorErrorCode = rprxMaterialSetParameterU(matsys_rprx, defaultMaterial_rprx, RPRX_UBER_MATERIAL_REFLECTION_MODE, RPRX_UBER_MATERIAL_COATING_MODE_METALNESS);

			if (minorErrorCode != RPR_SUCCESS) {
				defaultMaterial_rprx = nullptr;
				DfxGlobal::console::println("Failed to set RPR Default UBER Material Coating Metalness");
			}

			minorErrorCode = rprxMaterialSetParameterF(matsys_rprx, defaultMaterial_rprx, RPRX_UBER_MATERIAL_REFLECTION_METALNESS, 0.f, 0.f, 0.f, 1.f);

			if (minorErrorCode != RPR_SUCCESS) {
				defaultMaterial_rprx = nullptr;
				DfxGlobal::console::println("Failed to set RPR Default UBER Material Reflection Metalness");
			}

			minorErrorCode = rprxMaterialCommit(matsys_rprx, defaultMaterial_rprx);

			if (minorErrorCode != RPR_SUCCESS) {
				defaultMaterial_rprx = nullptr;
				DfxGlobal::console::println("Failed to set RPR Default UBER Material Commit");
			}
		}
	}

	void DfxRadeonProRenderNode::applyDefualtMatToObj(rpr_shape shp) {

		//minorErrorCode = rprShapeSetMaterial(shp, defaultMaterial);
		minorErrorCode = rprxShapeAttachMaterial(matsys_rprx, shp, defaultMaterial_rprx);

		if (minorErrorCode != RPR_SUCCESS) {
			DfxGlobal::console::println("Failed to apply RPR defualt material to shape");
		}
	}

	void DfxRadeonProRenderNode::setRenderMode(int val) {

		switch (val) {
		case 0:
			minorErrorCode = rprContextSetParameter1u(context, "rendermode", RPR_RENDER_MODE_GLOBAL_ILLUMINATION);
			if (minorErrorCode != RPR_SUCCESS) {
				DfxGlobal::console::println("Failed to set RPR render mode");
			}
			break;
		case 1:
			minorErrorCode = rprContextSetParameter1u(context, "rendermode", RPR_RENDER_MODE_DIRECT_ILLUMINATION);
			if (minorErrorCode != RPR_SUCCESS) {
				DfxGlobal::console::println("Failed to set RPR render mode");
			}
			break;
		case 2:
			minorErrorCode = rprContextSetParameter1u(context, "rendermode", RPR_RENDER_MODE_DIRECT_ILLUMINATION_NO_SHADOW);
			if (minorErrorCode != RPR_SUCCESS) {
				DfxGlobal::console::println("Failed to set RPR render mode");
			}
			break;
		case 3:
			minorErrorCode = rprContextSetParameter1u(context, "rendermode", RPR_RENDER_MODE_WIREFRAME);
			if (minorErrorCode != RPR_SUCCESS) {
				DfxGlobal::console::println("Failed to set RPR render mode");
			}
			break;
		case 4:
			minorErrorCode = rprContextSetParameter1u(context, "rendermode", RPR_RENDER_MODE_AMBIENT_OCCLUSION);
			if (minorErrorCode != RPR_SUCCESS) {
				DfxGlobal::console::println("Failed to set RPR render mode");
			}
			break;
		}
	}

	void DfxRadeonProRenderNode::setRayDepth(int depth) {

		minorErrorCode = rprContextSetParameter1u(context, "maxRecursion", depth);
		if (minorErrorCode != RPR_SUCCESS) {
			DfxGlobal::console::println("Failed to set RPR ray depth");
		}
	}

	void DfxRadeonProRenderNode::setDiffuseDepth(int depth) {
		minorErrorCode = rprContextSetParameter1u(context, "maxdepth.diffuse", depth);
		if (minorErrorCode != RPR_SUCCESS) {
			DfxGlobal::console::println("Failed to set RPR diffuse depth");
		}
	}

	void DfxRadeonProRenderNode::setGlossyDepth(int depth) {
		minorErrorCode = rprContextSetParameter1u(context, "maxdepth.glossy", depth);
		if (minorErrorCode != RPR_SUCCESS) {
			DfxGlobal::console::println("Failed to set RPR glossy depth");
		}
	}

	void DfxRadeonProRenderNode::setRefractionDepth(int depth) {

		minorErrorCode = rprContextSetParameter1u(context, "maxdepth.refraction", depth);
		if (minorErrorCode != RPR_SUCCESS) {
			DfxGlobal::console::println("Failed to set RPR refraction depth");
		}
	}

	void DfxRadeonProRenderNode::defaultSettings() {
		*(float*)getProperty(inCycles) = 50;
		*(float*)getProperty(inCycleUpdate) = 10;
		*(float*)getProperty(inRayDepth) = 8;
		*(float*)getProperty(inDiffuseDepth) = 3;
		*(float*)getProperty(inGlossyDepth) = 5;
		*(float*)getProperty(inRefractionDepth) = 5;
	}

	void DfxRadeonProRenderNode::extractFinalImage() {
		size_t size;

		if (final_image != nullptr) {
			delete[] final_image;
			final_image = nullptr;
			final_image_size = 0;
		}

		minorErrorCode = rprFrameBufferGetInfo(framebuffer, RPR_FRAMEBUFFER_DATA, 0, NULL, &size);

		if (minorErrorCode != RPR_SUCCESS) {
			DfxGlobal::console::println("Failed to get RPR Frame Buffer Info: Size");
		}
		else {

			int arrySize = size / sizeof(float);
			float* buffer = new float[arrySize];

			minorErrorCode = rprFrameBufferGetInfo(framebuffer, RPR_FRAMEBUFFER_DATA, size, buffer, 0);

			if (minorErrorCode != RPR_SUCCESS) {
				DfxGlobal::console::println("Failed to get RPR Frame Buffer Info: Raw Data");
			}
			else {

				final_image = new unsigned char[arrySize];
				final_image_size = sizeof(unsigned char) * arrySize;

				for (int i = 0; i < arrySize; i++) {
					final_image[i] = (unsigned int)std::max(255.f, 255.f*buffer[i]);
					//final_image[i] = buffer[i];
				}
			}

			delete[] buffer;
		}

	}

	QImage DfxRadeonProRenderNode::getFinalImage() {
		return q_final_image;
	}

	int DfxRadeonProRenderNode::iterations() {
		int rCycles = *(float*)getProperty(inCycles);

		return rCycles;
	}

	int  DfxRadeonProRenderNode::iterationsUpdateTrigger() {
		int rCycles = *(float*)getProperty(inCycleUpdate);

		return rCycles;
	}

	bool DfxRadeonProRenderNode::available() {
		initPlugin();

		int rc = ((DfxEnum*)getProperty(inDevice))->getCurrentIndex();

		if (rc == 0) {
			rpr_context temp_context;
			errorCode = rprCreateContext(RPR_API_VERSION, plugins, pluginCount, RPR_CREATION_FLAGS_ENABLE_CPU, NULL, NULL, &temp_context);
		
			if (errorCode != RPR_SUCCESS) {
				DfxGlobal::console::println("Radeon Pro Render: Unknown Device (CPU not Supported).");
				return false;
			}
			else {
				minorErrorCode = rprObjectDelete(temp_context);
			}
		}
		else if (rc == 1) {
			rpr_context temp_context;
			errorCode = rprCreateContext(RPR_API_VERSION, plugins, pluginCount, RPR_CREATION_FLAGS_ENABLE_GPU0, NULL, NULL, &temp_context);

			if (errorCode != RPR_SUCCESS) {
				DfxGlobal::console::println("Radeon Pro Render: Unknown Device (GPU not Supported or not found).");
				return false;
			}
			else {
				minorErrorCode = rprObjectDelete(temp_context);
			}
		}
		else {
			DfxGlobal::console::println("Radeon Pro Render: Unknown Device.");
		}

		return true;
	}
}

#endif