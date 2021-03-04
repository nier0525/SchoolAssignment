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

	// Base* base_ptr = new Base(); // 부모 객체에 자기 자신을 동적할당 
	 //Base* base_ptr = new Derived2nd(); // 부모 객체에 다른 자식 객체를 동적할당

	Base* base_ptr = new Derived(); // 부모 객체에 캐스팅 할 자식 객체를 동적할당
	
	Derived* der_ptr = dynamic_cast<Derived*>(base_ptr);
	// 이 때 런타임에서 캐스팅이 가능한 지 검사를 하고, 만일 가능하지 않다면 der_ptr 은
	// nullptr 가 되면서 안전하지 않은 캐스팅을 방지한다.

	// 만일 부모 객체가 자기 자신 혹은 캐스팅하는 자식과 다른 자식 클래스를 동적 할당
	// 했다면 이는 불안전한 캐스팅이 되므로 der_ptr 는 nullptr 로 리턴된다.
	// 요점은 RTTI 에서 "포인터가 실제로 가르키는 대상이 캐스팅이 될 수 있는 놈인가?"
	// 를 검사하고, 여부에 따라 값을 리턴한다.

	if (der_ptr != nullptr) {
		der_ptr->Print();
	}
	else {
		cout << "Derived Point is nullptr" << endl;
	}

	delete base_ptr;
	base_ptr = nullptr;
}