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

#include "FlInterpreter.h"

namespace fl {
	Interpreter::Interpreter(Space *xspace, Flow *xflow) : InterpreterScope(xspace) {
		_space = xspace;
		_flow = xflow;

		printDebugInfo = false;
	}

	Interpreter::~Interpreter() {
		clear();
	}

	void Interpreter::printDebugInfoStart(Flow *start) {
#ifdef FL_DEBUG

		std::cout << "ID: " << start->id() << "\n";

		std::cout << "\tType: Start\n";
		std::cout << "\tNext: " << start->next() << "\n\n";

#endif
	}

	void Interpreter::printDebugInfoProcess(Flow *process) {
#ifdef FL_DEBUG

		std::cout << "ID: " << process->id() << "\n";

		std::cout << "\tType: Process\n";
		std::cout << "\tNext: " << process->next() << "\n";
		for (auto p : process->paths()) std::cout << "\tPath: " << p.c_str() << "\n";
		std::cout << "\n";

#endif
	}

	void Interpreter::printDebugInfoDecision(Flow *decision) {
#ifdef FL_DEBUG

		std::cout << "ID: " << decision->id() << "\n";

		std::cout << "\tType: Decision\n";
		std::cout << "\tNext: " << decision->next() << "\n";
		std::cout << "\tCondition Path: " << decision->conditionPath().c_str() << "\n";
		std::cout << "\tYes: " << decision->yes() << "\n";
		std::cout << "\tNo: " << decision->no() << "\n\n";

#endif
	}

	void Interpreter::printDebugInfoBreak(Flow *brk) {
#ifdef FL_DEBUG

		std::cout << "ID: " << brk->id() << "\n";

		std::cout << "\tType: Break\n" << "\n";
		std::cout << "\tNext: " << brk->next() << "\n\n";

#endif
	}

	void Interpreter::setPrintDebugInfo(bool val) {
		printDebugInfo = val;
	}

	void Interpreter::run() {

		auto next = _flow->start();

		if (!next->isNull() && !next->isGraph()) {

			while (!next->isNull() && !next->isGraph()) {

				switch (next->type()) {
				case Flow::START:
				{
					runStart(next);
					if (next->next() == -1) next = Flow::null();
					else {
						next = _flow->node(next->next());
					}
				}
				break;
				case Flow::PROCESS:
				{
					runProcess(next);
					if (next->next() == -1) next = Flow::null();
					else {
						next = _flow->node(next->next());
					}
				}
				break;
				case Flow::DECISION:
				{
					if (runDecision(next)) {
						if (next->yes() == -1) next = Flow::null();
						else {
							next = _flow->node(next->yes());
						}
					}
					else {
						if (next->no() == -1) next = Flow::null();
						else {
							next = _flow->node(next->no());
						}
					}
				}
				break;
				case Flow::BREAK:
				{
					runBreak(next);
					next = Flow::null();
				}
				break;
				default:
					next = Flow::null();
					break;
				}
			}
		}
		else {
			std::cout << "Fluent Error: failed to run flow graph, no start.\n";
		}

		if (next->isNull()) delete next;
	}

	void Interpreter::runProcess(Flow *process) {
		if (printDebugInfo) printDebugInfoProcess(process);

		for (auto path : process->paths()) {
			auto any = _space->findAbsolute(path);

			if (any != nullptr) {
				if (any->type() == fl::type::DATA_OBJECT) {
					executeGraph(any->rawData<DataObject>());
				}
			}
		}

	}

	bool Interpreter::runDecision(Flow *decision) {
		if (printDebugInfo) printDebugInfoDecision(decision);

		auto any = _space->findAbsolute(decision->conditionPath());
		auto decisionNode = _space->findAbsolute(decision->conditionOuputNodePath());

		if (any != nullptr && decisionNode != nullptr) {
			if (any->type() == fl::type::DATA_OBJECT) {
				executeGraph(any->rawData<DataObject>(), decision);
				auto out = fetchInput(decisionNode->rawData<DataObject>()->property("output")->absolutePath())[0];

				return out->data<bool>();
			}
		}

		return false;
	}

	void Interpreter::runStart(Flow *start) {
		if (printDebugInfo) printDebugInfoStart(start);
	}

	void Interpreter::runBreak(Flow *brk) {
		if (printDebugInfo) printDebugInfoBreak(brk);
	}

	// Build Dag
	void Interpreter::buildDAG(DataObject *dao, DirectedAcyclicGraph<size_t, DataObject> *graph, Flow *flow) {
		std::unordered_map<size_t, DirectedAcyclicGraph<size_t, DataObject>::Vertex*> vertices;

		if (flow != nullptr) {
			if (flow->type() == Flow::DECISION) {
				auto any = _space->findAbsolute(flow->conditionOuputNodePath());

				if (any != nullptr) {
					auto vertex = new DirectedAcyclicGraph<size_t, DataObject>::Vertex();
					vertex->id = any->rawData<DataObject>()->uid();
					vertex->description = any->absolutePath();
					vertex->userData = any->rawData<DataObject>();
					vertex->staticMethod = any->rawData<DataObject>()->defuaultStaticMethod();

					vertices[vertex->id] = vertex;
					graph->addVertex(vertex);
				}
			}
		}


		for (auto any : dao->properties()) {
			if (any->type() == fl::type::DATA_OBJECT) {
				auto node = any->rawData<DataObject>();

				if (node->mode() == DataObject::NODE ||
					node->mode() == DataObject::COMPOUND_NODE) {
					auto vertex = new DirectedAcyclicGraph<size_t, DataObject>::Vertex();
					vertex->id = node->uid();
					vertex->description = any->absolutePath();
					vertex->path = any->absolutePath();
					vertex->userData = node;
					vertex->staticMethod = node->defuaultStaticMethod();

					vertices[vertex->id] = vertex;
					graph->addVertex(vertex);
				}
			}
		}

		for (auto vert : graph->allVertices()) {
			for (auto p : vert->userData->properties()) {
				auto lnks = _space->findLinks(p->absolutePath());

				for (auto link : lnks) {
					auto any = _space->findAbsolute(link.output_node);

					if (any != nullptr) {
						if (any->type() == fl::type::DATA_OBJECT) {
							auto itr = vertices.find(any->rawData<fl::DataObject>()->uid());

							if (itr != vertices.end()) {
								graph->addEdge(itr->second, vert);
							}
						}
					}
				}
			}
		}

	}

