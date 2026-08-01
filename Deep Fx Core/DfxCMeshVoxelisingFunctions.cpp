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

#include "DfxCMeshVoxelisingFunctions.h"

namespace dfxcore {

	void resetSpatialHashAttributes(DfxBounds bbox, float size, float padding) {
		float halfSize = 0.5f*size;

		bbox.addPadding(padding, padding, padding);

		float xlength = std::abs((bbox.xmax + size) - (bbox.xmin - size));
		float ylength = std::abs((bbox.ymax + size) - (bbox.ymin - size));
		float zlength = std::abs((bbox.zmax + size) - (bbox.zmin - size));

		int xcellCount = (int)(xlength / (size));
		int ycellCount = (int)(ylength / (size));
		int zcellCount = (int)(zlength / (size));

		float xincrement = bbox.xmin;
		float yincrement = bbox.ymin;
		float zincrement = bbox.zmin;

		float gridCellSize = size;
		int gridXCellCount = xcellCount;
		int gridYCellCount = ycellCount;
		int gridZCellCount = zcellCount;

		setSpatialHashAttributes(gridCellSize, gridXCellCount * gridYCellCount * gridZCellCount);
	}

	Voxel genVoxel(glm::vec3 location, DfxBounds &bbox, float size) {

		float halfSize = 0.5f*size;

		Voxel vox;
		vox.active = false;

		float xincrement = (bbox.xmin + size) + (size * location.x);
		float yincrement = (bbox.ymin + size) + (size * location.y);
		float zincrement = (bbox.zmin + size) + (size * location.z);

		vox.origin = glm::vec3(xincrement, yincrement, zincrement);
		vox.center = glm::vec3(xincrement + halfSize,
			yincrement + halfSize,
			zincrement + halfSize);

		vox.bounds.empty();
		vox.bounds.add(xincrement,
			yincrement,
			zincrement);

		vox.bounds.add(xincrement,
			yincrement,
			zincrement + size);

		vox.bounds.add(xincrement + size,
			yincrement,
			zincrement);

		vox.bounds.add(xincrement,
			yincrement + size,
			zincrement);

		vox.bounds.add(xincrement + size,
			yincrement,
			zincrement + size);

		vox.bounds.add(xincrement + size,
			yincrement + size,
			zincrement + size);

		vox.bounds.add(xincrement,
			yincrement + size,
			zincrement + size);

		vox.bounds.add(xincrement + size,
			yincrement + size,
			zincrement);

		return vox;
	}

