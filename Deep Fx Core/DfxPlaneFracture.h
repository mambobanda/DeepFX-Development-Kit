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

	static DfxGlobalEntityName DfxPlaneFracture_Identifier = "2F2X252X_DFX_PLANE_FRACTURE_NODE";

	class DfxPlaneFracture : public DfxEntity {
	private:
		std::vector<Dfx3dMeshData*> fructureMesh(Dfx3dMeshData *mesh, glm::mat4 trans, glm::vec3 pnormal, glm::vec3 pposition, float ampli, float freq, int octv, glm::vec3 segments, bool &success);

	public:
		static const DfxPropertyID outRef		= 0;
		static const DfxPropertyID proxies		= 1;
		static const DfxPropertyID inRef		= 2;
		static const DfxPropertyID inPlaneNormal	= 3;
		static const DfxPropertyID inPlanePosition	= 4;
		static const DfxPropertyID inAmplitude		= 5;
		static const DfxPropertyID inFrequency		= 6;
		static const DfxPropertyID inOctaveNumber	= 7;
		static const DfxPropertyID inSamplingInterval = 8;
		static const DfxPropertyID inPosition		= 9;
		static const DfxPropertyID inRotation		= 10;
		static const DfxPropertyID inScale			= 11;

		DfxPlaneFracture(DfxGlobalEntityName uniquename);

		virtual ~DfxPlaneFracture();

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