	// Free DAG Memory
	void Interpreter::deleteDAG(DirectedAcyclicGraph<size_t, DataObject> *graph) {
		for (auto v : graph->allVertices()) delete v;
		delete graph;
	}

	// Execute DataObject Graph
	void Interpreter::executeGraph(DataObject *dao, Flow *flow) {
		auto dag = new DirectedAcyclicGraph<size_t, DataObject>();
		buildDAG(dao, dag, flow);

		if (!dag->isCyclic()) {
			auto vList = dag->topologicalSort();

#ifdef FL_DEBUG

			printf("---New DAG---\n");
			printf("DAG Vertex Order: \n");
			for (auto v : vList) {
				printf("\t-%s\n", v->description.c_str());
			}

#endif

			for (auto v : vList) {
				if (v->userData->mode() == DataObject::NODE) {
					executeStaticMethod(DataObject::STATIC_MAIN, v->userData, _space, this);
				}
				else if (v->userData->mode() == DataObject::COMPOUND_NODE) {
					auto anyPath = v->userData->property("path");

					if (anyPath != nullptr) {
						anyPath->setData<std::string>(v->path);
					}
					else {
						std::cout << "Unknown Compound Path.\n";
					}

					if (!v->userData->isFunctional()) {
						executeGraph(v->userData);
					}
				}
			}
		}

		deleteDAG(dag);
	}

	void Interpreter::executeStaticMethod(std::string name, DataObject *dao, fl::Space *_space, fl::InterpreterScope *scope) {

		if (dao->iterationType() == fl::IterationType::NONE) {
			executeNormalStaticMethod(name, dao, _space, scope);
		}
		else if (dao->iterationType() == fl::IterationType::FOR_EACH) {
			executeIterationStaticMethod(name, dao, _space, scope, fl::IterationType::FOR_EACH, nullptr, IteratorOutputType::SAME_AS_INPUT);
		}
		else if (dao->iterationType() == fl::IterationType::MAP) {
			executeIterationStaticMethod(name, dao, _space, scope, fl::IterationType::MAP, mapIteratorOutputCallback, IteratorOutputType::SAME_AS_INPUT);
		}
		else if (dao->iterationType() == fl::IterationType::PEEK) {
			executeIterationStaticMethod(name, dao, _space, scope, fl::IterationType::PEEK, peekIteratorOutputCallback, IteratorOutputType::SAME_AS_INPUT);
		}
		else if (dao->iterationType() == fl::IterationType::FILTER) {
			executeIterationStaticMethod(name, dao, _space, scope, fl::IterationType::FILTER, filterIteratorOutputCallback, IteratorOutputType::BOOLEAN);
		}
		else if (dao->iterationType() == fl::IterationType::PARTITION) {
			executeIterationStaticMethod(name, dao, _space, scope, fl::IterationType::PARTITION, partitionIteratorOutputCallback, IteratorOutputType::BOOLEAN);
		}
		else if (dao->iterationType() == fl::IterationType::REDUCE) {
			executeReduceIterationStaticMethod(name, dao, _space, scope, fl::IterationType::REDUCE, IteratorOutputType::SAME_AS_INPUT);
		}

	}

	fl::Any *Interpreter::processReturnValues(fl::DataObject *dao, std::vector<fl::Any*> &returnValues, size_t outputRefPath, bool returnFirstOutptValue,
		std::unordered_map<std::string, fl::Any*> *outputAttributeLookup, bool useOutputLookup) {

		std::vector<fl::Any*> outputs;
		fl::Any *retAny = nullptr;

		if (returnValues.empty()) {
			dao->setErrorFlag(true);
			dao->setErrorString("Undefined Behavior");
			return nullptr;
		}
		else if (returnValues[0]->type() != fl::type::INTERPRETER_ERROR) {
			dao->setErrorFlag(true);
			dao->setErrorString("Undefined Behavior: Unknown State");
			return nullptr;
		}
		else {

			auto ierror = returnValues[0]->data<fl::InterpreterError>();
			dao->setErrorFlag(ierror.hasError);
			dao->setErrorString(ierror.errorMsg);

			if (!ierror.hasError) {
				for (auto attr : dao->propertiesOriginalOrder()) {
					if (attr->role() == fl::role::OUTPUT) outputs.push_back(attr);
				}

				if ((returnValues.size() - 1) != outputs.size()) {
					dao->setErrorFlag(true);
					dao->setErrorString("Incompatible Output Signature");
					return nullptr;
				}
				else {
					bool outputsAllMatch = true;

					for (int i = 0; i < outputs.size(); i++) {
						if (returnValues[i + 1]->type() != outputs[i]->type() && !outputs[i]->allowAnyType()) {
							outputsAllMatch = false;
							dao->setErrorFlag(true);
							dao->setErrorString("Return Value doesn't match output[" + std::to_string(i) + "] type.");
							return nullptr;;
						}

						if (returnValues[i + 1]->isArray() && !outputs[i]->isArray() && !outputs[i]->allowAnyType()) {
							outputsAllMatch = false;
							dao->setErrorFlag(true);
							dao->setErrorString("Return Value doesn't match output[" + std::to_string(i) + "] type. Not array.");
							return nullptr;;
						}
					}

					if (outputsAllMatch) {
						for (int i = 0; i < outputs.size(); i++) {

							if (outputs[i]->address() == outputRefPath && returnFirstOutptValue) {
								retAny = fl::duplicate(returnValues[i + 1]);
							}

							if (!useOutputLookup) {
								fl::move(returnValues[i + 1], outputs[i]);
							}
							else {
								auto itr = outputAttributeLookup->find(outputs[i]->absolutePath());

								if (itr != outputAttributeLookup->end()) {
									fl::move(returnValues[i + 1], itr->second);
								}
							}
						}
					}

					for (auto rt : returnValues) {
						fl::clear(rt);
						delete rt;
					}
				}
			}
		}

		return retAny;
	}

