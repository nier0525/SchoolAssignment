#include <iostream>
using namespace std;

class Base {
private:

public:
	Base() {}
	virtual ~Base() {};

	void Print() { cout << "Base Class" << endl; }
};

class Derived : public Base {
private:

public:
	Derived() {}
	virtual ~Derived() {}

	void Print() { cout << "Derived Class " << endl; }
	void PrintDerived() { cout << "Derived Secord Print" << endl; }
};

class Derived2nd : public Base {
private:

public:
	Derived2nd() {}
	virtual ~Derived2nd() {}

	void Print() { cout << "Derived 2nd Class " << endl; }
	void PrintDerived() { cout << "Derived 2nd Secord Print" << endl; }
};

int main() {

	// Base* base_ptr = new Base(); // �θ� ��ü�� �ڱ� �ڽ��� �����Ҵ� 
	 //Base* base_ptr = new Derived2nd(); // �θ� ��ü�� �ٸ� �ڽ� ��ü�� �����Ҵ�

	Base* base_ptr = new Derived(); // �θ� ��ü�� ĳ���� �� �ڽ� ��ü�� �����Ҵ�
	
	Derived* der_ptr = dynamic_cast<Derived*>(base_ptr);
	// �� �� ��Ÿ�ӿ��� ĳ������ ������ �� �˻縦 �ϰ�, ���� �������� �ʴٸ� der_ptr ��
	// nullptr �� �Ǹ鼭 �������� ���� ĳ������ �����Ѵ�.

	// ���� �θ� ��ü�� �ڱ� �ڽ� Ȥ�� ĳ�����ϴ� �ڽİ� �ٸ� �ڽ� Ŭ������ ���� �Ҵ�
	// �ߴٸ� �̴� �Ҿ����� ĳ������ �ǹǷ� der_ptr �� nullptr �� ���ϵȴ�.
	// ������ RTTI ���� "�����Ͱ� ������ ����Ű�� ����� ĳ������ �� �� �ִ� ���ΰ�?"
	// �� �˻��ϰ�, ���ο� ���� ���� �����Ѵ�.

	if (der_ptr != nullptr) {
		der_ptr->Print();
	}
	else {
		cout << "Derived Point is nullptr" << endl;
	}

	delete base_ptr;
	base_ptr = nullptr;
}