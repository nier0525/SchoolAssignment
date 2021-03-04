#pragma once

#include "Global.h"


template <typename c>
class Singleton {
private:
	static c* data;
protected:
	Singleton() { cout << "�̱��� ����\n"; }
	virtual ~Singleton() {
		cout << "�̱��� �Ҹ�\n";
	}
public:

	static c* Getdata() {
		if (data == nullptr) {
			data = new c;
		}
		return data;
	}
	static void Deletedata() {
		if (data) {
			delete data;
			data = nullptr;
		}
	}
};

template<typename c>
c* Singleton<c>::data = 0;