	void Interpreter::executeNormalStaticMethod(std::string name, DataObject *dao, fl::Space *_space, fl::InterpreterScope *scope) {

		std::vector<fl::Any*> args;
		std::vector<fl::Any*> returnValues;
		std::vector<fl::Any*> outputs;

		for (auto attr : dao->propertiesOriginalOrder()) {
			if (attr->role() == fl::role::INPUT || attr->role() == fl::role::HIDDEN_INPUT) {
				auto inputs = scope->fetchInput(attr->absolutePath());

				if (!inputs.empty()) {
					auto _attr = inputs[0];
					args.push_back(_attr);
				}
			}
		}

		if (dao->defuaultStaticMethod() != nullptr) {
			dao->callStaticMethod(name, _space, dao, args, returnValues);
			processReturnValues(dao, returnValues);
		}
		else
			dao->callMethod(fl::DataObject::MAIN, _space, scope);


	}

	bool Interpreter::validIterationOutputSignature(fl::DataObject *dao, fl::Space *_space,
		std::vector<std::pair<std::string, std::string>> outputReferences,
		size_t iterationType, size_t outputType) {

		if (iterationType == fl::IterationType::MAP ||
			iterationType == fl::IterationType::FILTER ||
			iterationType == fl::IterationType::PARTITION ||
			iterationType == fl::IterationType::PEEK ||
			iterationType == fl::IterationType::REDUCE) {

			if (outputReferences.empty()) {
				dao->setErrorFlag(true);
				dao->setErrorString("Undefined Output: Needs functor output.");
				return false;
			}
			else {
				auto _refOutAny = _space->findAbsolute(outputReferences[0].first);

				if (_refOutAny != nullptr) {
					if (_refOutAny->type() != outputType) {
						dao->setErrorFlag(true);
						dao->setErrorString("Undefined Output: First functor output wrong type.");
						return false;
					}
					else {
						if (_refOutAny->isArray()) {
							dao->setErrorFlag(true);
							dao->setErrorString("Undefined Output: First functor output can't be array.");
							return false;
						}
					}
				}
				else {
					dao->setErrorFlag(true);
					dao->setErrorString("Undefined Output: First functor output doesn't exist.");
					return false;
				}
			}

			return true;
		}

		else {
			return true;
		}
	}

