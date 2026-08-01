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
#include <string>
#include <unordered_map>
#include "FlModel.h"
#include "FlCommon.h"
#include "FlSpaceInterface.h"

namespace fl {
	
	
	class Flow: public fl::Model {
	public:
		static std::string decision_model_id;
	private:
		int ID_GEN;

		int _id;
		int _type;
		int _next;
		int _yes;
		int _no;

		SpaceInterface *_space;

		std::string _conditionPath;
		std::string _conditionOuputNodePath;
		std::vector<std::string> _paths;
		std::vector<Flow*> _nodes;

		Flow(SpaceInterface *space, int id, int type);

		std::string _text;

		float _posx;
		float _posy;
		
		void addAction(std::string);
		void removeAction(std::string);
		void setConditionPath(std::string);
		void removeConditionPath();
		void setConditionOuputNodePath(std::string);
		void removeConditionOuputNodePath();
		void removeNode(int);

	public:
		enum { F_NULL, GRAPH, START, PROCESS, DECISION, BREAK };

		Flow();
		Flow(SpaceInterface *space);
		~Flow();

		void resetID();
		void setID(int);
		int id();

		void setSpace(SpaceInterface *_space);

		void setType(int);
		int type();

		int next();
		void setNext(int);

		int yes();
		void setYes(int);

		int no();
		void setNo(int);

		bool hasNode(int);
		Flow *node(int id);

		float posX();
		void  setPosX(float);

		float posY();
		void  setPosY(float);

		std::vector<std::string> &paths();
		std::string conditionPath();
		std::string conditionOuputNodePath();

		std::vector<Flow*> &nodes();

		std::string text();
		void setText(std::string txt);

		int addProcess();
		void removeProcess(int);
		void addProcessAction(int index, std::string path);
		void removeProcessAction(int, std::string path);
		void insertExistingProcess(int id, int xnext, std::vector<std::string> pts);

		int addDecision();
		void removeDecision(int);
		void setDecisionConditionPath(int, std::string);
		void setDecisionConditionPath(std::string);
		void removeDecisionConditionPath(int);
		void insertExitingDecision(int, int xyes, int xno, std::string);
		void setDecisionOuputNodePath(int, std::string);
		void setDecisionOuputNodePath(std::string);

		int addBreak();
		void removeBreak(int);

		Flow *start();

		bool isNull();
		bool isGraph();

		static Flow *null();
		static void decision_method(SpaceInterface *space, DataObject *dao, InterpreterScope *scope);
		static fl::Any *decisionOutputNode(SpaceInterface *space);

		fl::Any *createModel(fl::Space *space);
	};
}