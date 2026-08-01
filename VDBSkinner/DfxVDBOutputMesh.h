/*
* The Mailo Today Solutions Software License
*
* Copyright 2018 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#pragma once

extern "C"
{
	struct DfxVDBFloat3
	{
		float x, y, z;
	};

	struct DfxVDBOutputMesh {

		DfxVDBFloat3 *verts;
		DfxVDBFloat3 *normals;
		unsigned int *indices;

		unsigned int nbVerts;
		unsigned int nbFaces;
	};

	struct DfxVDBOutputMeshFunctions
	{
		DfxVDBOutputMesh *(*new_mesh)();
		void (*init_vertex_array)(DfxVDBOutputMesh *, unsigned int);
		void (*init_index_array)(DfxVDBOutputMesh *, unsigned int);
		void(*msg)(const char *);
	};
}