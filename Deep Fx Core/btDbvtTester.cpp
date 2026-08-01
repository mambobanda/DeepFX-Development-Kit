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

#include "btDbvtTester.h"
#include "DfxSimulationCoreGlobal.h"

namespace dfxcore {

	namespace r_n_d {

		btDbvtTester::btDbvtTester(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename, DFX_POINT_ITEM) {
			setLogicalName("btDbvtTester");

			setPropertyState(inMesh, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inRayFrom, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inRayTo, DfxEntityPropertyState::INPUT_STATE);

			root = nullptr;
			bvh = nullptr;
		}

		btDbvtTester::~btDbvtTester() {
		
		}

		void btDbvtTester::init() {

		}

		void btDbvtTester::clear() {

		}

		void btDbvtTester::done() {

		}

		void btDbvtTester::reset() {
			
		}

		void btDbvtTester::clearTriangles() {
			if (triangles.size() > 0) {
				
				for (auto tri : triangles) delete tri;
				triangles.clear();
			}
		}

		void btDbvtTester::clearVolumes() {
			if (volumes.size() > 0) {

				//for (auto vol : volumes) delete vol;
				volumes.clear();
			}
		}

		void btDbvtTester::compute() {
			Dfx3dMeshData *mesh = (Dfx3dMeshData*)getProperty(inMesh);

			if (propertyHasBinding(inMesh)) {

				if (bvh != nullptr) {
					bvh->clear();
					delete bvh;

					bvh = new btDbvt;
				}
				else {
					bvh = new btDbvt;
				}

				clearTriangles();
				clearVolumes();

				if (mesh->getVertexCount() > 3 && ((int)(mesh->getIndexCount() / 3)) >= 1) {
					for (int i = 0; i < mesh->getIndexCount() / 3; i++) {
						
						int counter = i * 3;
						btVector3 a(mesh->getVertex(mesh->getIndex(counter)).x,
									mesh->getVertex(mesh->getIndex(counter)).y,
									mesh->getVertex(mesh->getIndex(counter)).z);

						btVector3 b(mesh->getVertex(mesh->getIndex(counter + 1)).x,
									mesh->getVertex(mesh->getIndex(counter + 1)).y,
									mesh->getVertex(mesh->getIndex(counter + 1)).z);

						btVector3 c(mesh->getVertex(mesh->getIndex(counter + 2)).x,
									mesh->getVertex(mesh->getIndex(counter + 2)).y,
									mesh->getVertex(mesh->getIndex(counter + 2)).z);
						

						xxTriangle *t = new xxTriangle;
						t->points[0] = a;
						t->points[1] = b;
						t->points[2] = c;

						btDbvtVolume volume = btDbvtVolume::FromPoints(t->points, 3);
						
						/*DfxGlobal::console::println(std::to_string(volume.tMins().x()) + 
													", " +
													std::to_string(volume.tMins().y()) +
													", " +
													std::to_string(volume.tMins().z()));*/

						root = bvh->insert(volume, t);

						triangles.push_back(t);
						volumes.push_back(volume);
					}
				}
			}

			cleanAllProperties();
			getDataInstance()->makeClean();
		}

		void btDbvtTester::onDataChanged(DfxEntityDataInstance *instance) {

		}

		void btDbvtTester::draw() {
			using namespace DfxEntityPropertyTypes;
			initializeOpenGLFunctions();

			pingParents();
			if (!getDataInstance()->isClean()) compute();

			if (propertyHasBinding(inRayFrom) && propertyHasBinding(inRayTo)) {
				
				CollideCallback collideCallback;

				btVector3 r1(((Float3*)getProperty(inRayFrom))->x,
							((Float3*)getProperty(inRayFrom))->y,
							((Float3*)getProperty(inRayFrom))->z);

				btVector3 r2(((Float3*)getProperty(inRayTo))->x,
							((Float3*)getProperty(inRayTo))->y,
							((Float3*)getProperty(inRayTo))->z);

				if (propertyHasBinding(inMesh)) {
					bvh->rayTest(bvh->m_root, r1, r2, collideCallback);
				}

				glDisable(GL_LIGHTING);
				glColor3f(1.f, 1.0f, 0.f);
				
				glBegin(GL_LINES);
				glVertex3f(((Float3*)getProperty(inRayFrom))->x, 
						   ((Float3*)getProperty(inRayFrom))->y, 
						   ((Float3*)getProperty(inRayFrom))->z);

				glVertex3f(((Float3*)getProperty(inRayTo))->x,
						   ((Float3*)getProperty(inRayTo))->y,
						   ((Float3*)getProperty(inRayTo))->z);
				glEnd();
				
				glEnable(GL_LIGHTING);
			}
			
			/*glm::vec3 primary(0.f, 0.f, 1.0);
			glm::vec3 secondary(.85f, .85f, .85f);

			initializeOpenGLFunctions();
			int frame_num = *(float*)getProperty(inFrame);
			float psize = *(float*)getProperty(inParticleSize);

			if (getFrameCount() > 0) {

				SimulationFrame *frame = getFrame(frame_num);
				if (frame == nullptr) frame = getFrame(frame_num - 1);

				if (frame != nullptr) {

					if (frame->isCached()) {
						glDisable(GL_LIGHTING);
						glPointSize(psize);
						glColor3f(0.f, 1.0f, 0.f);
						for (int i = 0; i < frame->getParticleCount(); i++) {

							glm::vec3 pos = frame->getParticles()[i];
							float speed = frame->getSpeeds()[i];

							glm::vec3 custom;
							getPreviewColor(speed, 10.0f, primary, secondary, custom);

							glBegin(GL_POINTS);
							glColor3f(custom[0], custom[1], custom[2]);
							glVertex3f(pos.x, pos.y, pos.z);
							glEnd();
						}
						glEnable(GL_LIGHTING);
					}
				}
			}*/
		}

		DfxBounds btDbvtTester::bounds() {
			return DfxBounds();
		}

		bool btDbvtTester::selectable() {
			return false;
		}

		void btDbvtTester::calculateBounds() {
			//
		}

		void btDbvtTester::initialize() {
			DfxCSimulationEntity::initialize();

			DfxEntityDB::createProperty(inMesh, "mesh", new DfxEntityPropertyTypes::Dfx3DMeshPropertyType());
			DfxEntityDB::createProperty(inRayFrom, "ray_from", new DfxEntityPropertyTypes::Float3PropertyType());
			DfxEntityDB::createProperty(inRayTo, "ray_to", new DfxEntityPropertyTypes::Float3PropertyType());
		}

	}
}