	void Interpreter::executeIterationStaticMethod(
		std::string name, DataObject *dao, fl::Space *_space, fl::InterpreterScope *scope, 
		size_t iterationType, IteratorOutputProcessor itopCallback, size_t outputType) {
	
		std::vector<fl::Any*> args;
		std::vector<fl::Any*> functorOutputs;
		std::vector<fl::Any*> returnValues;

		for (auto attr : dao->propertiesOriginalOrder()) {
			if (attr->role() == fl::role::INPUT) {
				auto inputs = scope->fetchInput(attr->absolutePath());

				if (!inputs.empty()) {
					auto _attr = inputs[0];
					args.push_back(_attr);
				}
			}
			else if (attr->role() == fl::role::OUTPUT) {
				functorOutputs.push_back(attr);
			}
		}

		if (args.size() < 2) {
			dao->setErrorFlag(true);
			dao->setErrorString("Undefined Input: Needs a functor and an Array");
		}
		else {

			if (args[0]->type() != fl::type::STD_STRING) {
				dao->setErrorFlag(true);
				dao->setErrorString("Undefined Input: Functor path has to be string");
				return;
			}

			if (!args[1]->isArray()) {
				dao->setErrorFlag(true);
				dao->setErrorString("Undefined Input: Second input must be array.");
				return;
			}

			if (args[1]->type() != fl::type::FLOAT &&
				args[1]->type() != fl::type::FLOAT_3 &&
				args[1]->type() != fl::type::SIZE_T &&
				args[1]->type() != fl::type::BOOL &&
				args[1]->type() != fl::type::STD_STRING &&
				args[1]->type() != fl::type::ATTRIBUTE_PATH) {

				dao->setErrorFlag(true);
				dao->setErrorString("Undefined Input: Array unknown type.");
				return;
			}

			if (args[2]->type() != fl::type::BOOL || args[2]->isArray()) {
				dao->setErrorFlag(true);
				dao->setErrorString("Undefined Input: 'Is Parallel' wrong type.");
				return;
			}

			if (args[3]->type() != fl::type::BOOL || args[3]->isArray()) {
				dao->setErrorFlag(true);
				dao->setErrorString("Undefined Input: 'Pass Index' wrong type.");
				return;
			}

			auto functorAny = _space->findAbsolute(args[0]->data<std::string>());

			if (functorAny != nullptr) {

				if (functorAny->type() == fl::type::DATA_OBJECT) {
					auto functorDao = functorAny->rawData<fl::DataObject>();

					if (functorDao->isFunctional()) {
						fl::DataObject tempDao;
						auto forEachDag = new DirectedAcyclicGraph<size_t, DataObject>();

						tempDao.addProperty(functorAny);
						buildDAG(functorDao, forEachDag, nullptr);

						if (!forEachDag->isCyclic()) {

							std::vector<fl::DirectedAcyclicGraph<size_t, fl::DataObject>::Vertex*> vList = forEachDag->topologicalSort();
							std::unordered_map <std::string, std::pair<std::string, fl::Any*>> attributeLookup;
							bool isParallel = args[2]->data<bool>();
							bool passIndex = args[3]->data<bool>();

							auto xArray = new std::vector<fl::Any*>();

							auto intputReferences = functorDao->inputReferences();
							auto outputReferences = functorDao->outputReferences();
							size_t outputReference = 0;

							if (!outputReferences.empty()) {
								auto outReferenceAny = _space->findAbsolute(outputReferences[0].first);
								outputReference = outReferenceAny->address();
							}

							for (auto v: vList) {
								for (auto attr : v->userData->propertiesOriginalOrder()) {
									if (attr->role() == fl::role::INPUT) {
										auto inputs = scope->fetchInput(attr->absolutePath());

										if (!inputs.empty()) {
											auto _attr = inputs[0];
											std::pair<std::string, fl::Any*> p(_attr->absolutePath(), _attr);
											attributeLookup[attr->absolutePath()] = p;
										}
									}
								}
							}

							dao->setErrorFlag(false);
							dao->setErrorString("");

							if (args[1]->type() == fl::type::FLOAT) {
								size_t _opType = fl::type::FLOAT;
								if (outputType == IteratorOutputType::BOOLEAN) _opType = fl::type::BOOL;

								if (validIterationOutputSignature(dao, _space, outputReferences, iterationType, _opType)) {
									processIterations<float>(isParallel, passIndex, (uint32_t)fl::type::FLOAT, args, vList, attributeLookup, functorDao->inputReferences(), outputReference, name, _space, xArray);
								}

								if (itopCallback != nullptr) itopCallback(dao, args[1], xArray, fl::type::FLOAT);
							}
							else if (args[1]->type() == fl::type::FLOAT_3) {
								size_t _opType = fl::type::FLOAT_3;
								if (outputType == IteratorOutputType::BOOLEAN) _opType = fl::type::BOOL;

								if (validIterationOutputSignature(dao, _space, outputReferences, iterationType, _opType)) {
									processIterations<glm::vec3>(isParallel, passIndex, (uint32_t)fl::type::FLOAT_3, args, vList, attributeLookup, functorDao->inputReferences(), outputReference, name, _space, xArray);
								}

								if (itopCallback != nullptr) itopCallback(dao, args[1], xArray, fl::type::FLOAT_3);
							}
							else if (args[1]->type() == fl::type::SIZE_T) {
								size_t _opType = fl::type::SIZE_T;
								if (outputType == IteratorOutputType::BOOLEAN) _opType = fl::type::BOOL;

								if (validIterationOutputSignature(dao, _space, outputReferences, iterationType, _opType)) {
									processIterations<size_t>(isParallel, passIndex, (uint32_t)fl::type::SIZE_T, args, vList, attributeLookup, functorDao->inputReferences(), outputReference, name, _space, xArray);
								}

								if (itopCallback != nullptr) itopCallback(dao, args[1], xArray, fl::type::SIZE_T);
							}
							else if (args[1]->type() == fl::type::STD_STRING) {
								size_t _opType = fl::type::STD_STRING;
								if (outputType == IteratorOutputType::BOOLEAN) _opType = fl::type::BOOL;

								if (validIterationOutputSignature(dao, _space, outputReferences, iterationType, _opType)) {
									processIterations<std::string>(isParallel, passIndex, (uint32_t)fl::type::STD_STRING, args, vList, attributeLookup, functorDao->inputReferences(), outputReference, name, _space, xArray);
								}

								if (itopCallback != nullptr) itopCallback(dao, args[1], xArray, fl::type::STD_STRING);
							}
							else if (args[1]->type() == fl::type::ATTRIBUTE_PATH) {
								size_t _opType = fl::type::ATTRIBUTE_PATH;
								if (outputType == IteratorOutputType::BOOLEAN) _opType = fl::type::BOOL;

								if (validIterationOutputSignature(dao, _space, outputReferences, iterationType, _opType)) {
									processIterations<std::string>(isParallel, passIndex, (uint32_t)fl::type::ATTRIBUTE_PATH, args, vList, attributeLookup, functorDao->inputReferences(), outputReference, name, _space, xArray);
								}

								if (itopCallback != nullptr) itopCallback(dao, args[1], xArray, fl::type::ATTRIBUTE_PATH);
							}
							else if (args[1]->type() == fl::type::BOOL) {
								size_t _opType = fl::type::BOOL;
								if (outputType == IteratorOutputType::BOOLEAN) _opType = fl::type::BOOL;

								if (validIterationOutputSignature(dao, _space, outputReferences, iterationType, _opType)) {
									processIterations<bool>(isParallel, passIndex, (uint32_t)fl::type::BOOL, args, vList, attributeLookup, functorDao->inputReferences(), outputReference, name, _space, xArray);
								}

								if (itopCallback != nullptr) itopCallback(dao, args[1], xArray, fl::type::BOOL);
							}
							
							
						}
					}
					else {
						dao->setErrorFlag(true);
						dao->setErrorString("Functor is not functional.");
						return;
					}
				}
				else {
					dao->setErrorFlag(true);
					dao->setErrorString("Unkown functor type.");
					return;
				}
			}
			else {
				dao->setErrorFlag(true);
				dao->setErrorString("Unable to retrieve functor.");
				return;
			}
			
		}
	}

