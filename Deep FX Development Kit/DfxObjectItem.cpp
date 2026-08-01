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

#include "DfxObjectItem.h"
#include "DfxScene.h"


DfxObjectItem::DfxObjectItem(DfxGlobalEntityName uniquename): DfxViewItem(DFX_OBJECT_ITEM, uniquename) {
	ready = false;
	cached = false;
	fileReferenced = true;

	setPropertyState(mesh, DfxEntityPropertyState::DATA_STORE_STATE);

	setPropertyState(item_id, DfxEntityPropertyState::OUTPUT_STATE);
	setPropertyState(outPosition, DfxEntityPropertyState::OUTPUT_STATE);
	setPropertyState(outRotation, DfxEntityPropertyState::OUTPUT_STATE);
	setPropertyState(outScale, DfxEntityPropertyState::OUTPUT_STATE);

	//setPropertyState(inMode, DfxEntityPropertyState::INPUT_STATE);
	//setPropertyState(inFilePath, DfxEntityPropertyState::INPUT_STATE);
	//setPropertyState(inIgnoreLocalTransform, DfxEntityPropertyState::INPUT_STATE);
	//setPropertyState(inDisplacementIndex, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(position, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(rotation, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(scale, DfxEntityPropertyState::INPUT_STATE);

	//displacementMap = new DfxObjectDisplacementMap();
	activeDisplacement = nullptr;

	((DfxTraitUID<size_t>*)getProperty(item_id))->setRootID(getInstanceID());

	((DfxEnum*)getProperty(inMode))->add("Keyframe");
	((DfxEnum*)getProperty(inMode))->add("Displacement");

	((DfxEnum*)getProperty(inIgnoreLocalTransform))->add("No");
	((DfxEnum*)getProperty(inIgnoreLocalTransform))->add("Yes");

	*(float*)getProperty(inDisplacementIndex) = 1;
}

DfxObjectItem::~DfxObjectItem() {
	//delete displacementMap;

	if (cached) {
		deleteCache();
	}
}

void DfxObjectItem::copy(DfxEntity& other) {

	auto properties = mapEntityProperties(&other);
	mapEntityProperties(this, properties);

	if (other.UniqueName().compare(DfxObjectItem_Identifier) == 0) {
		auto mesh = ((DfxObjectItem*)&other)->getMeshData();
		getMeshData()->copy(*mesh);

		setLogicalName(other.getLogicalName());
		calculateBounds();
		initOpenGlBuffer();
		setIsReady(true);

		DfxScene::getInstance()->addItem(this);

		DfxGlobal::command::exec("updateViewport");
	}
}

void DfxObjectItem::deleteCache() {
	glDeleteLists(displayListID, 1);
	glDeleteLists(displayListSelID, 1);
}

