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

#pragma once

#include <queue>
#include <functional>
#include <vector>

using DfxTraitUIDAction = std::function<bool(size_t)>;

typedef struct _traverse_callback {
	DfxTraitUIDAction next_id;
	DfxTraitUIDAction traverse_children;
}DfxTraitUIDTraversalActions;

template<typename T>
class DfxTraitUID {
private:
	T root_id;
	std::vector<DfxTraitUID<T>*> children;
public:
	DfxTraitUID();
	virtual ~DfxTraitUID();
	size_t childCount();
	void addChild(DfxTraitUID<T>*);
	void removeChild(DfxTraitUID<T>*);
	DfxTraitUID<T>* getChild(size_t); 
	std::vector<DfxTraitUID<T>*> &getChildren();
	void clearChildren();
	void setRootID(T);
	T getRootID();
	void traverseUIDTree(DfxTraitUIDTraversalActions &);
};

template<typename T>
DfxTraitUID<T>::DfxTraitUID() {
}

template<typename T>
DfxTraitUID<T>::~DfxTraitUID() {
}

template<typename T>
size_t DfxTraitUID<T>::childCount() {
	return children.size();
}

template<typename T>
void DfxTraitUID<T>::addChild(DfxTraitUID<T> *chd) {
	children.push_back(chd);
}

template<typename T>
void DfxTraitUID<T>::removeChild(DfxTraitUID<T> *chd ) {
	size_t counter = 0;
	for (int inx = 0; inx < children.size(); inx++) {

		if (chd->getRootID() == children[inx]->getRootID()) {
			children.erase(children.begin() + counter);
			return;
		}
		counter++;
	}
}

template<typename T>
DfxTraitUID<T>*  DfxTraitUID<T>::getChild(size_t inx) {
	return children[inx];
}

template<typename T>
std::vector<DfxTraitUID<T>*>  &DfxTraitUID<T>::getChildren() {
	return children;
}

template<typename T>
void DfxTraitUID<T>::clearChildren() {
	children.clear();
}

template<typename T>
void DfxTraitUID<T>::setRootID(T rid) {
	root_id = rid;
}

template<typename T>
T DfxTraitUID<T>::getRootID() {
	return root_id;
}

template<typename T>
void DfxTraitUID<T>::traverseUIDTree(DfxTraitUIDTraversalActions &actions) {

	std::queue<DfxTraitUID<T>*> queue;
	queue.push(this);

	while (!queue.empty())
	{
		DfxTraitUID<T> *tmp_ent = queue.front();
		queue.pop();

		actions.next_id(tmp_ent->getRootID());

		if (actions.traverse_children(tmp_ent->getRootID())) {
			for (auto child : tmp_ent->getChildren()) {
				queue.push(child);
			}
		}
	}
}