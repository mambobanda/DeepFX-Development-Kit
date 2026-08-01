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

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <DfxScene.h>
#include <btBulletDynamicsCommon.h>
#include <vector>
#include "util.h"
#include "DfxCSimulationEntity.h"
#include "DfxCDivergenceFreeSPHSolver.h"

namespace dfxcore {

	namespace r_n_d {

		static DfxGlobalEntityName btDbvtTester_Identifier = "2F2X252X_BT_DBVT_TESTER";

		struct xxTriangle {
			btVector3 points[3];
		};

		class CollideCallback : public btDbvt::ICollide {
		public:
			inline void	Process(const btDbvtNode *node) {
				
				if (node->isleaf()) {
					xxTriangle *tri = (xxTriangle*)node->data;

					std::cout << "P1: " << tri->points[0].x() << " " << tri->points[0].y() << " " << tri->points[0].y() << std::endl;
					std::cout << "P2: " << tri->points[1].x() << " " << tri->points[1].y() << " " << tri->points[1].y() << std::endl;
					std::cout << "P3: " << tri->points[2].x() << " " << tri->points[2].y() << " " << tri->points[2].y() << std::endl << std::endl;
				}
			}
			//void	Process(const btDbvtNode* n, btScalar) { Process(n); }
			//bool	Descent(const btDbvtNode*) { return(true); }
			//bool	AllLeaves(const btDbvtNode*) { return(true); }
		};

		class btDbvtTester : public DfxCSimulationEntity {
		private:
			btDbvt *bvh;
			btDbvtNode *root;
			std::vector<xxTriangle*> triangles;
			std::vector<btDbvtVolume> volumes;

			void clearTriangles();
			void clearVolumes();
		public:
			static const DfxPropertyID inMesh		= 0;
			static const DfxPropertyID inRayFrom	= 1;
			static const DfxPropertyID inRayTo		= 2;
		public:

			btDbvtTester(DfxGlobalEntityName uniquename);
			virtual ~btDbvtTester();

			void init()  override;

			void clear() override;

			void done()  override;

			void compute() override;

			void reset() override;

			void onDataChanged(DfxEntityDataInstance *instance) override;

			void draw() override;

			DfxBounds bounds() override;

			bool selectable() override;

			void calculateBounds() override;

			static void initialize();
		};
	}
}