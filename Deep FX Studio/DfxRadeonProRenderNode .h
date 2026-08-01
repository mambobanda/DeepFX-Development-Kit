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

#pragma once

#if ENABLE_PROTOTYPE_FEATURES

#include <DfxGlobal.h>
#include <DfxEntityDB.h>
#include <RadeonProRender.h>
#include <RprSupport.h>
#include <DfxRenderInterface.h>
#include <vector>


namespace radeon {

	typedef struct st_light{
		rpr_light light;
		size_t nodeID;
	}rprLight;

	typedef struct st_camera {
		rpr_camera camera;
		size_t nodeID;
	}rprCamera;

	typedef struct st_mesh {
		rpr_shape mesh;
		size_t nodeID;
	}rprMesh;

	static DfxGlobalEntityName DfxRadeonProRenderNode_Identifier = "2F2X252X_DFX_RADEON_PRO_RENDER_NODE";

	class DfxRadeonProRenderNode : public DfxRenderInterface {
	private:
		static size_t pluginCount;
		static rpr_int plugins[1];
		static rpr_int tahoePluginID;
		static bool pluginFailedToLoad;
		static bool tahoePluginLoaded;

	private:

		static void initPlugin();
	private:

		bool error();
		void newRPRContext();
		void newRPRFrameBuffer();
	private:
		rpr_shape createShape(Dfx3dMeshData *data);
		
		void brandNewScene(std::vector<size_t> nodes);
		void updateScene(std::vector<size_t> nodes);
		void setupDefualtMat();
		void applyDefualtMatToObj(rpr_shape);
	private:

		bool failedToCreateContext;
		rpr_int errorCode;
		rpr_int minorErrorCode;

		rpr_framebuffer framebuffer;
		rpr_framebuffer framebuffer01;
		rpr_material_system matsys;
		rprx_context matsys_rprx;
		rpr_scene scene;
		rpr_material_node defaultMaterial;
		rprx_material defaultMaterial_rprx;
		rpr_context context;

		std::vector<rprCamera> cameras;
		std::vector<rprLight> lights;
		std::vector<rprMesh>  shapes;

	private:
		QImage q_final_image;
		unsigned char *final_image;
		unsigned int final_image_size;
	public:

		static const DfxPropertyID outRef		= 0;
		static const DfxPropertyID inDevice		= 1;
		static const DfxPropertyID inRenderType = 2;
		static const DfxPropertyID inCycles		= 3;
		static const DfxPropertyID inCycleUpdate	= 4;
		static const DfxPropertyID inRayDepth	= 5;
		static const DfxPropertyID inDiffuseDepth		= 6;
		static const DfxPropertyID inRefractionDepth	= 7;
		static const DfxPropertyID inGlossyDepth		= 8;

	public:
		DfxRadeonProRenderNode(DfxGlobalEntityName uniquename);
		virtual ~DfxRadeonProRenderNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		unsigned char *getFinalImageBuffer() override;

		unsigned int getFinalImageBufferSize() override;

		QImage getFinalImage() override;

		bool isExportable() override;

		int  iterations() override;
		
		int  iterationsUpdateTrigger() override;
		
		bool available() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();

		void printErrorCode();

		void newFrame(std::vector<size_t> nodes) override;

		void render() override;

		void cleanUp() override;

		void setRenderMode(int val);

		void setRayDepth(int depth);

		void setDiffuseDepth(int depth);

		void setGlossyDepth(int depth);

		void setRefractionDepth(int depth);

		void defaultSettings();

		void extractFinalImage();
	};

}

#endif