#pragma once

#include "Global.h"

// �Ϲ� ���ø�
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
	cout << "�Ϲ� ���ø� ����" << endl;
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
	cout << "�κ� ���ø� ����" << endl;
	data = _data;
}

// ���ø� ���� Ư��ȭ
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
	cout << "���ø� Ư��ȭ ����" << endl;
	strcat(_data1, _data2);
	strcpy(str, _data1);
}