	void voxelizeMesh(Dfx3dMeshData &mesh, glm::vec3 scale, std::vector<glm::vec3> &positions, float voxelSize) {

		//vector<glm::vec3> positions;
		DfxBounds bbox;

#ifdef WIN32
		concurrency::concurrent_unordered_map< glm::vec3, glm::vec3, SpatialHash > cells;
#else
		std::unordered_map < glm::vec3, glm::vec3, SpatialHash > cells;
#endif

		bbox.empty();
		//#pragma omp parallel default(shared)
		{
			///	#pragma omp for schedule(static)
			for (int i = 0; i < mesh.getVertexCount(); i++) {
				bbox.add(mesh.getVertex(i).x * scale.x,
					mesh.getVertex(i).y * scale.y,
					mesh.getVertex(i).z * scale.z);
			}
		}

		resetSpatialHashAttributes(bbox, voxelSize, voxelSize);

#ifdef WIN32
#pragma omp parallel default(shared)
		{
#pragma omp for schedule(static)
#endif
			for (int i = 0; i < mesh.getIndexCount() / 3; i++) {
				DfxBounds triBox;
				int counter = i * 3;
				float triverts[3][3] =
				{
					{ mesh.getVertex(mesh.getIndex(counter)).x * scale.x,
					mesh.getVertex(mesh.getIndex(counter)).y * scale.y,
					mesh.getVertex(mesh.getIndex(counter)).z * scale.z
					},

					{ mesh.getVertex(mesh.getIndex(counter + 1)).x * scale.x,
					mesh.getVertex(mesh.getIndex(counter + 1)).y * scale.y,
					mesh.getVertex(mesh.getIndex(counter + 1)).z * scale.z
					},

					{ mesh.getVertex(mesh.getIndex(counter + 2)).x * scale.x,
					mesh.getVertex(mesh.getIndex(counter + 2)).y * scale.y,
					mesh.getVertex(mesh.getIndex(counter + 2)).z * scale.z
					}
				};

				triBox.empty();
				triBox.add(triverts[0][0], triverts[0][1], triverts[0][2]);
				triBox.add(triverts[1][0], triverts[1][1], triverts[1][2]);
				triBox.add(triverts[2][0], triverts[2][1], triverts[2][2]);

				float minx = triBox.xmin - (voxelSize * 2.0f);
				float miny = triBox.ymin - (voxelSize * 2.0f);
				float minz = triBox.zmin - (voxelSize * 2.0f);


				float voxelHalfSize = voxelSize * 0.5f;
				int xcount = ((triBox.xmax + voxelSize) - (minx - voxelSize)) / voxelHalfSize;
				int ycount = ((triBox.ymax + voxelSize) - (miny - voxelSize)) / voxelHalfSize;
				int zcount = ((triBox.zmax + voxelSize) - (minz - voxelSize)) / voxelHalfSize;

				for (int x = 0; x <= xcount; x++) {

					for (int y = 0; y <= ycount; y++) {

						for (int z = 0; z <= zcount; z++) {

							bool isVoxel = false;
							glm::vec3 location = getSpatialLocation(glm::vec3(minx, miny, minz), bbox.xmin, bbox.ymin, bbox.zmin);

							auto it = cells.find(location);

							if (it != cells.end()) isVoxel = true;

							if (!isVoxel) {

								Voxel vox = genVoxel(location, bbox, voxelSize);
								float center[] = { vox.center.x, vox.center.y, vox.center.z };
								float halfExtents[] = { 0.5f*voxelSize, 0.5f*voxelSize, 0.5f*voxelSize };

								if (vox.bounds.triangleBoxOverlap(center, halfExtents, triverts)) {

									cells.insert(std::pair<glm::vec3, glm::vec3>(location, vox.center));

								}
							}

							minz += voxelHalfSize;
						}

						minz = triBox.zmin - (voxelSize * 2.0f);
						miny += voxelHalfSize;
					}

					minz = triBox.zmin - (voxelSize * 2.0f);
					miny = triBox.ymin - (voxelSize * 2.0f);
					minx += voxelHalfSize;

				}

			}
#ifdef WIN32
		}
#endif

		auto it = cells.begin();
		for (; it != cells.end(); it++) {
			positions.push_back(it->second);
		}
		//positions = vgrid.getActiveVoxelCenters();

	}