	void Interpreter::executeReduceIterationStaticMethod(
		std::string name, DataObject *dao, fl::Space *_space, fl::InterpreterScope *scope, 
		size_t iterationType, size_t outputType) {

		std::vector<fl::Any*> args;
		std::vector<fl::Any*> functorOutputs;
		std::vector<fl::Any*> returnValues;

		for (auto attr : dao->propertiesOriginalOrder()) {
			if (attr->role() == fl::role::INPUT) {
				auto inputs = scope->fetchInput(attr->absolutePath());

				if (!inputs.empty()) {
					auto _attr = inputs[0];
					args.push_back(_attr);
				}
			}
			else if (attr->role() == fl::role::OUTPUT) {
				functorOutputs.push_back(attr);
			}
		}

		if (args.size() < 3) {
			dao->setErrorFlag(true);
			dao->setErrorString("Undefined Input: Needs a functor, an Array and an initial value");
		}
		else {

			if (args[0]->type() != fl::type::STD_STRING) {
				dao->setErrorFlag(true);
				dao->setErrorString("Undefined Input: Functor path has to be string");
				return;
			}

			if (!args[1]->isArray()) {
				dao->setErrorFlag(true);
				dao->setErrorString("Undefined Input: Second input must be array.");
				return;
			}

			if (args[1]->type() != args[2]->type()) {
				dao->setErrorFlag(true);
				dao->setErrorString("Undefined Input: Array and Initial value must be same type");
				return;
			}

			if (args[1]->type() != fl::type::FLOAT &&
				args[1]->type() != fl::type::FLOAT_3 &&
				args[1]->type() != fl::type::SIZE_T &&
				args[1]->type() != fl::type::BOOL &&
				args[1]->type() != fl::type::STD_STRING &&
				args[1]->type() != fl::type::ATTRIBUTE_PATH) {

				dao->setErrorFlag(true);
				dao->setErrorString("Undefined Input: Array / Initial value unknown type.");
				return;
			}

			if (args[3]->type() != fl::type::BOOL || args[3]->isArray()) {
				dao->setErrorFlag(true);
				dao->setErrorString("Undefined Input: 'Pass Index' wrong type.");
				return;
			}

			auto functorAny = _space->findAbsolute(args[0]->data<std::string>());

			if (functorAny != nullptr) {

				if (functorAny->type() == fl::type::DATA_OBJECT) {
					auto functorDao = functorAny->rawData<fl::DataObject>();

					if (functorDao->isFunctional()) {
						fl::DataObject tempDao;
						auto forEachDag = new DirectedAcyclicGraph<size_t, DataObject>();

						tempDao.addProperty(functorAny);
						buildDAG(functorDao, forEachDag, nullptr);

						if (!forEachDag->isCyclic()) {

							std::vector<fl::DirectedAcyclicGraph<size_t, fl::DataObject>::Vertex*> vList = forEachDag->topologicalSort();
							std::unordered_map <std::string, std::pair<std::string, fl::Any*>> attributeLookup;
							bool isParallel = args[2]->data<bool>();
							bool passIndex = args[3]->data<bool>();

							auto xArray = new std::vector<fl::Any*>();

							auto intputReferences = functorDao->inputReferences();
							auto outputReferences = functorDao->outputReferences();
							std::string outputReference = "";

							if (!outputReferences.empty()) outputReference = outputReferences[0].first;

							for (auto v : vList) {
								for (auto attr : v->userData->propertiesOriginalOrder()) {
									if (attr->role() == fl::role::INPUT) {
										auto inputs = scope->fetchInput(attr->absolutePath());

										if (!inputs.empty()) {
											auto _attr = inputs[0];
											std::pair<std::string, fl::Any*> p(_attr->absolutePath(), _attr);
											attributeLookup[attr->absolutePath()] = p;
										}
									}
								}
							}

							dao->setErrorFlag(false);
							dao->setErrorString("");

							fl::Any *result = nullptr;
							if (args[1]->type() == fl::type::FLOAT) {
								size_t _opType = fl::type::FLOAT;
								if (outputType == IteratorOutputType::BOOLEAN) _opType = fl::type::BOOL;

								if (validIterationOutputSignature(dao, _space, outputReferences, iterationType, _opType)) {
									result = processReduceIterations<float>(passIndex, (uint32_t)fl::type::FLOAT, args, vList, attributeLookup, functorDao->inputReferences(), outputReference, name, _space);
								}

							}
							else if (args[1]->type() == fl::type::FLOAT_3) {
								size_t _opType = fl::type::FLOAT_3;
								if (outputType == IteratorOutputType::BOOLEAN) _opType = fl::type::BOOL;

								if (validIterationOutputSignature(dao, _space, outputReferences, iterationType, _opType)) {
									result = processReduceIterations<glm::vec3>(passIndex, (uint32_t)fl::type::FLOAT_3, args, vList, attributeLookup, functorDao->inputReferences(), outputReference, name, _space);
								}

							}
							else if (args[1]->type() == fl::type::SIZE_T) {
								size_t _opType = fl::type::SIZE_T;
								if (outputType == IteratorOutputType::BOOLEAN) _opType = fl::type::BOOL;

								if (validIterationOutputSignature(dao, _space, outputReferences, iterationType, _opType)) {
									result = processReduceIterations<size_t>(passIndex, (uint32_t)fl::type::SIZE_T, args, vList, attributeLookup, functorDao->inputReferences(), outputReference, name, _space);
								}

							}
							else if (args[1]->type() == fl::type::STD_STRING) {
								size_t _opType = fl::type::STD_STRING;
								if (outputType == IteratorOutputType::BOOLEAN) _opType = fl::type::BOOL;

								if (validIterationOutputSignature(dao, _space, outputReferences, iterationType, _opType)) {
									result = processReduceIterations<std::string>(passIndex, (uint32_t)fl::type::STD_STRING, args, vList, attributeLookup, functorDao->inputReferences(), outputReference, name, _space);
								}
							}
							else if (args[1]->type() == fl::type::ATTRIBUTE_PATH) {
								size_t _opType = fl::type::ATTRIBUTE_PATH;
								if (outputType == IteratorOutputType::BOOLEAN) _opType = fl::type::BOOL;

								if (validIterationOutputSignature(dao, _space, outputReferences, iterationType, _opType)) {
									result = processReduceIterations<std::string>(passIndex, (uint32_t)fl::type::ATTRIBUTE_PATH, args, vList, attributeLookup, functorDao->inputReferences(), outputReference, name, _space);
								}
							}
							else if (args[1]->type() == fl::type::BOOL) {
								size_t _opType = fl::type::BOOL;
								if (outputType == IteratorOutputType::BOOLEAN) _opType = fl::type::BOOL;

								if (validIterationOutputSignature(dao, _space, outputReferences, iterationType, _opType)) {
									result = processReduceIterations<bool>(passIndex, (uint32_t)fl::type::BOOL, args, vList, attributeLookup, functorDao->inputReferences(), outputReference, name, _space);
								}
							}

							if (dao->hasProperty("value")) {

								auto propAny = dao->property("value");
								fl::move(result, propAny);
								delete result;
							}
							else {
								dao->setErrorFlag(true);
								dao->setErrorString("Reduce Iterator Callback: Can't find property 'value'");
							}
						}
					}
					else {
						dao->setErrorFlag(true);
						dao->setErrorString("Functor is not functional.");
						return;
					}
				}
				else {
					dao->setErrorFlag(true);
					dao->setErrorString("Unkown functor type.");
					return;
				}
			}
			else {
				dao->setErrorFlag(true);
				dao->setErrorString("Unable to retrieve functor.");
				return;
			}
		
		}
	}

