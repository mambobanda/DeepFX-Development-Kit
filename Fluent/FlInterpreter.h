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

#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <iostream>
#include <queue>
#include <functional>
#include "FlUtil.h"
#include "FlCommon.h"
#include "FlInterpreterScope.h"
#include "FlAny.h"
#include "FlSpace.h"
#include "FlFlow.h"
#include "FlDirectedAcyclicGraph.h"
#include "FlAnyHelpers.h"

namespace fl {

	using IteratorOutputProcessor = std::function<void(DataObject *dao, fl::Any *, std::vector<fl::Any*> *, size_t)>;

	class Interpreter: public InterpreterScope {
	private:

		class IteratorOutputType { public: enum { SAME_AS_INPUT = 0, BOOLEAN = 1 }; };

		Space *_space;
		Flow  *_flow;

		bool printDebugInfo;

		void runStart(Flow *start);
		void runProcess(Flow *process);
		bool runDecision(Flow *decision);
		void runBreak(Flow *brk);

		void executeGraph(DataObject *dao, Flow *flow = nullptr);
		fl::Any *processReturnValues(fl::DataObject *dao, std::vector<fl::Any*> &returnValues, size_t outputRefPath = 0, bool returnFirstOutptValue = false,
			std::unordered_map<std::string, fl::Any*> *outputAttributeLookup = nullptr, bool useOutputLookup = false);

		void executeStaticMethod(std::string name, DataObject *dao, fl::Space *_space, fl::InterpreterScope *scope);
		void executeNormalStaticMethod(std::string name, DataObject *dao, fl::Space *_space, fl::InterpreterScope *scope);

		void executeIterationStaticMethod(std::string name, DataObject *dao, fl::Space *_space, fl::InterpreterScope *scope, size_t iterationType, IteratorOutputProcessor itopCallback, size_t outputType);

		void executeReduceIterationStaticMethod(std::string name, DataObject *dao, fl::Space *_space, fl::InterpreterScope *scope, size_t iterationType, size_t outputType);

		bool validIterationOutputSignature(fl::DataObject *dao, fl::Space *_space, std::vector<std::pair<std::string, std::string>> refs, size_t iterationType, size_t outputType);

		static void mapIteratorOutputCallback(DataObject *dao, fl::Any *, std::vector<fl::Any*> *, size_t);

		static void filterIteratorOutputCallback(DataObject *dao, fl::Any *, std::vector<fl::Any*> *, size_t);

		static void peekIteratorOutputCallback(DataObject *dao, fl::Any *, std::vector<fl::Any*> *, size_t);

		static void partitionIteratorOutputCallback(DataObject *dao, fl::Any *, std::vector<fl::Any*> *, size_t);

		void buildDAG(DataObject *, DirectedAcyclicGraph<size_t, DataObject> *graph, Flow *flow= nullptr);
		void deleteDAG(DirectedAcyclicGraph<size_t, DataObject> *graph);
	private:
		void printDebugInfoStart(Flow *start);
		void printDebugInfoProcess(Flow *process);
		void printDebugInfoDecision(Flow *decision);
		void printDebugInfoBreak(Flow *brk);

	public:

		Interpreter(Space *space,  Flow *flow);
		~Interpreter();

		void run();

		void setPrintDebugInfo(bool);

	private:

		template<typename T>
		fl::Any *processIteration(long long int inx, bool pass_index, uint32_t arrayType, std::vector<fl::Any*> args, std::vector<T> *vArray,
			std::vector<fl::DirectedAcyclicGraph<size_t, fl::DataObject>::Vertex*> &vList, 
			std::vector<std::vector<fl::Any*>> &t_argsArray, std::unordered_map<std::string, fl::Any*> &outputAttributeLookup,
			size_t outReference, std::string name, fl::Space *_space, std::vector<fl::Any*> &tempAttributes) {
		
			fl::Any *outValue = nullptr;

			int counter = 0;	
			for (auto v : vList) {

				std::vector<fl::Any*> t_returnValues;
				std::vector<fl::Any*> inputArguments;
				inputArguments.resize(t_argsArray[counter].size());
				for (size_t inj = 0; inj < t_argsArray[counter].size(); inj++) {
					if (t_argsArray[counter][inj]->miscFlag() == -100) {
						auto arrayValAny = new fl::Any;
						arrayValAny->newData<T>(arrayType);
						arrayValAny->setData<T>((*vArray)[inx]);
						inputArguments[inj] = arrayValAny;

						tempAttributes.push_back(arrayValAny);
					}else if (t_argsArray[counter][inj]->miscFlag() == -200) {
						auto arrayIndexAny = new fl::Any;
						arrayIndexAny->newData<float>(fl::type::FLOAT);
						arrayIndexAny->setData<float>((float)inx);
						inputArguments[inj] = arrayIndexAny;

						tempAttributes.push_back(arrayIndexAny);
					}
					else {
						inputArguments[inj] = t_argsArray[counter][inj];
					}
				}
				
				v->staticMethod(_space, v->userData, inputArguments, t_returnValues);
						
				outValue = processReturnValues(v->userData, t_returnValues, outReference, true, &outputAttributeLookup, true);

				counter++;
			}

			return outValue;
		}