	void voxelizeMeshVolume(Dfx3dMeshData &mesh, glm::vec3 scale, std::vector<glm::vec3> &positions, float voxelSize, bool shell) {
		//vector<glm::vec3> positions;
		DfxBounds bbox;
		VoxelGrid vgrid((mesh.getIndexCount() / 3) * 2);

		bbox.empty();
		//#pragma omp parallel default(shared)
		{
			///	#pragma omp for schedule(static)
			for (int i = 0; i < mesh.getVertexCount(); i++) {
				bbox.add(mesh.getVertex(i).x * scale.x,
					mesh.getVertex(i).y * scale.y,
					mesh.getVertex(i).z * scale.z);
			}
		}
		vgrid.setBBox(bbox);
		vgrid.buildVoxelGrid(voxelSize, voxelSize);

		//#pragma omp parallel default(shared)
		{
			//	#pragma omp for schedule(static)
			for (int i = 0; i < mesh.getIndexCount() / 3; i++) {
				DfxBounds triBox;
				int counter = i * 3;
				float triverts[3][3] =
				{
					{ mesh.getVertex(mesh.getIndex(counter)).x * scale.x,
					  mesh.getVertex(mesh.getIndex(counter)).y * scale.y,
					  mesh.getVertex(mesh.getIndex(counter)).z * scale.z
					},

					{ mesh.getVertex(mesh.getIndex(counter + 1)).x * scale.x,
					  mesh.getVertex(mesh.getIndex(counter + 1)).y * scale.y,
					  mesh.getVertex(mesh.getIndex(counter + 1)).z * scale.z
					},

					{ mesh.getVertex(mesh.getIndex(counter + 2)).x * scale.x,
					  mesh.getVertex(mesh.getIndex(counter + 2)).y * scale.y,
					  mesh.getVertex(mesh.getIndex(counter + 2)).z * scale.z
					}
				};

				triBox.empty();
				triBox.add(triverts[0][0], triverts[0][1], triverts[0][2]);
				triBox.add(triverts[1][0], triverts[1][1], triverts[1][2]);
				triBox.add(triverts[2][0], triverts[2][1], triverts[2][2]);

				float minx = triBox.xmin - (voxelSize * 2.0f);
				float miny = triBox.ymin - (voxelSize * 2.0f);
				float minz = triBox.zmin - (voxelSize * 2.0f);


				float voxelHalfSize = voxelSize * 0.5f;
				int xcount = ((triBox.xmax + voxelSize) - (minx - voxelSize)) / voxelHalfSize;
				int ycount = ((triBox.ymax + voxelSize) - (miny - voxelSize)) / voxelHalfSize;
				int zcount = ((triBox.zmax + voxelSize) - (minz - voxelSize)) / voxelHalfSize;

				for (int x = 0; x <= xcount; x++) {

					for (int y = 0; y <= ycount; y++) {

						for (int z = 0; z <= zcount; z++) {

							Voxel *vox = vgrid.getVoxel(glm::vec3(minx, miny, minz));

							if (vox != NULL) {

								float center[] = { vox->center.x, vox->center.y, vox->center.z };
								float halfExtents[] = { 0.5f*voxelSize, 0.5f*voxelSize, 0.5f*voxelSize };

								if (vox->bounds.triangleBoxOverlap(center, halfExtents, triverts)) {
									VoxTriangles *vtris;

									if (shell)
										vox->active = true;

									if (vox->userdata == NULL) {
										vtris = new VoxTriangles;
										vox->userdata = vtris;
									}
									else {
										vtris = (VoxTriangles*)vox->userdata;
									}

									vtris->t.push_back(counter);
								}
							}

							minz += voxelHalfSize;
						}

						minz = triBox.zmin - (voxelSize * 2.0f);
						miny += voxelHalfSize;
					}

					minz = triBox.zmin - (voxelSize * 2.0f);
					miny = triBox.ymin - (voxelSize * 2.0f);
					minx += voxelHalfSize;

				}
			}
		}

		//Voxelize Volume
		int xcount = vgrid.getGridXCellCount();
		int ycount = vgrid.getGridYCellCount();
		int zcount = vgrid.getGridZCellCount();

		bool penup = true;

		//#pragma omp parallel default(shared)
		//{
		//#pragma omp for schedule(static)
		for (int z = 0; z < zcount; z++) {

			for (int y = 0; y < ycount; y++) {
				bool penup = true;

				for (int x = 0; x < xcount; x++) {

					bool hasTriangles = true;
					bool stateChange = false;
					Voxel *vox = vgrid.getDirectVoxel(glm::vec3(x, y, z));


					if (vox->userdata == NULL)
						hasTriangles = false;

					if (!hasTriangles && penup)
						continue;

					if (hasTriangles) {
						VoxTriangles *vtris = (VoxTriangles *)vox->userdata;

						int nb = vtris->t.size();

						for (int i = 0; i < nb; i++) {
							glm::vec3 ray(-10.0, 0.0, 0.0);
							int counter = vtris->t[i];
							float triverts[3][3] =
							{
								{ mesh.getVertex(mesh.getIndex(counter)).x * scale.x,
								  mesh.getVertex(mesh.getIndex(counter)).y * scale.y,
								  mesh.getVertex(mesh.getIndex(counter)).z * scale.z
								},

								{ mesh.getVertex(mesh.getIndex(counter + 1)).x * scale.x,
								  mesh.getVertex(mesh.getIndex(counter + 1)).y * scale.y,
								  mesh.getVertex(mesh.getIndex(counter + 1)).z * scale.z
								},

								{ mesh.getVertex(mesh.getIndex(counter + 2)).x * scale.x,
								  mesh.getVertex(mesh.getIndex(counter + 2)).y * scale.y,
								  mesh.getVertex(mesh.getIndex(counter + 2)).z * scale.z }
							};

							glm::vec3 p1 = glm::vec3(triverts[0][0], triverts[0][1], triverts[0][2]);
							glm::vec3 p2 = glm::vec3(triverts[1][0], triverts[1][1], triverts[1][2]);
							glm::vec3 p3 = glm::vec3(triverts[2][0], triverts[2][1], triverts[2][2]);

							glm::vec3 normal = glm::cross((p2 - p1), (p3 - p1));
							float dot = glm::dot(normal, ray);

							if (dot > 0) {
								penup = false;
								stateChange = true;
								break;
							}
							else {
								penup = true;
								break;
							}
						}
					}

					if (stateChange)
						continue;

					if (!penup) {
						vox->active = true;
					}
				}
			}
		}
		//}

		//Clean Volume Voxelisation
		//#pragma omp parallel default(shared)
		//{
		//#pragma omp for schedule(static)
		for (int z = 0; z < zcount; z++) {

			for (int y = 0; y < ycount; y++) {
				bool penup = false;

				for (int x = xcount - 1; x >= 0; x--) {
					bool hasTriangles = true;
					bool stateChange = false;
					Voxel *vox = vgrid.getDirectVoxel(glm::vec3(x, y, z));


					if (vox->userdata == NULL)
						hasTriangles = false;

					if (!hasTriangles && penup)
						continue;

					if (hasTriangles) {
						VoxTriangles *vtris = (VoxTriangles *)vox->userdata;

						int nb = vtris->t.size();

						for (int i = 0; i < nb; i++) {
							glm::vec3 ray(10.0, 0.0, 0.0);
							int counter = vtris->t[i];
							float triverts[3][3] =
							{ { mesh.getVertex(mesh.getIndex(counter)).x * scale.x,
								mesh.getVertex(mesh.getIndex(counter)).y * scale.y,
								mesh.getVertex(mesh.getIndex(counter)).z * scale.z
							  },

							  { mesh.getVertex(mesh.getIndex(counter + 1)).x * scale.x,
								mesh.getVertex(mesh.getIndex(counter + 1)).y * scale.y,
								mesh.getVertex(mesh.getIndex(counter + 1)).z * scale.z
							  },

							  { mesh.getVertex(mesh.getIndex(counter + 2)).x * scale.x,
								mesh.getVertex(mesh.getIndex(counter + 2)).y * scale.y,
								mesh.getVertex(mesh.getIndex(counter + 2)).z * scale.z
							  }
							};

							glm::vec3 p1 = glm::vec3(triverts[0][0], triverts[0][1], triverts[0][2]);
							glm::vec3 p2 = glm::vec3(triverts[1][0], triverts[1][1], triverts[1][2]);
							glm::vec3 p3 = glm::vec3(triverts[2][0], triverts[2][1], triverts[2][2]);

							glm::vec3 normal = glm::cross((p2 - p1), (p3 - p1));
							float dot = glm::dot(normal, ray);

							if (dot > 0) {
								penup = true;
								stateChange = true;
								break;
							}
							else {
								penup = false;
								break;
							}
						}
					}

					if (stateChange)
						continue;

					if (!penup) {
						//#pragma omp critical
						vox->active = false;
					}
				}
			}
		}
		//}
		positions = vgrid.getActiveVoxelCenters();
		vgrid.release();
	}
}