	void Interpreter::mapIteratorOutputCallback(DataObject *dao, fl::Any *vArray, std::vector<fl::Any*> *xArray, size_t type) {
		
		if (dao->hasProperty("out_array") && dao->hasProperty("size")) {

			auto propAny = dao->property("out_array");
			bool sameType = true;
			bool allScalar = true;

			for (auto arr : (*xArray)) {
				if (arr->type() != type) sameType = false;
				break;
			}

			for (auto arr : (*xArray)) {
				if (arr->isArray()) allScalar = false;
				break;
			}

			if (!sameType) {
				dao->setErrorFlag(true);
				dao->setErrorString("Map Iterator Callback: Return values are of different type'");
				return;
			}

			if (!allScalar) {
				dao->setErrorFlag(true);
				dao->setErrorString("Map Iterator Callback: All Return values must be scalar values");
				return;
			}

			float size = 0;
			if (type == fl::type::FLOAT) {
				fl::clear(propAny);
				propAny->newData<std::vector<float>>(fl::type::FLOAT);

				for(auto an: (*xArray)) propAny->rawData<std::vector<float>>()->push_back(an->data<float>());		
				size = (float)propAny->rawData<std::vector<float>>()->size();
			}
			else if (type == fl::type::FLOAT_3) {
				fl::clear(propAny);
				propAny->newData<std::vector<glm::vec3>>(fl::type::FLOAT_3);

				for (auto an : (*xArray)) propAny->rawData<std::vector<glm::vec3>>()->push_back(an->data<glm::vec3>());
				size = (float)propAny->rawData<std::vector<glm::vec3>>()->size();
			}
			else if (type == fl::type::SIZE_T) {
				fl::clear(propAny);
				propAny->newData<std::vector<size_t>>(fl::type::SIZE_T);

				for (auto an : (*xArray)) propAny->rawData<std::vector<size_t>>()->push_back(an->data<size_t>());
				size = (float)propAny->rawData<std::vector<size_t>>()->size();
			}
			else if (type == fl::type::STD_STRING) {
				fl::clear(propAny);
				propAny->newData<std::vector<std::string>>(fl::type::STD_STRING);

				for (auto an : (*xArray)) propAny->rawData<std::vector<std::string>>()->push_back(an->data<std::string>());
				size = (float)propAny->rawData<std::vector<std::string>>()->size();
			}
			else if (type == fl::type::BOOL) {
				fl::clear(propAny);
				propAny->newData<std::vector<bool>>(fl::type::BOOL);

				for (auto an : (*xArray)) propAny->rawData<std::vector<bool>>()->push_back(an->data<bool>());
				size = (float)propAny->rawData<std::vector<bool>>()->size();
			}
			else if (type == fl::type::ATTRIBUTE_PATH) {
				fl::clear(propAny);
				propAny->newData<std::vector<std::string>>(fl::type::STD_STRING);

				for (auto an : (*xArray)) propAny->rawData<std::vector<std::string>>()->push_back(an->data<std::string>());
				size = (float)propAny->rawData<std::vector<std::string>>()->size();
			}

			dao->property("size")->setData<float>(size);
		}
		else {
			dao->setErrorFlag(true);
			dao->setErrorString("Map Iterator Callback: Can't find property 'out_array'");
		}
	}

