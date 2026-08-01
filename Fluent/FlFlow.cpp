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

#include "FlFlow.h"
#include "FlDataObject.h"
#include "FlInterpreterScope.h"

namespace fl {

	std::string Flow::decision_model_id = "/FLUENT/FLOW/DECISION_NODE";

	Flow::Flow() {

	}

	Flow::Flow(SpaceInterface *space) {

		_space = space;

		ID_GEN = 0;
		_id   = -1000;
		_type = GRAPH;
		_next = -1;
		_yes  = -1;
		_no   = -1;

		_text = "";

		_posx = 0.f;
		_posy = 0.f;

		auto xstart = new Flow(space, ID_GEN++, START);
		_nodes.push_back(xstart);
	}

	Flow::Flow(SpaceInterface *space, int xid, int xtype) {

		_space = space;

		_id = xid;
		_type = xtype;
		_next = -1;
		_yes  = -1;
		_no   = -1;

	}

	Flow::~Flow() {
		if (!_conditionOuputNodePath.empty() && _space != nullptr) {
			auto any = _space->findAbsolute(_conditionOuputNodePath);

			if (any != nullptr) _space->remove(any);			
		}

		for (auto f : _nodes) delete f;
	}

	void Flow::resetID() {
		if (_id == -1000) {
			int max = 0;

			for (auto flw : _nodes) {
				max = std::max(flw->id(), max);
			}

			ID_GEN = max + 2;
		}
	}

	void Flow::setID(int u_id) {
		_id = u_id;
	}

	int Flow::id() {
		return _id;
	}

	void Flow::setSpace(SpaceInterface *space) {
		_space = space;
	}

	float Flow::posX() {
		return _posx;
	}

	void  Flow::setPosX(float px) {
		_posx = px;
	}

	float Flow::posY() {
		return _posy;
	}

	void  Flow::setPosY(float py) {
		_posy = py;
	}

	std::string Flow::text() {
		return _text;
	}

	void Flow::setText(std::string txt) {
		_text = txt;
	}

	void Flow::setType(int typ) {
		_type = typ;
	}

	int Flow::type() {
		return _type;
	}

	int Flow::next() {
		return _next;
	}

	void Flow::setNext(int nxt) {
		_next = nxt;
	}

	int Flow::yes() {
		return _yes;
	}

	void Flow::setYes(int ys) {
		_yes = ys;
	}

	int Flow::no() {
		return _no;
	}

	void Flow::setNo(int n) {
		_no = n;
	}

	bool Flow::hasNode(int inx) {
		for (auto f : _nodes) if (f->id() == inx) return true;

		return false;
	}

	Flow *Flow::node(int xid) {
		for (auto &n : _nodes) {
			if (n->id() == xid) return n;
		}

		return this;
	}

	std::vector<std::string> &Flow::paths() {
		return _paths;
	}

	std::string Flow::conditionPath() {
		return _conditionPath;
	}

	std::string Flow::conditionOuputNodePath() {
		return _conditionOuputNodePath;
	}

	std::vector<Flow*> &Flow::nodes() {
		return _nodes;
	}

	void Flow::removeNode(int xid) {
		auto itr = _nodes.begin();

		while (itr != _nodes.end()) {
			if ((*itr)->id() == xid) {
				_nodes.erase(itr);
				break;
			}
			itr++;
		}
	}

	int Flow::addProcess() {
		auto proc = new Flow(_space, ID_GEN++, PROCESS);
		_nodes.push_back(proc);

		return proc->id();
	}

	void Flow::removeProcess(int xid) {
		removeNode(xid);
	}

	void Flow::addAction(std::string act) {
		_paths.push_back(act);
	}

	void Flow::removeAction(std::string act) {
		auto itr = _paths.begin();

		while (itr != _paths.end()) {
			if ((*itr) == act) {
				_paths.erase(itr);
				break;
			}
			itr++;
		}
	}

	void Flow::setConditionPath(std::string cpath) {
		_conditionPath = cpath;
	}

	void Flow::removeConditionPath() {
		_conditionPath = "";
	}

	void Flow::setConditionOuputNodePath(std::string cpath) {
		_conditionOuputNodePath = cpath;
	}

	void Flow::removeConditionOuputNodePath() {
		_conditionOuputNodePath = "";
	}

	void Flow::addProcessAction(int xid, std::string path) {
		auto itr = _nodes.begin();

		while (itr != _nodes.end()) {
			if ((*itr)->id() == xid) {
				(*itr)->addAction(path);
				break;
			}
			itr++;
		}
	}