		template<typename T>
		void processIterations(bool parallel, bool pass_index, uint32_t arrayType,
			std::vector<fl::Any*> &args,
			std::vector<fl::DirectedAcyclicGraph<size_t, fl::DataObject>::Vertex*> &vlist,
			std::unordered_map <std::string, std::pair<std::string, fl::Any*>> &attributeLookup,
			std::vector<std::pair<std::string, std::string>> &inputReferences,
			size_t outReference, std::string name, fl::Space *_space, std::vector<fl::Any*> *xArray) {

			std::unordered_map<std::string, fl::Any*> outputAttributeLookup;
			std::vector<fl::Any*> tempAttributes;
			std::unordered_map<std::string, fl::Any*> argumentDependencyMap;

			std::vector<std::vector<fl::Any*>> t_argsArray;

			auto vArray = args[1]->rawData<std::vector<T>>();

			if (!vArray->empty()) {

				auto arrayValAny = new fl::Any;
				auto arrayIndexAny = new fl::Any;
				
				arrayValAny->newData<T>(arrayType);
				arrayValAny->setMiscFlag(-100);

				arrayIndexAny->newData<float>(fl::type::FLOAT);
				arrayIndexAny->setMiscFlag(-200);

				tempAttributes.push_back(arrayValAny);
				tempAttributes.push_back(arrayIndexAny);

				fl::Any *outValue = nullptr;

				size_t argCounter = 4;
				for (size_t inj = 0; inj < inputReferences.size(); inj++) {
					if (inj == 0) {
						argumentDependencyMap[inputReferences[inj].first] = arrayValAny;
					}
					else if (inj == 1 && pass_index) {	
						argumentDependencyMap[inputReferences[inj].first] = arrayIndexAny;
					}
					else {
						if (argCounter < args.size()) {
							argumentDependencyMap[inputReferences[inj].first] = args[argCounter];
							argCounter++;
						}
					}
				}

				for (auto v : vlist) {
					for (auto attr : v->userData->propertiesOriginalOrder()) {
						if (attr->role() == fl::role::OUTPUT) {
							auto tany = new fl::Any;
							fl::copy(attr, tany);
							outputAttributeLookup[attr->absolutePath()] = tany;
						}
					}
				}

				for (auto v : vlist) {

					std::vector<fl::Any*> t_args;
					std::vector<fl::Any*> t_returnValues;

					for (auto attr : v->userData->propertiesOriginalOrder()) {
						if (attr->role() == fl::role::INPUT) {

							bool hasDependency = false;

							for (size_t inj = 0; inj < inputReferences.size(); inj++) {
								auto itr = argumentDependencyMap.find(attr->absolutePath());

								if (itr != argumentDependencyMap.end()) {
									t_args.push_back(itr->second);
									hasDependency = true;
									break;
								}
							}

							if (!hasDependency) {

								auto itr00 = attributeLookup.find(attr->absolutePath());

								if (itr00 != attributeLookup.end()) {
									auto itr01 = outputAttributeLookup.find(itr00->second.first);

									if (itr01 != outputAttributeLookup.end()) {
										t_args.push_back(itr01->second);
									}
									else {
										t_args.push_back(itr00->second.second);
									}

								}
							}
						}
					}

					t_argsArray.push_back(t_args);
				}

				if(xArray != nullptr) xArray->resize(vArray->size(), nullptr);

				if (!parallel) {
					util::xTimer process_timer;
					process_timer.startTimer();

					for (long long int inx = 0; inx < (long long int)vArray->size(); inx++) {
						auto _ovl = processIteration<T>(inx, pass_index, arrayType, args, vArray, vlist, t_argsArray, outputAttributeLookup, outReference, name, _space, tempAttributes);
						if(xArray != nullptr) (*xArray)[inx] = _ovl;
					}

					process_timer.stopTimer();
					std::cout << "\nGlobal Process Iteration time: " << process_timer.getTime() << "ms"<< std::endl;
				}
				else {

					#pragma omp parallel default(shared)
					{
						#pragma omp for schedule(static)
						for (long long int inx = 0; inx < (long long int)vArray->size(); inx++) {
							auto _ovl = processIteration<T>(inx, pass_index, arrayType, args, vArray, vlist, t_argsArray, outputAttributeLookup, outReference, name, _space, tempAttributes);
							if (xArray != nullptr) (*xArray)[inx] = _ovl;
						}
					}
				}

				for (auto temp : tempAttributes) {
					fl::clear(temp);
					delete temp;
				}

				for (auto p : outputAttributeLookup) {
					fl::clear(p.second);
					delete p.second;
				}
			}

		}

