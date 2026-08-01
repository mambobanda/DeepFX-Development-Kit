/*
* The Mailo Today Solutions Software License
*
* Copyright 2017 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#pragma once

template<typename T>
class DfxArray {
private:
	T *_data;
	size_t _size;

public:

	DfxArray();

	DfxArray(size_t sz);

	virtual ~DfxArray();

	void deleteArray();

	void resize(size_t newsz);

	size_t size();

	T &operator[](size_t inx);

	T get(size_t inx);

	void set(size_t inx, T val);

	bool isNull();
};

template<typename T>
DfxArray<T>::DfxArray() {
	_data = nullptr;
	_size = 0;
}

template<typename T>
DfxArray<T>::DfxArray(size_t sz) {
	_data = new T[sz];
	_size = sz;
}

template<typename T>
DfxArray<T>::~DfxArray() {
	deleteArray();
}

template<typename T>
void DfxArray<T>::deleteArray() {
	delete[] _data;
	_data = nullptr;
	_size = 0;
}

template<typename T>
void DfxArray<T>::resize(size_t newsz) {
	deleteArray();
	_data = new T[newsz];
	_size = newsz;
}

template<typename T>
T &DfxArray<T>::operator[](size_t inx) {
	return _data[inx];
}

template<typename T>
T DfxArray<T>::get(size_t inx){
	return _data[inx];
}

template<typename T>
void DfxArray<T>::set(size_t inx, T val) {
	_data[inx] = val;
}

template<typename T>
size_t DfxArray<T>::size() {
	return _size;
}

template<typename T>
bool DfxArray<T>::isNull() {
	if (_data == nullptr)
		return true;
	else
		return false;
}