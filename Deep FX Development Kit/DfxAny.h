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


class DfxAny {
private:
	void *_data_;
public:
	DfxAny();

	void *getData();
	void setData(void *);

	bool isNull();
	void toNull();

	template<typename T>
	void deleteData();

	template<typename T>
	void deleteArray();
};

template<typename T>
void DfxAny::deleteData() {
	T *ptr = (T*)_data_;
	delete ptr;
	ptr = nullptr;
	_data_ = nullptr;
}

template<typename T>
void DfxAny::deleteArray() {
	T *ptr = (T*)_data_;
	delete[] ptr;
	ptr = nullptr;
	_data_ = nullptr;
}
