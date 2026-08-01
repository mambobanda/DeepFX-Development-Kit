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

#include "DfxSceneGraphSerializer.h"

DfxSceneGraphSerializer::DfxSceneGraphSerializer() {
	formatVersion = 1;
}

int DfxSceneGraphSerializer::getFormatVersion() {
	return formatVersion;
}

void DfxSceneGraphSerializer::setFormatVersion(int val) {
	formatVersion = val;
}

void DfxSceneGraphSerializer::serializeGraph(std::vector<DfxQBaseNode *> graph, const char* path) {
	/*using namespace rapidjson;

	StringBuffer s;
	Writer<StringBuffer> writer(s);

	writer.StartObject();
	writer.Key("format");
	writer.String("dfx_graph_fmt");
	writer.Key("version");
	writer.Uint(formatVersion);

	for (auto node : graph) {
		DfxEntity *ent = node->getEntity();

		if (ent->isExportable()) {

			writer.Key(ent->getLogicalName().c_str());
			writer.StartObject();

			writer.Key("global_id");
			writer.Uint64(ent->getGlobalID());
			writer.Key("instance_id");
			writer.Uint64(ent->getInstanceID());
			writer.Key("posX");
			writer.Int(node->pos().x());
			writer.Key("posY");
			writer.Int(node->pos().y());
			writer.Key("connections");
			writer.StartArray();
			
			auto _slots = node->getAllSlots();

			for (auto slt : _slots) {
				if (slt->getSlotType() == DfxQNodeSlotType::OUTPUT_SLOT) {

					for (size_t i = 0; i < slt->getAllConnections().size(); i++) {
						auto conn = slt->getAllConnections()[i];
						writer.StartObject();
						writer.Key("output");
						writer.Uint64(conn->slot1()->getPropertyID());
						writer.Key("target");
						writer.Uint(conn->slot2()->getParentEntity()->getInstanceID());
						writer.Key("input");
						writer.Uint64(conn->slot2()->getPropertyID());
						writer.EndObject();
					}
				}
			}

			writer.EndArray();

			auto data = ent->exportData();

			for (auto d : data) {
				writer.Key(d.first.c_str());
				writer.Key(d.second.c_str());
			}

			writer.EndObject();
		}
	}

	writer.EndObject();

	std::ofstream out(path);
	out << s.GetString();
	out.close();*/
}

