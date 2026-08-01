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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "VH5File.h"
#include "VResource.h"
#include "VBuffer.h"
#include "VH5Attribute.h"
#include "VH5Buffer.h"
#include "VH5OResource.h"

namespace xvault {

	namespace h5 {

		h5File::h5File() {
			_file = nullptr;	
			_attrStrType = new H5::StrType(H5::PredType::C_S1, 256);
		}

		h5File::~h5File() {
			close();
			delete _attrStrType;
		}

		bool h5File::isOpen() {
			return (_file != nullptr);
		}

		bool h5File::openGroup(std::string path, H5::Group &grp) {
			try
			{
				grp = _file->openGroup(path);
				return true;
			}  

			// catch failure caused by the H5File operations
			catch (H5::FileIException error) {
				_error = error.getDetailMsg();
				return false;
			}

			// catch failure caused by the DataSet operations
			catch (H5::DataSetIException error) {
				_error = error.getDetailMsg();
				return false;
			}

			// catch failure caused by the DataSpace operations
			catch (H5::DataSpaceIException error) {
				_error = error.getDetailMsg();
				return false;
			}

			return false;
		}

		bool h5File::open(std::string path, unsigned int iomode) {

			try
			{
				// Turn off the auto-printing when failure occurs so that we can
				// handle the errors appropriately
				H5::Exception::dontPrint();

				uint32_t access_mode = H5F_ACC_TRUNC;

				if (iomode == xvault::io::OPEN_READ_WRITE) access_mode = H5F_ACC_RDWR;
				else if (iomode == xvault::io::OPEN_READ_ONLY) access_mode = H5F_ACC_RDONLY;

				// Create a new file using the default property lists. 
				_file = new H5::H5File(path, access_mode);

				return true;

			}  // end of try block

			// catch failure caused by the H5File operations
			catch (H5::FileIException error) {
				_error = error.getDetailMsg();
				return false;
			}

			// catch failure caused by the DataSet operations
			catch (H5::DataSetIException error) {
				_error = error.getDetailMsg();
				return false;
			}

			// catch failure caused by the DataSpace operations
			catch (H5::DataSpaceIException error) {
				_error = error.getDetailMsg();
				return false;
			}

			return false;
		}

		bool h5File::close() {

			try
			{
				if (_file != nullptr) {
					delete _file;
					_file = nullptr;
				}

				return true;
			}

			catch (H5::FileIException error) {
				_error = error.getDetailMsg();
				return false;
			}

			// catch failure caused by the DataSet operations
			catch (H5::DataSetIException error) {
				_error = error.getDetailMsg();
				return false;
			}

			// catch failure caused by the DataSpace operations
			catch (H5::DataSpaceIException error) {
				_error = error.getDetailMsg();
				return false;
			}

			return false;
		}

