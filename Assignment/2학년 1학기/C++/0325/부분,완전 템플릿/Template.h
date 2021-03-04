#pragma once

#include "Global.h"

// 일반 템플릿
//-------------------------------------------------

template <typename T>
class AddProgram {
private:
	T data1;
public:
	AddProgram() { data1 = 0; }
	~AddProgram() {}

	void Add(T _data1 , T _data2);
	T GetData() { return data1; }
};

template <typename T>
void AddProgram<T>::Add(T _data1, T _data2) {
	cout << "일반 템플릿 실행" << endl;
	data1 = _data1 + _data2;
}

//-------------------------------------------------

template<typename T>
class AddProgram<T*> {
private:
	T* data;
public:
	AddProgram() { data = nullptr; }
	~AddProgram() { delete data; }

	void Add(T* _data);
	T* GetData() { return data; }
};

//-------------------------------------------------

template<typename T>
void AddProgram<T*>::Add(T* _data) {
	cout << "부분 템플릿 실행" << endl;
	data = _data;
}

// 템플릿 완전 특수화
//-------------------------------------------------

template<>
class AddProgram<char*> {
private:
	char* str;
public:
	AddProgram() { str = new char[100]; }
	~AddProgram() { delete[] str; }

	void Add(char* _data1, char* _data2);
	char* GetData() { return str; }
};

void AddProgram<char*>::Add(char* _data1, char* _data2) {
	cout << "템플릿 특수화 실행" << endl;
	strcat(_data1, _data2);
	strcpy(str, _data1);
}
