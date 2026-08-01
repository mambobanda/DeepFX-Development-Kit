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


#include "DfxSceneGraphNodeFunctions.h"
#include "DfxSceneGraph.h"

// IO
void DfxSceneGraphNodeFunctions::saveGraph() {

	DfxSceneGraphSerializer save;

	QString fileName 
		= QFileDialog::getSaveFileName(nullptr, QString("Save Graph"), "C:/test", QString("Files (*.sgraph)"));

	/*save.serializeGraph(DfxSceneGraph::getInstance()->getNodeList(), 
						fileName.toUtf8().toStdString().c_str());*/
}

void DfxSceneGraphNodeFunctions::loadGraph() {

	DfxSceneGraphSerializer load;

	QString fileName
		= QFileDialog::getOpenFileName(nullptr, QString("Load Graph"), "C:/test", QString("Files (*.sgraph)"));

	/*load.deserializeGraph(DfxSceneGraph::getInstance()->getNodeList(),
		fileName.toUtf8().toStdString().c_str());*/
}

// Math Nodes
void DfxSceneGraphNodeFunctions::addSCountNode() {

	DfxEntity *node = new DfxNodes::sCountNode(DfxNodes::sCountNode_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

void DfxSceneGraphNodeFunctions::addSUniformArrayNode() {

	DfxEntity *node = new DfxNodes::sUniformArrayNode(DfxNodes::sUniformArrayNode_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

void DfxSceneGraphNodeFunctions::addVCountNode() {

	DfxEntity *node = new DfxNodes::vCountNode(DfxNodes::vCountNode_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

void DfxSceneGraphNodeFunctions::addVUniformArrayNode() {

	DfxEntity *node = new DfxNodes::vUniformArrayNode(DfxNodes::vUniformArrayNode_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}


void DfxSceneGraphNodeFunctions::addSValueNode() {

	DfxEntity *node = new DfxNodes::sValueNode(DfxNodes::sValueNode_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

void DfxSceneGraphNodeFunctions::addSAddNode() {

	DfxEntity *node = new DfxNodes::sAddNode(DfxNodes::sAddNode_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

void DfxSceneGraphNodeFunctions::addSSubtractNode() {

	DfxEntity *node = new DfxNodes::sSubtractNode(DfxNodes::sSubtractNode_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

void DfxSceneGraphNodeFunctions::addSMultiplyNode() {

	DfxEntity *node = new DfxNodes::sMultiplyNode(DfxNodes::sMultiplyNode_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

void DfxSceneGraphNodeFunctions::addSDivideNode() {

	DfxEntity *node = new DfxNodes::sDivideNode(DfxNodes::sDivideNode_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

void DfxSceneGraphNodeFunctions::addVValueNode() {

	DfxEntity *node = new DfxNodes::vValueNode(DfxNodes::vValueNode_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

void DfxSceneGraphNodeFunctions::addVAddNode() {

	DfxEntity *node = new DfxNodes::sDivideNode(DfxNodes::vValueNode_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

// Point Nodes
void DfxSceneGraphNodeFunctions::addMeshVoxelizerNode() {
	DfxEntity *node = new dfxcore::MeshVoxelizer(dfxcore::MeshVoxelizer_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

void DfxSceneGraphNodeFunctions::addPointViewNode() {
	DfxGlobal::command::exec("addPointView");
}

void DfxSceneGraphNodeFunctions::addPointGridNode() {
	DfxEntity *node = new dfxcore::DfxPointGrid(dfxcore::DfxPointGrid_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

// Time Nodes
void DfxSceneGraphNodeFunctions::addTimeInfoNode() {
	DfxEntity *node = new DfxNodes::TimeInfo(DfxNodes::TimeInfo_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

// Item Nodes
void DfxSceneGraphNodeFunctions::addObjectInfoNode() {
	DfxEntity *node = new DfxObjectItemInfo(DfxObjectItemInfo_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

void DfxSceneGraphNodeFunctions::addMapUniformDisplacementNode() {
	DfxEntity *node = new DfxMapUniformDisplacement(DfxMapUniformDisplacement_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

void DfxSceneGraphNodeFunctions::addMapUniformDisplacementProxyNode() {
	DfxEntity *node = new DfxMapUniformDisplacementProxy(DfxMapUniformDisplacementProxy_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

void DfxSceneGraphNodeFunctions::addSpotterNode() {
	
	DfxGlobal::command::exec("addSpotter");
	DfxGlobal::command::exec("updateViewport");
}

void DfxSceneGraphNodeFunctions::addCopyObjectNode() {

	DfxEntity *node = new DfxObjectItemCopy(DfxObjectItemCopy_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

void DfxSceneGraphNodeFunctions::addObjectProxyViewerNode() {
	DfxGlobal::command::exec("addObjectProxyViewer");
}

// Simulation Nodes
void DfxSceneGraphNodeFunctions::addDFSPHNode() {
	DfxEntity *node = new dfxcore::sph::DivergenceFreeSPHSolver(dfxcore::sph::DivergenceFreeSPHSolver_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

void DfxSceneGraphNodeFunctions::addSphRigidNode() {
	DfxEntity *node = new dfxcore::sph::SPHRigid(dfxcore::sph::SPHRigid_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

void DfxSceneGraphNodeFunctions::addSphLiquidNode() {
	DfxEntity *node = new dfxcore::sph::SPHLiquid(dfxcore::sph::SPHLiquid_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

void DfxSceneGraphNodeFunctions::addSphParticleCacheNode() {

	DfxEntity *node = new dfxcore::sph::SPHParticleCache(dfxcore::sph::SPHParticleCache_Identifier);

	/*QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);*/
}

void DfxSceneGraphNodeFunctions::addPointXFormNode() {

	DfxEntity *node = new dfxcore::PointXForm(dfxcore::PointXForm_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

void DfxSceneGraphNodeFunctions::addBulletSolver() {
	
	DfxEntity *node = new dfxcore::bullet::DfxCBulletSolver(dfxcore::bullet::DfxCBulletSolver_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

void DfxSceneGraphNodeFunctions::addBulletRigidObject() {
	 
	DfxEntity *node = new dfxcore::bullet::DfxCBulletRigidObject(dfxcore::bullet::DfxCBulletRigidObject_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}

void DfxSceneGraphNodeFunctions::addBulletRigidArray() {

	DfxEntity *node = new dfxcore::bullet::DfxCBulletRigidArray(dfxcore::bullet::DfxCBulletRigidArray_Identifier);

	QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	DfxQBaseNode *qnode = new DfxQStandardNode(node);
	scene->addItem(qnode);
}