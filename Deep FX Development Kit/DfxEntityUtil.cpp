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

#include "DfxEntityUtil.h"

bool entityHasProperty(std::string pro_name, DfxEntity *entity, size_t &propid) {

	using namespace DfxEntityPropertyTypes;

	DfxEntityDataInstance *data = entity->getDataInstance();
	std::vector<size_t> lst = entity->getDataInstance()->getPropertyList();

	for (auto id : lst) {

		if ( data->getPropertyInstance(id)->getName().compare(pro_name) == 0 ) {

			propid = id;
			return true;
		}
	}

	return false;
}

std::vector < std::pair<std::string, std::string> > mapEntityProperties(DfxEntity *ent) {

	using namespace DfxEntityPropertyTypes;

	DfxEntityDataInstance *data = ent->getDataInstance();
	std::vector<size_t> lst = ent->getDataInstance()->getPropertyList();

	std::vector < std::pair<std::string, std::string> > map;

	for (auto id : lst) {

		if (data->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iFLOAT_TYPE) {

			std::pair<std::string, std::string> p;
			p.first  = data->getPropertyInstance(id)->getName();
			p.second = QString::number(*(float*)data->getPropertyInstance(id)->getData()).toUtf8().toStdString();
			map.push_back(p);
		}
		else if (data->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iFLOAT3_TYPE) {

			std::stringstream ss;
			ss << ((Float3*)data->getPropertyInstance(id)->getData())->x << "/";
			ss << ((Float3*)data->getPropertyInstance(id)->getData())->y << "/";
			ss << ((Float3*)data->getPropertyInstance(id)->getData())->z;	

			std::pair<std::string, std::string> p;
			p.first  = data->getPropertyInstance(id)->getName();
			p.second = ss.str();
			map.push_back(p);
		}
		else if (data->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iSIZE_T_TYPE) {

			std::pair<std::string, std::string> p;
			p.first = data->getPropertyInstance(id)->getName();
			p.second = QString::number(*(size_t*)data->getPropertyInstance(id)->getData()).toUtf8().toStdString();
			map.push_back(p);
		}
		else if (data->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iENUM_TYPE) {
			
			std::pair<std::string, std::string> p;
			p.first  = data->getPropertyInstance(id)->getName();
			p.second = QString::number(((DfxEnum*)data->getPropertyInstance(id)->getData())->getCurrentIndex()).toUtf8().toStdString();
			map.push_back(p);
		}
		else if (data->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iFILE_PATH_TYPE) {

			std::pair<std::string, std::string> p;
			p.first  = data->getPropertyInstance(id)->getName();
			p.second = *(std::string*)data->getPropertyInstance(id)->getData();
			std::replace(p.second.begin(), p.second.end(), ':', '~');
			map.push_back(p);
		}
		else if (data->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iSTRING_TYPE) {

			std::pair<std::string, std::string> p;
			p.first = data->getPropertyInstance(id)->getName();
			p.second = *(std::string*)data->getPropertyInstance(id)->getData();
			std::replace(p.second.begin(), p.second.end(), ':', '~');
			map.push_back(p);
		}

	}

	return map;
}

void mapEntityProperties(DfxEntity *entity, std::vector < std::pair<std::string, std::string>> &properties) {

	using namespace DfxEntityPropertyTypes;

	DfxEntityDataInstance *data = entity->getDataInstance();
	std::vector<size_t> lst = entity->getDataInstance()->getPropertyList();

	size_t id = -1;

	for (auto prop: properties) {

		bool hasProperty = entityHasProperty(prop.first, entity, id);

		if (hasProperty) {

			if (data->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iFLOAT_TYPE) {			

				try {
					*(float*)entity->getProperty(id) = std::stof(prop.second);
				}
				catch (const std::invalid_argument &ia) {
					DfxGlobal::console::println("Open Scene Error: property \'float\' invalid argument.");
				}
			}
			else if (data->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iFLOAT3_TYPE) {

				try {
					QStringList slist = QString(prop.second.c_str()).split('/');

					if (slist.size() >= 3) {
						((Float3*)entity->getProperty(id))->x = std::stof(slist[0].toStdString());
						((Float3*)entity->getProperty(id))->y = std::stof(slist[1].toStdString());
						((Float3*)entity->getProperty(id))->z = std::stof(slist[2].toStdString());
					}
					else {
						((Float3*)entity->getProperty(id))->x = 0.0f;
						((Float3*)entity->getProperty(id))->y = 0.0f;
						((Float3*)entity->getProperty(id))->z = 0.0f;
					}
				}
				catch (const std::invalid_argument &ia) {
					DfxGlobal::console::println("Open Scene Error: property \'float3\' invalid argument.");
				}

			}
			else if (data->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iSIZE_T_TYPE) {

				try {
					*(size_t*)entity->getProperty(id) = std::stoull(prop.second);
				}
				catch (const std::invalid_argument &ia) {
					DfxGlobal::console::println("Open Scene Error: property \'size_t\' invalid argument.");
				}
			}
			else if (data->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iENUM_TYPE) {

				try {
					size_t inx = std::stoull(prop.second);
					((DfxEnum*)entity->getProperty(id))->setCurrentIndex(inx);
				}
				catch (const std::invalid_argument &ia) {
					DfxGlobal::console::println("Open Scene Error: property \'enum\' invalid argument.");
				}
			}
			else if (data->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iFILE_PATH_TYPE) {

				try {
					std::string val = prop.second;
					std::replace(val.begin(), val.end(), '~', ':');
					*(std::string*)entity->getProperty(id) = val;
				}
				catch (const std::invalid_argument &ia) {
					DfxGlobal::console::println("Open Scene Error: property \'filepath\' invalid argument.");
				}
			}
			else if (data->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iSTRING_TYPE) {

				try {
					std::string val = prop.second;
					std::replace(val.begin(), val.end(), '~', ':');
					*(std::string*)entity->getProperty(id) = val;
				}
				catch (const std::invalid_argument &ia) {
					DfxGlobal::console::println("Open Scene Error: property \'string\' invalid argument.");
				}
			}
		}
	}
}