void DfxObjectItem::cacheMesh() {
	Dfx3dMeshData *mdata = getMeshData();

	glm::vec3 selColor(1.f, 0.498f, 0.164);

	displayListID = glGenLists(1);
	displayListSelID = glGenLists(1);
	displayListSelEvtID = glGenLists(1);

	glNewList(displayListID, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	size_t counter = 0;
	for (size_t inx = 0; inx < mdata->getIndexCount() / 3; inx++) {

		auto vindex1 = mdata->getIndex(counter);
		auto vindex2 = mdata->getIndex(counter + 1);
		auto vindex3 = mdata->getIndex(counter + 2);

		glm::vec3 pos1;
		glm::vec3 pos2;
		glm::vec3 pos3;

		glm::vec3 col1;
		glm::vec3 col2;
		glm::vec3 col3;

		pos1 = mdata->getVertex(vindex1);
		pos2 = mdata->getVertex(vindex2);
		pos3 = mdata->getVertex(vindex3);

		col1 = mdata->getColor(vindex1);
		col2 = mdata->getColor(vindex2);
		col3 = mdata->getColor(vindex3);

		glm::vec3 nom = glm::triangleNormal(pos1, pos2, pos3);
		glNormal3f(nom.x, nom.y, nom.z);

		glNormal3f(nom.x, nom.y, nom.z);

		glColor3f(col1.x, col1.y, col1.z);
		glVertex3f(pos1.x, pos1.y, pos1.z);

		glColor3f(col2.x, col2.y, col2.z);
		glVertex3f(pos2.x, pos2.y, pos2.z);

		glColor3f(col3.x, col3.y, col3.z);
		glVertex3f(pos3.x, pos3.y, pos3.z);

		counter += 3;
	}
	glEnd();
	glEndList();

	glNewList(displayListSelID, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	counter = 0;
	for (size_t inx = 0; inx < mdata->getIndexCount() / 3; inx++) {

		auto vindex1 = mdata->getIndex(counter);
		auto vindex2 = mdata->getIndex(counter + 1);
		auto vindex3 = mdata->getIndex(counter + 2);

		glm::vec3 pos1;
		glm::vec3 pos2;
		glm::vec3 pos3;

		glm::vec3 col1;
		glm::vec3 col2;
		glm::vec3 col3;

		pos1 = mdata->getVertex(vindex1);
		pos2 = mdata->getVertex(vindex2);
		pos3 = mdata->getVertex(vindex3);

		glm::vec3 nom = glm::triangleNormal(pos1, pos2, pos3);
		glNormal3f(nom.x, nom.y, nom.z);

		glNormal3f(nom.x, nom.y, nom.z);

		glColor3f(selColor.x, selColor.y, selColor.z);
		glVertex3f(pos1.x, pos1.y, pos1.z);

		glColor3f(selColor.x, selColor.y, selColor.z);
		glVertex3f(pos2.x, pos2.y, pos2.z);

		glColor3f(selColor.x, selColor.y, selColor.z);
		glVertex3f(pos3.x, pos3.y, pos3.z);

		counter += 3;
	}
	glEnd();
	glEndList();

	glNewList(displayListSelEvtID, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	counter = 0;
	for (size_t inx = 0; inx < mdata->getIndexCount() / 3; inx++) {

		auto vindex1 = mdata->getIndex(counter);
		auto vindex2 = mdata->getIndex(counter + 1);
		auto vindex3 = mdata->getIndex(counter + 2);

		glm::vec3 pos1;
		glm::vec3 pos2;
		glm::vec3 pos3;

		glm::vec3 col1;
		glm::vec3 col2;
		glm::vec3 col3;

		pos1 = mdata->getVertex(vindex1);
		pos2 = mdata->getVertex(vindex2);
		pos3 = mdata->getVertex(vindex3);

		col1 = mdata->getColor(vindex1);
		col2 = mdata->getColor(vindex2);
		col3 = mdata->getColor(vindex3);

		glm::vec3 nom = glm::triangleNormal(pos1, pos2, pos3);
		glNormal3f(nom.x, nom.y, nom.z);

		glNormal3f(nom.x, nom.y, nom.z);

		glColor3f(colorID.redf(), colorID.greenf(), colorID.bluef());
		glVertex3f(pos1.x, pos1.y, pos1.z);

		glColor3f(colorID.redf(), colorID.greenf(), colorID.bluef());
		glVertex3f(pos2.x, pos2.y, pos2.z);

		glColor3f(colorID.redf(), colorID.greenf(), colorID.bluef());
		glVertex3f(pos3.x, pos3.y, pos3.z);

		counter += 3;
	}
	glEnd();
	glEndList();

	cached = true;
}

void DfxObjectItem::drawMesh() {
	
	if (!cached) {
		cacheMesh();
	}	

	if (!isSelected()) {
		glCallList(displayListID);
	}
	else {
		glCallList(displayListSelID);
	}
	
	
}

void DfxObjectItem::draw() {

	if (!isSelected()) {
		drawMesh();
	}else {
		drawMesh();
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.5);
		drawMesh();
		glLineWidth(1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_LIGHTING);
	}
}

void DfxObjectItem::drawSelection() {

	glDisable(GL_LIGHTING);
	glDisable(GL_DITHER);
	glShadeModel(GL_FLAT);

	glCallList(displayListSelEvtID);

	glEnable(GL_LIGHTING);
	glEnable(GL_DITHER);
	glShadeModel(GL_FLAT);
}

void DfxObjectItem::drawSilhouette() {
	glShadeModel(GL_FLAT);
	glDisable(GL_LIGHTING);
	drawMesh();
	glEnable(GL_LIGHTING);
	glShadeModel(GL_FLAT);
}

Dfx3dMeshData *DfxObjectItem::getMeshData() {
	return (Dfx3dMeshData*)DfxEntity::getProperty(mesh);
}

void DfxObjectItem::initOpenGlBuffer() {

	initializeOpenGLFunctions();

	/*Dfx3dMeshData *msh = (Dfx3dMeshData*)DfxEntity::getProperty(mesh);

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, msh->getVertexBufferBytes(), msh->getVertexBuffer(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (char*)(sizeof(GL_FLOAT) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (char*)(sizeof(GL_FLOAT) * 6));

	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, msh->getIndexBufferBytes(), msh->getIndexBuffer(), GL_STATIC_DRAW);*/
}

void DfxObjectItem::bindOpenGlBuffers() {
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
}

bool DfxObjectItem::isReady() {
	return ready;
}

bool DfxObjectItem::isRenderable() {
	return true;
}

void DfxObjectItem::setIsReady(bool rd) {
	ready = rd;
}

void DfxObjectItem::calculateBounds() {
	Dfx3dMeshData *mdata = getMeshData();

	itemBounds.empty();

	glm::mat4 transMat = transformationMatrix();

	for (size_t inx = 0; inx < mdata->getVertexCount(); inx++) {
		glm::vec3 vert = glm::vec3(transMat * glm::vec4(mdata->getVertex(inx), 1.f));
		itemBounds.add(vert.x, vert.y, vert.z);
	}
}

void DfxObjectItem::compute() {
	syncTransforms();

	cleanAllProperties();
	getDataInstance()->makeClean();
	makeChildrenDirty();
}

DfxBounds DfxObjectItem::bounds() {
	return itemBounds;
}

bool DfxObjectItem::selectable() {
	return true;
}


DfxObjectDisplacementMap *DfxObjectItem::getDisplacementMap() {
	return displacementMap;
}

bool DfxObjectItem::renderDisplacement() {
	size_t mode = ((DfxEnum*)getProperty(inMode))->getCurrentIndex();

	if (mode == 1) {
		return true;
	}

	return false;
}

bool DfxObjectItem::ignoreLocalTransform() {

	if (((DfxEnum*)getProperty(inIgnoreLocalTransform))->getCurrentIndex() == 1) return true;

	return false;
}

glm::vec3 DfxObjectItem::getDisplacementVertex(size_t index) {

	Dfx3dMeshData *mdata = getMeshData();
	size_t dispIndex = *(float*)getProperty(inDisplacementIndex);

	auto disp = displacementMap->getDisplacement(dispIndex);

	if(disp == nullptr) disp = displacementMap->getDisplacement(dispIndex - 1);

	if (disp != nullptr) {
		if (index >= 0 && index < disp->getCount()) {
			return disp->getPositions()[index];
		}
		else {
			return glm::vec3(0.f);
		}
	}
	else {
		if (index >= 0 && index < mdata->getVertexCount() ) {
			return mdata->getVertex(index);
		}
		else {
			return glm::vec3(0.f);
		}
	}
}

std::string DfxObjectItem::getFilePath() {
	return *(std::string*)getProperty(inFilePath);
}

void DfxObjectItem::setFilePath(std::string path) {
	*(std::string*)getProperty(inFilePath) = path;
}

void DfxObjectItem::save(std::ofstream &) {

}

void DfxObjectItem::load(std::ifstream &) {

	*((size_t*)getProperty(item_id)) = getInstanceID();

	filesystem::path path(getFilePath());

	if (!path.has_extension()) {
		DfxGlobal::console::println("Failed to load object: Invalid path, unknown file, no extension found. " + getFilePath());
	}
	else {

		if (filesystem::exists(path)) {

			DfxString ext(path.extension().string().substr(1, path.extension().string().size() - 1).c_str());

			if (DfxObjectLoaderService::hasLoader(ext.c_str())) {

				DfxObjectLoaderService *objLoader = nullptr;

				objLoader = DfxObjectLoaderService::fetchLoader("obj");

				objLoader->setFilename(path.string());

				objLoader->loadObject(*getMeshData());

				if (objLoader->getStatus() == DfxObjectLoaderService::SUCCESS) {

					calculateBounds();

					initOpenGlBuffer();

					setIsReady(true);
				
					std::stringstream ss;
					ss << "loadObject '" << path.stem().string() << path.extension().string() << "' done...";
					DfxGlobal::console::println(ss.str());
				}
				else {
					delete objLoader;
				}

				delete objLoader;
			}
			else {
				DfxGlobal::console::println("No Loader found.");
			}
		}
		else {
			DfxGlobal::console::println("Failed to load object: file doesn't exist.");
		}
	}

}

void *DfxObjectItem::creator(DfxGlobalEntityName uniquename) {
	return new DfxObjectItem(uniquename);
}

void DfxObjectItem::initialize() {

	DfxViewItem::initialize();
	
	DfxEntityDB::createProperty(outPosition, "out_position", new DfxEntityPropertyTypes::Float3PropertyType());
	DfxEntityDB::createProperty(outRotation, "out_rotation", new DfxEntityPropertyTypes::Float3PropertyType());
	DfxEntityDB::createProperty(outScale, "out_scale", new DfxEntityPropertyTypes::Float3PropertyType());
	DfxEntityDB::createProperty(inMode, "render_mode", new DfxEntityPropertyTypes::DfxEnumPropertyType());
	DfxEntityDB::createProperty(inFilePath, "file_path", new DfxEntityPropertyTypes::FilePathPropertyType());
	DfxEntityDB::createProperty(inIgnoreLocalTransform, "ignore_transform", new DfxEntityPropertyTypes::DfxEnumPropertyType());
	DfxEntityDB::createProperty(inDisplacementIndex, "displacement_index", new DfxEntityPropertyTypes::FloatPropertyType());
	
	DfxEntityDB::createProperty(mesh, "@mesh_data", new DfxEntityPropertyTypes::Dfx3DMeshPropertyType());
}

void DfxObjectItem::syncTransforms() {

	DfxEntityPropertyTypes::Float3 _position = *(DfxEntityPropertyTypes::Float3*)getProperty(position);
	DfxEntityPropertyTypes::Float3 _rotation = *(DfxEntityPropertyTypes::Float3*)getProperty(rotation);
	DfxEntityPropertyTypes::Float3 _scale = *(DfxEntityPropertyTypes::Float3*)getProperty(scale);

	((DfxEntityPropertyTypes::Float3*)getProperty(outPosition))->x = _position.x;
	((DfxEntityPropertyTypes::Float3*)getProperty(outPosition))->y = _position.y;
	((DfxEntityPropertyTypes::Float3*)getProperty(outPosition))->z = _position.z;

	((DfxEntityPropertyTypes::Float3*)getProperty(outRotation))->x = _rotation.x;
	((DfxEntityPropertyTypes::Float3*)getProperty(outRotation))->y = _rotation.y;
	((DfxEntityPropertyTypes::Float3*)getProperty(outRotation))->z = _rotation.z;

	((DfxEntityPropertyTypes::Float3*)getProperty(outScale))->x = _scale.x;
	((DfxEntityPropertyTypes::Float3*)getProperty(outScale))->y = _scale.y;
	((DfxEntityPropertyTypes::Float3*)getProperty(outScale))->z = _scale.z;

}

bool DfxObjectItem::isFileReferenced() {
	return fileReferenced;
}

void DfxObjectItem::setFilerReferenced(bool fr) {
	fileReferenced = fr;
}