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

#include <vector>
#include <DfxEntity.h>
#include <DfxTraitUID.h>
#include <DfxUtility.h>
#include <QImage>
#include <QRgb>
#include <QColor>
#include "DfxEntityBasicPropertyTypes.h"
#include "DfxEntityDB.h"
#include "DfxGlobal.h"
#include "DfxNvBlastGlobal.h"


namespace dfxcore {

	static DfxGlobalEntityName DfxCutoutFracture_Identifier = "2F2X252X_DFX_CUTOUT_FRACTURE_NODE";

	class DfxCutoutFracture : public DfxEntity {
	private:
		struct BITMAP {
			uint8_t* bitmap;
			int32_t width; 
			int32_t height;
		};

		std::vector<Dfx3dMeshData *> fructureMesh(Dfx3dMeshData *mesh, glm::mat4 trans, glm::vec3 plane_normal,
		glm::vec3 plane_point, uint8_t* bitmap, int32_t map_width, int32_t map_height, bool &success);
		
		bool loadImage(std::string path, BITMAP &bmp);
	public:
		static const DfxPropertyID outRef			= 0;
		static const DfxPropertyID proxies			= 1;
		static const DfxPropertyID inRef			= 2;
		static const DfxPropertyID inImagePath		= 3;
		static const DfxPropertyID inPlaneNormal	= 4;
		static const DfxPropertyID inPlanePosition	= 5;
		static const DfxPropertyID inPosition		= 6;
		static const DfxPropertyID inRotation		= 7;
		static const DfxPropertyID inScale			= 8;

		DfxCutoutFracture(DfxGlobalEntityName uniquename);

		virtual ~DfxCutoutFracture();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();

		void addUID(DfxEntityPropertyTypes::UID*);

		void removeUID(DfxEntityPropertyTypes::UID*);

		glm::mat4 getAbsoluteTransform();

		glm::mat4 getAbsoluteTransform(DfxEntityPropertyTypes::ProxyObject *);
	};
}