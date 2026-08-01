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

#include <algorithm>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <DfxEntityDB.h>
#include <DfxEntityBasicPropertyTypes.h>
#include <DfxBounds.h>
#include "Dfx3dMeshData.h"

namespace dfxutil {

	Dfx3dMeshData *findMeshData(DfxEntityPropertyTypes::UID *);
	DfxEntityPropertyTypes::ProxyObjectArray *findProxyMeshData(DfxEntityPropertyTypes::UID *);

	std::vector<DfxEntity*> traitUIDFindAll( DfxEntityPropertyTypes::UID &uid );

	std::vector<DfxEntity*> traitUIDSearch(DfxEntityPropertyTypes::UID &uid, DfxGlobalEntityName uname, DfxGlobalEntityName uname_limit = "", bool limit_search = false);

	void traitUIDLinkRelated(DfxEntity *, DfxPropertyID);

	// Geo Tools
	namespace geo {
		glm::mat4 createTransformationMatrix(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl);
		glm::vec3 meshCenter(Dfx3dMeshData &, glm::vec3 pos=glm::vec3(), glm::vec3 rot = glm::vec3(), glm::vec3 scl = glm::vec3(1.f, 1.f, 1.f));
		glm::vec3 moveMeshToOrigin(Dfx3dMeshData &);
	}
}