void DfxSceneGraphSerializer::deserializeGraph(std::vector<DfxQBaseNode *> graph, const char* path) {
	//using namespace rapidjson;

	//QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	//std::string line;
	//std::stringstream ss;
	//std::ifstream in(path);

	//if (in.is_open()) {

	//	while (std::getline(in, line)) {
	//		ss << line;
	//	}

	//	in.close();
	//}

	//Document d;
	//d.Parse(ss.str().c_str());
	//std::stringstream ss2;
	//if (d.IsObject()) {

	//	if (!d.HasMember("format")) {
	//		DfxGlobal::console::println("Load Graph: Unknown File Format");
	//		return;
	//	}

	//	if (!d["format"].IsString()) {
	//		DfxGlobal::console::println("Load Graph: Unknown File Format");
	//		return;
	//	}

	//	if (std::string(d["format"].GetString()).compare("dfx_graph_fmt") != 0) {

	//		DfxGlobal::console::println("Load Graph: Incorrect File Format");
	//		return;
	//	}

	//	if (!d.HasMember("version")) {
	//		DfxGlobal::console::println("Load Graph: Unknown Format Version");
	//		return;
	//	}

	//	if (!d["version"].IsUint()) {
	//		DfxGlobal::console::println("Load Graph: Unknown Format Version");
	//		return;
	//	}

	//	if (d["version"].GetUint() != 1) {

	//		DfxGlobal::console::println("Load Graph: Incorrect Format Version");
	//		return;
	//	}

	//	for (Value::ConstMemberIterator itr = d.MemberBegin(); itr != d.MemberEnd(); ++itr) {

	//		if (itr->value.IsObject()) {

	//			ProxyNode pnode;
	//			pnode.ent = nullptr;

	//			if (itr->value.HasMember("global_id")) {

	//				if (itr->value["global_id"].IsUint64()) {

	//					pnode.ent = DfxEntityDB::newEntityInstance(itr->value["global_id"].GetUint64());
	//					pnode.instance_id = itr->value["instance_id"].GetUint64();
	//					pnode.x = itr->value["posX"].GetInt();
	//					pnode.y = itr->value["posY"].GetInt();

	//					/*auto conarr = itr->value["connections"].GetArray();
	//					
	//					for (size_t i = 0; i < conarr.Size(); i++) {
	//						
	//						ProxyConnection prx;
	//						prx.in = conarr[i]["input"].GetUint64();
	//						prx.target = conarr[i]["target"].GetUint64();
	//						prx.out = conarr[i]["output"].GetUint64();

	//						pnode.connections.push_back(prx);
	//					}*/
	//				}
	//			}

	//			if (pnode.ent != nullptr) {

	//				std::vector<std::pair<std::string, std::string>> properties;

	//				for (Value::ConstMemberIterator itr2 = itr->value.MemberBegin(); itr2 != itr->value.MemberEnd(); ++itr2) {

	//					/*std::string name = itr2->name.GetString();

	//					if ( name.compare("global_id") == 0 || name.compare("instance_id") == 0 || name.compare("posX") == 0 ||
	//						 name.compare("posY") == 0 || name.compare("connections") == 0 ) {
	//						continue;
	//					}

	//					std::pair<std::string, std::string> p(itr2->name.GetString(), itr2->value.GetString());
	//					properties.push_back(p);*/
	//				}

	//				//if (properties.size() > 0) pnode.ent->importData(properties);
	//				_proxies.push_back(pnode);
	//			}
	//			else {
	//				DfxGlobal::console::println("Entity Doesnt Exist");
	//			}
	//		}
	//	}

	//	
	//}

	//installProxies();
}

void DfxSceneGraphSerializer::installProxies() {

	//QGraphicsScene *scene = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getScene();

	//for (auto &pro : proxies) {
	//	pro.node = new DfxQNode(pro.ent);
	//	pro.node->setPos(pro.x, pro.y);
	//	scene->addItem(pro.node);
	//}

	//for (auto &pro : proxies) {

	//	for (int i = 0; i < pro.connections.size(); i++) {

	//		auto _conn = pro.connections.at(i);
	//		DfxQNodeSlot *slt01 = pro.node->findSlot(_conn.out);
	//		DfxQNodeSlot *slt02 = nullptr;

	//		for (size_t i = 0; i < proxies.size(); i++) {
	//			auto chd = proxies[i];
	//			if (chd.instance_id == _conn.target) {
	//				slt02 = chd.node->findSlot(_conn.in);
	//			}
	//		}

	//		if ( slt01 != nullptr && slt02 != nullptr) {

	//			if (slt01->getParentEntity()
	//				->getDataInstance()
	//				->getPropertyInstance(slt01->getPropertyID())->getState() == DfxEntityPropertyState::OUTPUT_STATE &&
	//				slt02->getParentEntity()
	//				->getDataInstance()
	//				->getPropertyInstance(slt02->getPropertyID())->getState() == DfxEntityPropertyState::INPUT_STATE &&
	//				slt01->getParentEntity()->getPropertyType(slt01->getPropertyID()) == 
	//				slt02->getParentEntity()->getPropertyType(slt02->getPropertyID())) {

	//				DfxQNodeConnection *conn = new DfxQNodeConnection(0);
	//				scene->addItem(conn);
	//				conn->setPort1(slt01);
	//				conn->setPos1(slt01->scenePos());
	//				conn->setPort2(slt02);
	//				conn->setPos2(slt02->scenePos());
	//				conn->updatePath();
	//				conn->bind();
	//			}
	//		}			
	//		
	//	}
	//}
}