		bool h5File::writeResource(std::string resource_path, oResource &res) {
		
			H5::Group group;
			bool validPath = false;
			filesystem::path path(resource_path);
			std::string accum = "";

			auto itr = path.begin();

			try {

				while (itr != path.end()) {

					if (itr->generic_string() == "/")
						accum += itr->generic_string();
					else
						accum += "/" + itr->generic_string();

					if (!_file->nameExists(accum)) _file->createGroup(accum);

					itr++;

					if (itr == path.end()) {
						validPath = true;
						group = _file->openGroup(resource_path);
					}
				
				}

				if (validPath) {

					for (int i = 0; i < res.attributeCount(); i++) {

						hsize_t dims[1]; // dataset dimensions
						dims[0] = 1;
						H5::DataSpace dataspace(1, dims);

						switch (res.attribute(i)->type()) {
						case type::CHAR:
						{

							char val[] = { res.attribute(i)->valueChar() };
							H5::Attribute myatt_in = group.createAttribute(res.attribute(i)->name(), H5::PredType::C_S1, dataspace);
							myatt_in.write(H5::PredType::C_S1, val);

						}
						break;
						case type::STRING:
						{
							const H5std_string strwritebuf(res.attribute(i)->valueString());

							H5::Attribute myatt_in = group.createAttribute(res.attribute(i)->name(), *_attrStrType, dataspace);
							myatt_in.write(*_attrStrType, strwritebuf);

						}
						break;
						case type::DOUBLE:
						{

							double val[] = { res.attribute(i)->valueDouble() };
							H5::Attribute myatt_in = group.createAttribute(res.attribute(i)->name(), H5::PredType::IEEE_F64BE, dataspace);
							myatt_in.write(H5::PredType::NATIVE_DOUBLE, val);

						}
						break;
						case type::FLOAT:
						{

							float val[] = { res.attribute(i)->valueFloat() };
							H5::Attribute myatt_in = group.createAttribute(res.attribute(i)->name(), H5::PredType::IEEE_F32BE, dataspace);
							myatt_in.write(H5::PredType::NATIVE_FLOAT, val);

						}
						break;
						case type::INT:
						{
							int val[] = { res.attribute(i)->valueInt() };
							H5::Attribute myatt_in = group.createAttribute(res.attribute(i)->name(), H5::PredType::STD_I32BE, dataspace);
							myatt_in.write(H5::PredType::NATIVE_INT, val);
						}
						break;
						case type::LONG:
						{
							long val[] = { res.attribute(i)->valueLong() };
							H5::Attribute myatt_in = group.createAttribute(res.attribute(i)->name(), H5::PredType::STD_I64BE, dataspace);
							myatt_in.write(H5::PredType::NATIVE_LONG, val);
						}
						break;
						case type::LONG_LONG:
						{
							long long val[] = { res.attribute(i)->valueLongLong() };
							H5::Attribute myatt_in = group.createAttribute(res.attribute(i)->name(), H5::PredType::STD_I64BE, dataspace);
							myatt_in.write(H5::PredType::NATIVE_LLONG, val);
						}
						break;
						case type::SHORT:
						{
							short val[] = { res.attribute(i)->valueShort() };
							H5::Attribute myatt_in = group.createAttribute(res.attribute(i)->name(), H5::PredType::STD_I32BE, dataspace);
							myatt_in.write(H5::PredType::NATIVE_SHORT, val);
						}
						break;
						case type::UNSIGNED_INT:
						{
							unsigned int val[] = { res.attribute(i)->valueUInt() };
							H5::Attribute myatt_in = group.createAttribute(res.attribute(i)->name(), H5::PredType::STD_U32BE, dataspace);
							myatt_in.write(H5::PredType::NATIVE_UINT, val);
						}
						break;
						case type::UNSIGNED_LONG:
						{
							unsigned long val[] = { res.attribute(i)->valueULong() };
							H5::Attribute myatt_in = group.createAttribute(res.attribute(i)->name(), H5::PredType::STD_U64BE, dataspace);
							myatt_in.write(H5::PredType::NATIVE_ULONG, val);
						}
						break;
						case type::UNSIGNED_LONG_LONG:
						{
							unsigned long long val[] = { res.attribute(i)->valueULongLong() };
							H5::Attribute myatt_in = group.createAttribute(res.attribute(i)->name(), H5::PredType::STD_U64BE, dataspace);
							myatt_in.write(H5::PredType::NATIVE_ULLONG, val);
						}
						break;
						case type::UNSIGNED_SHORT:
						{
							unsigned short val[] = { res.attribute(i)->valueUShort() };
							H5::Attribute myatt_in = group.createAttribute(res.attribute(i)->name(), H5::PredType::STD_U32BE, dataspace);
							myatt_in.write(H5::PredType::NATIVE_USHORT, val);
						}
						break;
						default:
							break;
						};
					}

					for (int i = 0; i < res.bufferCount(); i++) {

						hsize_t dims[1]; // dataset dimensions
						dims[0] = res.buffer(i)->bufferSize();
						H5::DataSpace dataspace(1, dims);

						switch (res.buffer(i)->type()) {
						case type::CHAR:
						{
							H5::DataSet dataset = group.createDataSet(res.buffer(i)->name(), H5::PredType::C_S1, dataspace);
							dataset.write(res.buffer(i)->bufferChar(), H5::PredType::C_S1);
						}
						break;
						case type::DOUBLE:
						{
							H5::DataSet dataset = group.createDataSet(res.buffer(i)->name(), H5::PredType::IEEE_F64BE, dataspace);
							dataset.write(res.buffer(i)->bufferDouble(), H5::PredType::NATIVE_DOUBLE);
						}
						break;
						case type::FLOAT:
						{
							H5::DataSet dataset = group.createDataSet(res.buffer(i)->name(), H5::PredType::IEEE_F32BE, dataspace);
							dataset.write(res.buffer(i)->bufferFloat(), H5::PredType::NATIVE_FLOAT);
						}
						break;
						case type::INT:
						{
							H5::DataSet dataset = group.createDataSet(res.buffer(i)->name(), H5::PredType::STD_I32BE, dataspace);
							dataset.write(res.buffer(i)->bufferInt(), H5::PredType::NATIVE_INT);
						}
						break;
						case type::LONG:
						{
							H5::DataSet dataset = group.createDataSet(res.buffer(i)->name(), H5::PredType::STD_I64BE, dataspace);
							dataset.write(res.buffer(i)->bufferLong(), H5::PredType::NATIVE_LONG);
						}
						break;
						case type::LONG_LONG:
						{
							H5::DataSet dataset = group.createDataSet(res.buffer(i)->name(), H5::PredType::STD_I64BE, dataspace);
							dataset.write(res.buffer(i)->bufferLongLong(), H5::PredType::NATIVE_LLONG);
						}
						break;
						case type::SHORT:
						{
							H5::DataSet dataset = group.createDataSet(res.buffer(i)->name(), H5::PredType::STD_I32BE, dataspace);
							dataset.write(res.buffer(i)->bufferShort(), H5::PredType::NATIVE_SHORT);
						}
						break;
						case type::UNSIGNED_INT:
						{
							H5::DataSet dataset = group.createDataSet(res.buffer(i)->name(), H5::PredType::STD_U32BE, dataspace);
							dataset.write(res.buffer(i)->bufferUInt(), H5::PredType::NATIVE_UINT);
						}
						break;
						case type::UNSIGNED_LONG:
						{
							H5::DataSet dataset = group.createDataSet(res.buffer(i)->name(), H5::PredType::STD_U64BE, dataspace);
							dataset.write(res.buffer(i)->bufferULong(), H5::PredType::NATIVE_ULONG);
						}
						break;
						case type::UNSIGNED_LONG_LONG:
						{
							H5::DataSet dataset = group.createDataSet(res.buffer(i)->name(), H5::PredType::STD_U64BE, dataspace);
							dataset.write(res.buffer(i)->bufferULongLong(), H5::PredType::NATIVE_ULLONG);
						}
						break;
						case type::UNSIGNED_SHORT:
						{
							H5::DataSet dataset = group.createDataSet(res.buffer(i)->name(), H5::PredType::STD_U32BE, dataspace);
							dataset.write(res.buffer(i)->bufferUShort(), H5::PredType::NATIVE_USHORT);
						}
						break;
						default:
							break;
						};

					}
				}

				return true;
			}

			catch (H5::FileIException error) {
				_error = error.getDetailMsg();
				return false;
			}

			// catch failure caused by the DataSet operations
			catch (H5::DataSetIException error) {
				_error = error.getDetailMsg();
				return false;
			}

			// catch failure caused by the DataSpace operations
			catch (H5::DataSpaceIException error) {
				_error = error.getDetailMsg();
				return false;
			}

			return false;
		}