		template<typename T>
		fl::Any *processReduceIteration(long long int inx, bool pass_index, uint32_t arrayType, std::vector<fl::Any*> args, 
			std::vector<T> *vArray, fl::Any *accumulator,
			std::vector<fl::DirectedAcyclicGraph<size_t, fl::DataObject>::Vertex*> &vList,
			std::unordered_map <std::string, std::pair<std::string, fl::Any*>> &attributeLookup,
			std::vector<std::pair<std::string, std::string>> &inputReferences,
			std::string outReference, std::string name, fl::Space *_space) {

			std::unordered_map<std::string, fl::Any*> outputAttributeLookup;
			std::vector<fl::Any*> tempAttributes;
			std::unordered_map<std::string, fl::Any*> argumentDependencyMap;

			auto arrayValAny = new fl::Any;
			auto accumValAny = new fl::Any;
			auto arrayIndexAny = new fl::Any;

			arrayValAny->newData<T>(arrayType);
			arrayValAny->setData<T>((*vArray)[inx]);

			fl::copy(accumulator, accumValAny);

			arrayIndexAny->newData<float>(fl::type::FLOAT);
			arrayIndexAny->setData<float>((float)inx);

			tempAttributes.push_back(arrayValAny);
			tempAttributes.push_back(accumValAny);
			tempAttributes.push_back(arrayIndexAny);

			fl::Any *outValue = nullptr;

			size_t argCounter = 5;
			for (size_t inj = 0; inj < inputReferences.size(); inj++) {
				if (inj == 0) {
					argumentDependencyMap[inputReferences[inj].first] = arrayValAny;
				}
				else if (inj == 1) {
					argumentDependencyMap[inputReferences[inj].first] = accumValAny;
				}
				else if (inj == 2 && pass_index) {
					argumentDependencyMap[inputReferences[inj].first] = arrayIndexAny;
				}
				else {
					if (argCounter < args.size()) {
						argumentDependencyMap[inputReferences[inj].first] = args[argCounter];
						argCounter++;
					}
				}
			}

			for (auto v : vList) {
				for (auto attr : v->userData->propertiesOriginalOrder()) {
					if (attr->role() == fl::role::OUTPUT) {
						auto tany = new fl::Any;
						fl::copy(attr, tany);
						outputAttributeLookup[attr->absolutePath()] = tany;
					}
				}
			}

			for (auto v : vList) {

				std::vector<fl::Any*> t_args;
				std::vector<fl::Any*> t_returnValues;

				for (auto attr : v->userData->propertiesOriginalOrder()) {
					if (attr->role() == fl::role::INPUT) {

						bool hasDependency = false;

						for (size_t inj = 0; inj < inputReferences.size(); inj++) {
							auto itr = argumentDependencyMap.find(attr->absolutePath());

							if (itr != argumentDependencyMap.end()) {
								t_args.push_back(itr->second);
								hasDependency = true;
								break;
							}
						}

						if (!hasDependency) {

							auto itr00 = attributeLookup.find(attr->absolutePath());

							if (itr00 != attributeLookup.end()) {
								auto itr01 = outputAttributeLookup.find(itr00->second.first);

								if (itr01 != outputAttributeLookup.end()) {
									t_args.push_back(itr01->second);
								}
								else {
									t_args.push_back(itr00->second.second);
								}

							}
						}
					}
				}

				v->userData->callStaticMethod(name, _space, v->userData, t_args, t_returnValues);

				outValue = processReturnValues(v->userData, t_returnValues, 0, true, &outputAttributeLookup, true);

			}

			for (auto temp : tempAttributes) {
				fl::clear(temp);
				delete temp;
			}

			for (auto p : outputAttributeLookup) {
				fl::clear(p.second);
				delete p.second;
			}

			return outValue;
		}
	
		template<typename T>
		fl::Any *processReduceIterations(bool pass_index, uint32_t arrayType,
			std::vector<fl::Any*> &args,
			std::vector<fl::DirectedAcyclicGraph<size_t, fl::DataObject>::Vertex*> &vlist,
			std::unordered_map <std::string, std::pair<std::string, fl::Any*>> &attributeLookup,
			std::vector<std::pair<std::string, std::string>> &inputReferences,
			std::string outReference, std::string name, fl::Space *_space) {


			auto vArray = args[1]->rawData<std::vector<T>>();
			fl::Any *accumulator = fl::duplicate(args[2]);
			if (!vArray->empty()) {

				for (long long int inx = 0; inx < (long long int)vArray->size(); inx++) {
					auto _ovl = processReduceIteration<T>(inx, pass_index, arrayType, args, vArray, accumulator, vlist, attributeLookup, inputReferences, outReference, name, _space);
					fl::move(_ovl, accumulator);
				}
			}

			return accumulator;
		}
	};
}