	void Interpreter::filterIteratorOutputCallback(DataObject *dao, fl::Any *vArray, std::vector<fl::Any*> *xArray, size_t type) {
		if (dao->hasProperty("out_array") && dao->hasProperty("size")) {

			auto propAny = dao->property("out_array");
			bool sameType = true;
			bool allScalar = true;

			for (auto arr : (*xArray)) {
				if (arr->type() != fl::type::BOOL) sameType = false;
				break;
			}

			for (auto arr : (*xArray)) {
				if (arr->isArray()) allScalar = false;
				break;
			}

			if (!sameType) {
				dao->setErrorFlag(true);
				dao->setErrorString("Filter Iterator Callback: Return values are of different type' must be a True/False value");
				return;
			}

			if (!allScalar) {
				dao->setErrorFlag(true);
				dao->setErrorString("Filter Iterator Callback: All Return values must be scalar values");
				return;
			}

			float size = 0;
			if (type == fl::type::FLOAT) {
				fl::clear(propAny);
				propAny->newData<std::vector<float>>(fl::type::FLOAT);

				auto originalArray = vArray->rawData<std::vector<float>>();

				for (size_t inx = 0; inx < xArray->size(); inx++) {
					if ((*xArray)[inx]->data<bool>()) {
						propAny->rawData<std::vector<float>>()->push_back((*originalArray)[inx]);
					}
				}

				size = (float)propAny->rawData<std::vector<float>>()->size();
			}
			else if (type == fl::type::FLOAT_3) {
				fl::clear(propAny);
				propAny->newData<std::vector<glm::vec3>>(fl::type::FLOAT_3);

				auto originalArray = vArray->rawData<std::vector<glm::vec3>>();

				for (size_t inx = 0; inx < xArray->size(); inx++) {
					if ((*xArray)[inx]->data<bool>()) {
						propAny->rawData<std::vector<glm::vec3>>()->push_back((*originalArray)[inx]);
					}
				}

				size = (float)propAny->rawData<std::vector<glm::vec3>>()->size();
			}
			else if (type == fl::type::SIZE_T) {
				fl::clear(propAny);
				propAny->newData<std::vector<size_t>>(fl::type::SIZE_T);

				auto originalArray = vArray->rawData<std::vector<size_t>>();

				for (size_t inx = 0; inx < xArray->size(); inx++) {
					if ((*xArray)[inx]->data<bool>()) {
						propAny->rawData<std::vector<size_t>>()->push_back((*originalArray)[inx]);
					}
				}

				size = (float)propAny->rawData<std::vector<size_t>>()->size();
			}
			else if (type == fl::type::STD_STRING) {
				fl::clear(propAny);
				propAny->newData<std::vector<std::string>>(fl::type::STD_STRING);

				auto originalArray = vArray->rawData<std::vector<std::string>>();

				for (size_t inx = 0; inx < xArray->size(); inx++) {
					if ((*xArray)[inx]->data<bool>()) {
						propAny->rawData<std::vector<std::string>>()->push_back((*originalArray)[inx]);
					}
				}

				size = (float)propAny->rawData<std::vector<std::string>>()->size();
			}
			else if (type == fl::type::BOOL) {
				fl::clear(propAny);
				propAny->newData<std::vector<bool>>(fl::type::BOOL);

				auto originalArray = vArray->rawData<std::vector<bool>>();

				for (size_t inx = 0; inx < xArray->size(); inx++) {
					if ((*xArray)[inx]->data<bool>()) {
						propAny->rawData<std::vector<bool>>()->push_back((*originalArray)[inx]);
					}
				}

				size = (float)propAny->rawData<std::vector<bool>>()->size();
			}
			else if (type == fl::type::ATTRIBUTE_PATH) {
				fl::clear(propAny);
				propAny->newData<std::vector<std::string>>(fl::type::STD_STRING);

				auto originalArray = vArray->rawData<std::vector<std::string>>();

				for (size_t inx = 0; inx < xArray->size(); inx++) {
					if ((*xArray)[inx]->data<bool>()) {
						propAny->rawData<std::vector<std::string>>()->push_back((*originalArray)[inx]);
					}
				}

				size = (float)propAny->rawData<std::vector<std::string>>()->size();
			}

			dao->property("size")->setData<float>(size);
		}
		else {
			dao->setErrorFlag(true);
			dao->setErrorString("Filter Iterator Callback: Can't find property 'out_array'");
		}
	}

	void Interpreter::partitionIteratorOutputCallback(DataObject *dao, fl::Any *vArray, std::vector<fl::Any*> *xArray, size_t type) {
		if (dao->hasProperty("out_array0") && dao->hasProperty("size0") &&
			dao->hasProperty("out_array1") && dao->hasProperty("size1")) {

			auto propAny00 = dao->property("out_array0");
			auto propAny01 = dao->property("out_array1");

			bool sameType = true;
			bool allScalar = true;

			for (auto arr : (*xArray)) {
				if (arr->type() != fl::type::BOOL) sameType = false;
				break;
			}

			for (auto arr : (*xArray)) {
				if (arr->isArray()) allScalar = false;
				break;
			}

			if (!sameType) {
				dao->setErrorFlag(true);
				dao->setErrorString("Partition Iterator Callback: Return values are of different type' must be a True/False value");
				return;
			}

			if (!allScalar) {
				dao->setErrorFlag(true);
				dao->setErrorString("Partition Iterator Callback: All Return values must be scalar values");
				return;
			}

			float size00 = 0;
			float size01 = 0;
			if (type == fl::type::FLOAT) {
				fl::clear(propAny00);
				fl::clear(propAny01);

				propAny00->newData<std::vector<float>>(fl::type::FLOAT);
				propAny01->newData<std::vector<float>>(fl::type::FLOAT);

				auto originalArray = vArray->rawData<std::vector<float>>();

				for (size_t inx = 0; inx < xArray->size(); inx++) {
					if ((*xArray)[inx]->data<bool>()) {
						propAny00->rawData<std::vector<float>>()->push_back((*originalArray)[inx]);
					}
					else {
						propAny01->rawData<std::vector<float>>()->push_back((*originalArray)[inx]);
					}
				}

				size00 = (float)propAny00->rawData<std::vector<float>>()->size();
				size01 = (float)propAny01->rawData<std::vector<float>>()->size();
			}
			else if (type == fl::type::FLOAT_3) {
				fl::clear(propAny00);
				fl::clear(propAny01);

				propAny00->newData<std::vector<glm::vec3>>(fl::type::FLOAT_3);
				propAny01->newData<std::vector<glm::vec3>>(fl::type::FLOAT_3);

				auto originalArray = vArray->rawData<std::vector<glm::vec3>>();

				for (size_t inx = 0; inx < xArray->size(); inx++) {
					if ((*xArray)[inx]->data<bool>()) {
						propAny00->rawData<std::vector<glm::vec3>>()->push_back((*originalArray)[inx]);
					}
					else {
						propAny01->rawData<std::vector<glm::vec3>>()->push_back((*originalArray)[inx]);
					}
				}

				size00 = (float)propAny00->rawData<std::vector<glm::vec3>>()->size();
				size01 = (float)propAny01->rawData<std::vector<glm::vec3>>()->size();
			}
			else if (type == fl::type::SIZE_T) {
				fl::clear(propAny00);
				fl::clear(propAny01);

				propAny00->newData<std::vector<size_t>>(fl::type::SIZE_T);
				propAny01->newData<std::vector<size_t>>(fl::type::SIZE_T);

				auto originalArray = vArray->rawData<std::vector<size_t>>();

				for (size_t inx = 0; inx < xArray->size(); inx++) {
					if ((*xArray)[inx]->data<bool>()) {
						propAny00->rawData<std::vector<size_t>>()->push_back((*originalArray)[inx]);
					}
					else {
						propAny01->rawData<std::vector<size_t>>()->push_back((*originalArray)[inx]);
					}
				}

				size00 = (float)propAny00->rawData<std::vector<size_t>>()->size();
				size01 = (float)propAny01->rawData<std::vector<size_t>>()->size();
			}
			else if (type == fl::type::STD_STRING) {
				fl::clear(propAny00);
				fl::clear(propAny01);

				propAny00->newData<std::vector<std::string>>(fl::type::STD_STRING);
				propAny01->newData<std::vector<std::string>>(fl::type::STD_STRING);

				auto originalArray = vArray->rawData<std::vector<std::string>>();

				for (size_t inx = 0; inx < xArray->size(); inx++) {
					if ((*xArray)[inx]->data<bool>()) {
						propAny00->rawData<std::vector<std::string>>()->push_back((*originalArray)[inx]);
					}
					else {
						propAny01->rawData<std::vector<std::string>>()->push_back((*originalArray)[inx]);
					}
				}

				size00 = (float)propAny00->rawData<std::vector<std::string>>()->size();
				size01 = (float)propAny01->rawData<std::vector<std::string>>()->size();
			}
			else if (type == fl::type::BOOL) {
				fl::clear(propAny00);
				fl::clear(propAny01);

				propAny00->newData<std::vector<bool>>(fl::type::BOOL);
				propAny01->newData<std::vector<bool>>(fl::type::BOOL);

				auto originalArray = vArray->rawData<std::vector<bool>>();

				for (size_t inx = 0; inx < xArray->size(); inx++) {
					if ((*xArray)[inx]->data<bool>()) {
						propAny00->rawData<std::vector<bool>>()->push_back((*originalArray)[inx]);
					}
					else {
						propAny01->rawData<std::vector<bool>>()->push_back((*originalArray)[inx]);
					}
				}

				size00 = (float)propAny00->rawData<std::vector<bool>>()->size();
				size01 = (float)propAny01->rawData<std::vector<bool>>()->size();
			}
			else if (type == fl::type::ATTRIBUTE_PATH) {
				fl::clear(propAny00);
				fl::clear(propAny01);

				propAny00->newData<std::vector<std::string>>(fl::type::ATTRIBUTE_PATH);
				propAny01->newData<std::vector<std::string>>(fl::type::ATTRIBUTE_PATH);

				auto originalArray = vArray->rawData<std::vector<std::string>>();

				for (size_t inx = 0; inx < xArray->size(); inx++) {
					if ((*xArray)[inx]->data<bool>()) {
						propAny00->rawData<std::vector<std::string>>()->push_back((*originalArray)[inx]);
					}
					else {
						propAny01->rawData<std::vector<std::string>>()->push_back((*originalArray)[inx]);
					}
				}

				size00 = (float)propAny00->rawData<std::vector<std::string>>()->size();
				size01 = (float)propAny01->rawData<std::vector<std::string>>()->size();
			}

			dao->property("size0")->setData<float>(size00);
			dao->property("size1")->setData<float>(size01);
		}
		else {
			dao->setErrorFlag(true);
			dao->setErrorString("Partition Iterator Callback: Can't find property 'out_array'");
		}
	}

