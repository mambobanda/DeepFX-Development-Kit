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

#include <vector>
#include <mutex>

template<typename T>
class DfxConcurrentVector {
private:
	std::vector<T> _vector;
private:
	std::mutex push_back_lock;
	std::mutex at_lock;
public:
	DfxConcurrentVector();

	T at(size_t);
	void push_back(T);
	size_t size();
};

template<typename T>
DfxConcurrentVector<T>::DfxConcurrentVector() {

}

template<typename T>
T DfxConcurrentVector<T>::at(size_t index) {
	return _vector[index];
}

template<typename T>
void  DfxConcurrentVector<T>::push_back(T val) {
	while (!push_back_lock.try_lock());

	_vector.push_back(val);

	push_back_lock.unlock();
}

template<typename T>
size_t DfxConcurrentVector<T>::size() {
	return _vector.size();
}