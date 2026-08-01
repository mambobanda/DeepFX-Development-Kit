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
 /**
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <stack>
#include "SgAssetHandler.h"
#include "SgAssociation.h"
#include "SgVariantsHandler.h"
#include "SgAssetGroupHandler.h"
#include "SgAssetManager.h"
#include "SgStateIO.h"

namespace deepsg {

	class SgAssetState : public SgStateIO {
	private:
		static BlockReader root;
		static BlockReader asset;
		static BlockReader association;
		static BlockReader variants;
		static BlockReader group;
	private:
		// Asset Block Callbacks
		static void assetLoadObject(rapidjson::Value::Member &);
		static void assetLoadName(rapidjson::Value::Member &);
		static void assetLoadContext(rapidjson::Value::Member &);
		static void assetLoadKind(rapidjson::Value::Member &);
		static void assetLoadPath(rapidjson::Value::Member &);
		static void assetLoadAsset(rapidjson::Value::Member &);
		static void assetLoadAssociation(rapidjson::Value::Member &);
		static void assetLoadVariants(rapidjson::Value::Member &);
		static void assetLoadGroup(rapidjson::Value::Member &);

		// Association Block Callbacks
		static void associationLoadName(rapidjson::Value::Member &);
		static void associationLoadKind(rapidjson::Value::Member &);
		static void associationLoadSurface(rapidjson::Value::Member &);
		static void associationLoadPath(rapidjson::Value::Member &);

		// Variant Block Callbacks
		static void variantsLoadName(rapidjson::Value::Member &);
		static void variantsLoadKind(rapidjson::Value::Member &);
		static void variantsLoadAssociation(rapidjson::Value::Member &);

		// Variant Block Callbacks
		static void groupLoadName(rapidjson::Value::Member &);
		static void groupLoadAssociation(rapidjson::Value::Member &);

	public:

		SgAssetState();
		virtual ~SgAssetState();

		void saveAsset(SgAssetHandler &);
		void saveAssociation(SgAssociationHandler &);
		void saveVariant(SgVariantsHandler &);
		void saveGroup(SgAssetGroupHandler &);

		void save(std::string) override;
		void load(std::string) override;
	};

}