	void Interpreter::peekIteratorOutputCallback(DataObject *dao, fl::Any *vArray, std::vector<fl::Any*> *xxArray, size_t type) {
		if (dao->hasProperty("out_array") && dao->hasProperty("size")) {

			auto propAny = dao->property("out_array");

			float size = 0;
			if (type == fl::type::FLOAT) {
				fl::clear(propAny);
				propAny->newData<std::vector<float>>(fl::type::FLOAT);

				auto originalArray = vArray->rawData<std::vector<float>>();

				for (auto an : (*originalArray)) propAny->rawData<std::vector<float>>()->push_back(an);
				size = (float)propAny->rawData<std::vector<float>>()->size();
			}
			else if (type == fl::type::FLOAT_3) {
				fl::clear(propAny);
				propAny->newData<std::vector<glm::vec3>>(fl::type::FLOAT_3);

				auto originalArray = vArray->rawData<std::vector<glm::vec3>>();

				for (auto an : (*originalArray)) propAny->rawData<std::vector<glm::vec3>>()->push_back(an);
				size = (float)propAny->rawData<std::vector<glm::vec3>>()->size();
			}
			else if (type == fl::type::SIZE_T) {
				fl::clear(propAny);
				propAny->newData<std::vector<size_t>>(fl::type::SIZE_T);

				auto originalArray = vArray->rawData<std::vector<size_t>>();

				for (auto an : (*originalArray)) propAny->rawData<std::vector<size_t>>()->push_back(an);
				size = (float)propAny->rawData<std::vector<size_t>>()->size();
			}
			else if (type == fl::type::STD_STRING) {
				fl::clear(propAny);
				propAny->newData<std::vector<std::string>>(fl::type::STD_STRING);

				auto originalArray = vArray->rawData<std::vector<std::string>>();

				for (auto an : (*originalArray)) propAny->rawData<std::vector<std::string>>()->push_back(an);
				size = (float)propAny->rawData<std::vector<std::string>>()->size();
			}
			else if (type == fl::type::BOOL) {
				fl::clear(propAny);
				propAny->newData<std::vector<bool>>(fl::type::BOOL);

				auto originalArray = vArray->rawData<std::vector<bool>>();

				for (auto an : (*originalArray)) propAny->rawData<std::vector<bool>>()->push_back(an);
				size = (float)propAny->rawData<std::vector<bool>>()->size();
			}
			else if (type == fl::type::ATTRIBUTE_PATH) {
				fl::clear(propAny);
				propAny->newData<std::vector<std::string>>(fl::type::STD_STRING);

				auto originalArray = vArray->rawData<std::vector<std::string>>();

				for (auto an : (*originalArray)) propAny->rawData<std::vector<std::string>>()->push_back(an);
				size = (float)propAny->rawData<std::vector<std::string>>()->size();
			}

			dao->property("size")->setData<float>(size);
		}
		else {
			dao->setErrorFlag(true);
			dao->setErrorString("Peek Iterator Callback: Can't find property 'out_array'");
		}
	}

	
}