		bool h5File::readResource(std::string resource_path, iResource &rsc) {

			try {

				if (_file->nameExists(resource_path)) {

					auto group = _file->openGroup(resource_path);

					if (group.attrExists("__content__")) {

						// iterateAttrs callback signature
						// void(*attr_operator_t)(H5Object& loc, const H5std_string attr_name, void *operator_data);

						filesystem::path path(group.getObjName());
						auto group_name = path.stem().generic_string() + path.extension().generic_string();

						std::vector<Attribute*> attrArray;
						std::vector<Buffer*> buffArray;

						for (int i = 0; i < group.getNumObjs(); i++) {
							auto obj_type = group.getObjTypeByIdx(i);

							if (obj_type == H5G_DATASET) {
								auto dataset = group.openDataSet(group.getObjnameByIdx(i));

								auto _class = dataset.getTypeClass();
								if (_class == H5T_INTEGER) {

									hsize_t *extents = nullptr;
									int dim = dataset.getSpace().getSimpleExtentNdims();
									extents = new hsize_t[dim];

									dataset.getSpace().getSimpleExtentDims(extents);
									
									auto nbuff = new h5::h5Buffer();
									filesystem::path path(dataset.getObjName());
									auto dataset_name = path.stem().generic_string() + path.extension().generic_string();

									nbuff->setName(dataset_name);
									nbuff->setBufferSize(extents[0]);

									if (dataset.getIntType().getSign() == H5T_sign_t::H5T_SGN_NONE) {
										if (dataset.getIntType().getSize() == 4) {
											nbuff->setType(type::UNSIGNED_INT);
											unsigned int *b = new unsigned int[extents[0]];
											dataset.read(b, H5::PredType::NATIVE_UINT);

											nbuff->setBufferUInt(b);
											buffArray.push_back(nbuff);
										}
										else if (dataset.getIntType().getSize() == 8) {
											nbuff->setType(type::UNSIGNED_LONG_LONG);
											unsigned long long *b = new unsigned long long[extents[0]];
											dataset.read(b, H5::PredType::NATIVE_ULLONG);

											nbuff->setBufferULongLong(b);
											buffArray.push_back(nbuff);
										}
									}
									else {
										if (dataset.getIntType().getSize() == 4) {
											nbuff->setType(type::INT);
											int *b = new int[extents[0]];
											dataset.read(b, H5::PredType::NATIVE_INT);

											nbuff->setBufferInt(b);
											buffArray.push_back(nbuff);
										}
										else if (dataset.getIntType().getSize() == 8) {
											nbuff->setType(type::LONG_LONG);
											long long *b = new long long[extents[0]];
											dataset.read(b, H5::PredType::NATIVE_LLONG);

											nbuff->setBufferLongLong(b);
											buffArray.push_back(nbuff);
										}
									}

									delete[] extents;
								}
								else if (_class == H5T_FLOAT) {

									hsize_t *extents = nullptr;
									int dim = dataset.getSpace().getSimpleExtentNdims();
									extents = new hsize_t[dim];

									dataset.getSpace().getSimpleExtentDims(extents);

									auto nbuff = new h5::h5Buffer();
									filesystem::path path(dataset.getObjName());
									auto dataset_name = path.stem().generic_string() + path.extension().generic_string();

									nbuff->setName(dataset_name);
									nbuff->setBufferSize(extents[0]);

									nbuff->setType(type::FLOAT);
									float *b = new float[extents[0]];
									dataset.read(b, H5::PredType::NATIVE_FLOAT);

									nbuff->setBufferFloat(b);
									buffArray.push_back(nbuff);
								}
								else if (_class == H5T_STRING) {
									
									hsize_t *extents = nullptr;
									int dim = dataset.getSpace().getSimpleExtentNdims();
									extents = new hsize_t[dim];

									dataset.getSpace().getSimpleExtentDims(extents);

									auto nbuff = new h5::h5Buffer();
									filesystem::path path(dataset.getObjName());
									auto dataset_name = path.stem().generic_string() + path.extension().generic_string();

									nbuff->setName(dataset_name);
									nbuff->setBufferSize(extents[0]);

									nbuff->setType(type::CHAR);
									char *b = new char[extents[0]];
									dataset.read(b, H5::PredType::C_S1);

									nbuff->setBufferChar(b);
									buffArray.push_back(nbuff);
								}
							}
						}

						group.iterateAttrs([](H5::H5Object& loc, const H5std_string attr_name, void *operator_data) {

							auto h_attr = loc.openAttribute(attr_name);
							auto arr = (std::vector<Attribute*>*)operator_data;

							auto nattr = new h5::h5Attribute;
							nattr->setName(attr_name);
							arr->push_back(nattr);

							int dataType = -1;

							switch (h_attr.getTypeClass()) {
							case H5T_INTEGER:
							{
								auto intType = h_attr.getIntType();

								if (intType.getSign() == H5T_sign_t::H5T_SGN_NONE) {

									if (intType.getSize() == 4) {
										unsigned int val[] = { 0 };
										h_attr.read(H5::PredType::NATIVE_UINT, val);
										nattr->setType(type::UNSIGNED_INT);
										nattr->setValueUInt(val[0]);
									}
									else if (intType.getSize() == 8) {
										unsigned long long val[] = { 0 };
										h_attr.read(H5::PredType::NATIVE_ULLONG, val);
										nattr->setType(type::UNSIGNED_LONG_LONG);
										nattr->setValueULongLong(val[0]);
									}

								}
								else {

									if (intType.getSize() == 4) {
										int val[] = { 0 };
										h_attr.read(H5::PredType::NATIVE_INT, val);
										nattr->setType(type::INT);
										nattr->setValueInt(val[0]);
									}
									else if (intType.getSize() == 8) {
										long long val[] = { 0 };
										h_attr.read(H5::PredType::NATIVE_LLONG, val);
										nattr->setType(type::LONG_LONG);
										nattr->setValueLongLong(val[0]);
									}
								}
							}
							break;
							case H5T_FLOAT:
							{
								auto floatType = h_attr.getIntType();

								if (floatType.getSize() == 4) {
									float val[] = { 0 };
									h_attr.read(H5::PredType::NATIVE_FLOAT, val);
									nattr->setType(type::FLOAT);
									nattr->setValueFloat(val[0]);
								}
								else if (floatType.getSize() == 8) {
									double val[] = { 0 };
									h_attr.read(H5::PredType::NATIVE_DOUBLE, val);
									nattr->setType(type::DOUBLE);
									nattr->setValueDouble(val[0]);
								}

							}
							break;
							case H5T_STRING:
							{
								H5::StrType strType(H5::PredType::C_S1, 256);
								std::string temp;
								h_attr.read(strType, temp);
								nattr->setType(type::STRING);
								nattr->setValueString(temp);
							}
							break;
							default:
								break;
							};

						}, 0, &attrArray);

						for (auto i : attrArray) rsc.addAttribute(i);
						for (auto i : buffArray) rsc.addBuffer(i);

						return true;
					}
				}

			}

			catch (H5::FileIException error) {
				_error = error.getDetailMsg();
				return false;
			}

			// catch failure caused by the DataSet operations
			catch (H5::DataSetIException error) {
				_error = error.getDetailMsg();
				return false;
			}

			// catch failure caused by the DataSpace operations
			catch (H5::DataSpaceIException error) {
				_error = error.getDetailMsg();
				return false;
			}

			return false;
		}

		void  h5File::walkFile(H5::Group &group, std::string path, WalkCallback callback, void *userdata) {
			for (int i = 0; i < group.getNumObjs(); i++) {

				auto obj_type = group.getObjTypeByIdx(i);

				if (obj_type == H5G_GROUP) {

					auto child_group = group.openGroup(group.getObjnameByIdx(i));

					auto resource = new h5::h5IResource;
					auto group_name = group.getObjnameByIdx(i);
					auto group_full_path = path + "/" + group_name;

					if (readResource(group_full_path, *resource)) {
						callback(group_full_path, resource, userdata);
					}
					else {
						walkFile(child_group, group_full_path, callback, userdata);
					}

					delete resource;
				}

			}
		}

		void h5File::walkFile(WalkCallback callback, void *userdata) {
			auto root = _file->openGroup("/");
			walkFile(root, "/", callback, userdata);
		}

		std::string h5File::errorMsg() {
			return _error;
		}
	}
}