	void Flow::removeProcessAction(int xid, std::string path) {
		auto itr = _nodes.begin();

		while (itr != _nodes.end()) {
			if ((*itr)->id() == xid) {
				(*itr)->removeAction(path);
				break;
			}
			itr++;
		}
	}

	void Flow::insertExistingProcess(int id, int xnext, std::vector<std::string> pts) {
		auto proc = new Flow(_space, id, PROCESS);
		proc->setNext(xnext);
		for (auto p : pts) proc->addAction(p);

		_nodes.push_back(proc);

		if (ID_GEN <= id)  ID_GEN = id + 1;
	}

	int Flow::addDecision() {
		auto deci = new Flow(_space, ID_GEN++, DECISION);

		/*auto any = decisionOutputNode(_space);
		deci->setConditionOuputNodePath(any->absolutePath());*/

		_nodes.push_back(deci);
		return deci->id();
	}

	void Flow::removeDecision(int xid) {
		removeNode(xid);
	}

	void Flow::setDecisionConditionPath(int xid, std::string pth) {
		auto itr = _nodes.begin();

		while (itr != _nodes.end()) {
			if ((*itr)->id() == xid) {
				(*itr)->setConditionPath(pth);
				break;
			}
			itr++;
		}
	}

	void Flow::setDecisionConditionPath(std::string pth) {
		_conditionPath = pth;
	}

	void Flow::removeDecisionConditionPath(int xid) {
		auto itr = _nodes.begin();

		while (itr != _nodes.end()) {
			if ((*itr)->id() == xid) {
				(*itr)->removeConditionPath();
				break;
			}
			itr++;
		}
	}

	void Flow::insertExitingDecision(int xid, int xyes, int xno, std::string pth) {
		auto dec = new Flow(_space, xid, DECISION);
		dec->setConditionPath(pth);
		dec->setYes(xyes);
		dec->setNo(xno);

		_nodes.push_back(dec);

		if (ID_GEN <= xid)  ID_GEN = xid + 1;
	}

	void Flow::setDecisionOuputNodePath(int xid, std::string pth) {
		auto itr = _nodes.begin();

		while (itr != _nodes.end()) {
			if ((*itr)->id() == xid) {
				(*itr)->setConditionOuputNodePath(pth);
				break;
			}
			itr++;
		}
	}

	void Flow::setDecisionOuputNodePath(std::string pth) {
		_conditionOuputNodePath = pth;
	}

	int Flow::addBreak() {
		auto brk = new Flow(_space, ID_GEN++, BREAK);
		_nodes.push_back(brk);

		return brk->id();
	}

	void Flow::removeBreak(int inx) {
		removeNode(inx);
	}

	bool Flow::isNull() {
		if (_type == F_NULL) return true;
		return false;
	}

	bool Flow::isGraph() {
		if (_type == GRAPH) return true;
		return false;
	}

	Flow *Flow::start() {
		auto itr = _nodes.begin();

		while (itr != _nodes.end()) {
			if ((*itr)->id() == 0) {
				return (*itr);
				break;
			}
			itr++;
		}

		return this;
	}

	Flow *Flow::null() {
		return new Flow(nullptr, -1, F_NULL);
	}

	void Flow::decision_method(SpaceInterface *space, DataObject *dao, InterpreterScope *scope) {
		if (!dao->hasProperty("condition")) return;
		if (!dao->hasProperty("output")) return;

		auto _condition = scope->fetchInput(dao->property("condition")->absolutePath())[0];

		bool a = _condition->data<bool>();

		auto output = new fl::Any();
		output->newData<bool> (fl::type::BOOL);
		output->setData<bool>(a);

		auto check = output->data<bool>();

		scope->addOutput(dao->property("output")->absolutePath(), output);
	}
	
	fl::Any *Flow::decisionOutputNode(SpaceInterface *space) {

		// Find Fluent Branch
		auto temp_branch = space->findAbsolute("~/FLUENT/flow/decisions");

		if (temp_branch != nullptr) {

			auto decision_node = fl::DataObject::Builder()
				.setName("decision")
				.setLabel("Decision Node")
				.setMode(fl::DataObject::NODE)
				.setProtected(true)
				.setHasModel(true)
				.setModelReference(decision_model_id)
				.addProperty(space, type::BOOL, role::INPUT, "condition", "True/False", false)
				.addProperty(space, type::BOOL, role::NONE, "output", "Condition Output", false)
				.addFunction(fl::DataObject::MAIN, decision_method)
				.build(space);

			space->move(decision_node, temp_branch);

			return decision_node;
		}

		return nullptr;
	}

	fl::Any *Flow::createModel(fl::Space *space) {
		return decisionOutputNode(space);
	}
}