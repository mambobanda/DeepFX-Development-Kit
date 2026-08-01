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

template<typename T>
class DfxLinkedList {
private:
	DfxLinkedList *_next;
	T value;
	size_t count;
public:
	DfxLinkedList();
	DfxLinkedList(T val);
	~DfxLinkedList();

	void push_back(T val);
	T at(size_t inx);
	size_t size();
	T getValue();
	void setValue(T);
	DfxLinkedList *next();
	void setNext(DfxLinkedList *);
	void clear();
};

template<typename T>
DfxLinkedList<T>::DfxLinkedList() {
	_next = nullptr;
	count = 0;
}

template<typename T>
DfxLinkedList<T>::DfxLinkedList(T val) {
	_next = nullptr;
	count = 0;
	value = val;
}

template<typename T>
DfxLinkedList<T>::~DfxLinkedList() {

}

template<typename T>
void DfxLinkedList<T>::push_back(T val) {
	DfxLinkedList<T> *temp = new DfxLinkedList(val);

	if (_next == nullptr) {
		_next = temp;
		count++;
	}
	else {
		DfxLinkedList<T> *nt = _next;
		while (nt != nullptr) {
			if (nt->next() == nullptr) {
				nt->setNext(temp);
				count++;
				break;
			}
			else {
				nt = nt->next();
			}
		}
	}
}

template<typename T>
T DfxLinkedList<T>::at(size_t inx) {

	size_t counter = 0;
	DfxLinkedList<T> *temp = _next;

	while (temp != nullptr)
	{
		if (inx == counter) {
			return temp->getValue();
		}
		else {
			temp = temp->next();
			counter++;
		}
	}

	return T();
}

template<typename T>
size_t DfxLinkedList<T>::size() {
	return count;
}

template<typename T>
T DfxLinkedList<T>::getValue() {
	return value;
}

template<typename T>
void DfxLinkedList<T>::setValue(T val) {
	value = val;
}

template<typename T>
DfxLinkedList<T> *DfxLinkedList<T>::next() {
	return _next;
}

template<typename T>
void DfxLinkedList<T>::setNext(DfxLinkedList *nxt) {
	_next = nxt;
}

template<typename T>
void DfxLinkedList<T>::clear() {

	DfxLinkedList<T> *nt = _next;

	while (nt != nullptr) {
		DfxLinkedList<T> *temp = nt->next();
		delete nt;
		nt = temp;

		count--;
	}
}