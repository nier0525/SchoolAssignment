#include <iostream>
using namespace std;

class Base {
private:
	int a = 10;
public:
	Base() {}
	virtual ~Base() {};

	void print() { cout << "Base Class" << endl; }
	void printBase() { cout << "Base Class A : " << a << endl; }
};

class Derived : public Base {
private:
	int b = 20;
public:
	Derived() {}
	virtual ~Derived() {}

	void print() { cout << "Derived Class" << endl; }
	void printDerived() { cout << "Derived Class B : " << b << endl; }
};

int main() {


	// static_cast
	// static_cast 는 보통 실수에서 정수, 정수에서 실수와 같은 형변환이 필요할 때 사용함.
	// 상속 역시 모두 허용은 하지만 다운캐스팅을 사용하는 건 상당히 위험하고 부적절함.
	int static_int = 0;
	double static_double = 3.5;
	float static_float = 5.5f;

	cout << "< static_cast >" << endl;
	static_int = static_cast<int>(static_double);	// double -> int 로 변환
	static_double = static_cast<double>(static_float);	// float -> double 로 변환. 물론 반대도 가능	
	
	cout << "static_cast<int>(double) : " << static_int << endl;
	cout << "static_cast<double>(float) : " << static_double << endl;
	// 정상적으로 변환이 된다.
	// 추가로 static_cast<int*>(int[]) , static_cast<int>(enum) 과 같은 식으로도 사용 가능

	//int* ptr;
	//ptr = static_cast<int*>(5);
	//cout << "ptr : " << ptr << endl;
	// 포인터 타입을 정수 타입으로 변환한다는 논리상 맞지 않는 변환을 방지함.

	Base *static_base;
	Derived static_der;
	static_base = static_cast<Derived*>(&static_der);
	cout << "static_cast UpCasting ";
	static_base->print();
	static_base->printBase();
	// 업캐스팅
	// 문제 없이 사용 가능

	Base static_base2;
	Derived* static_der2;

	static_der2 = static_cast<Derived*>(&static_base2);
	cout << "static_cast DownCasting ";
	static_der2->print();
	static_der2->printDerived(); // 자식내에 멤버함수의 값이 예상값과 다름
	// 다운캐스팅
	// 다운캐스팅은 허용은 되지만, 자식 클래스의 멤버 함수를 부르게 된다면 예상값과 다르게 나온다.
	// 부모 클래스 인스턴스가 자식멤버함수를 부르는 것이 비정상적인 결과를 초래할 수 있다.
	// 그렇기 때문에 다운캐스팅을 사용하기에 static_cast 는 상당히 위험하고 부적절하다.
	cout << endl;
	



	// dynamic_cast
	// dynamic_cast 는 다운캐스팅을 사용할 때 안전하게 쓰기 위해서 사용한다. (safe DownCasting)
	// 런타임 시간에 실제로 해당 타입이 다운캐스팅이 가능한 지 검사하기 때문에 런타임 비용이
	// 조금 높다는 흠이 있다
	cout << "< dynamic_cast >" << endl;


	// 부모클래스가 생성자로 생성되었고 부모 포인터가 가르키고 있는 클래스를 자식클래스
	// 포인터로 형변환 할 때
	cout << "< case 1 >" << endl;
	Base* dynamic_base = new Base(); // 부모 객체 생성

	cout << "dynamic_base print : ";
	dynamic_base->print();

	Derived* dynamic_Derived = dynamic_cast<Derived*>(dynamic_base);
	cout << "DownCasting : ";
	if (dynamic_Derived == nullptr) {
		cout << "Runtime Error" << endl;
	}
	else {
		dynamic_Derived->print();
	}
	delete dynamic_base;
	// 이 경우 Base 객체가 이미 생성되었기 때문에 Derived 생성자가 불리지 않는다.
	// 생성자가 불리지 않았기 때문에 형변환 할 타입이 없음. dynamic_Derived 는 nullptr


	// 자식클래스의 생성자로 생성된 부모클래스 포인터가 가리키고 있는 클래스를 자식클래스로
	// 형변환 할 때
	cout << "< case 2 >" << endl;
	Base* dynamic_base2 = new Derived(); // 업캐스팅

	cout << "dynamic_base print : ";
	dynamic_base2->print();

	Derived* dynamic_Derived2 = dynamic_cast<Derived*>(dynamic_base2);
	cout << "DownCasting : ";
	if (dynamic_Derived2 == nullptr) {
		cout << "Runtime Error" << endl;
	}
	else {
		dynamic_Derived2->print();
	}
	delete dynamic_base2;
	// 위와 다른 점은 부모클래스를 객체 생성할 때 업캐스팅을 했다는 점이다.
	// 부모클래스가 실제 가르키는 클래스(new 를 통해 생긴 클래스)는 자기 자식클래스 이므로
	// dynamic_cast 를 통해서 안전하게 다운캐스팅이 가능하다.
	
	// dynamic_cast<Base*>(Derived) 와 같이 명시적인 업캐스팅도 가능하다.
	cout << endl;



	// const_cast
	// const_cast 는 포인터 or 참조형의 상수성(const) 을 잠깐 제거해주는데 사용된다.
	// volatile 키워드를 잠깐 제거해야 할 때도 사용한다.
	// 다른 캐스트 연산자가 하는 형변환은 불가능하고 오로지 상수성을 제거하는 경우에만 사용.
	// 다른 캐스트 연산자는 상수성을 제거하지 못한다.
	// 함수 포인터에는 사용이 불가능
	cout << "< const_cast >" << endl;


	// 포인터 상수성 제거 (const char*)
	cout << "< case 1 >" << endl;
	char str[] = "ABC Text";
	const char* ptr = str;
	cout << "Before : " << str << endl;

	// ptr[0] = 'Q';  이런식으로 사용하는 경우 당연하지만 에러가 난다.
	
	char* const_char = const_cast<char*>(ptr);
	const_char[0] = 'Q';
	cout << "After : " << str << endl;


	// 참조형 상수성 제거 (const int&)
	cout << "< case 2 >" << endl;
	int const_int = 15;
	const int& ref = const_int;
	cout << "Before : " << const_int << endl;

	int& ref_int = const_cast<int&>(ref);
	ref_int = 30;
	cout << "After : " << const_int << endl;

	// 이런식으로 포인터 or 래퍼런스 상수형을 잠깐 제거하는 경우에만 사용한다.
	cout << endl;



	// reinterpret_cast
	// reinterpret_cast 는 포인터 타입끼리 변환을 허용하는 캐스트 연산자이다.
	// 중요한 건 정수형마저 포인터로 바꿔버릴 수 있다는 점이다. ( 당연히 위험 )
	// 다른 형태의 포인터로 변경이 가능하기 때문에 잘못 사용하는 경우에는 결과 값이
	// 예상값과 다를 수 있다. 이 연산자는 확실하게 변환이 가능한 상황에만 한정적으로
	// 사용하는 것이 바람직하다.

	cout << "< reinterpret_cast >" << endl;
	int reinterpret_int = 12456789;
	int* ptr_int;
	ptr_int = reinterpret_cast<int*>(reinterpret_int);
	// int -> int* 로 형변환
	// 이 경우 변수 reinterpret_int 의 값을 절대주소르 받는 ptr_int 포인터 변수가
	// 어느 곳을 가르킬 지 모르기 때문에 위험하다.

	int* ptr_int2 = &reinterpret_int;
	char* reinterpret_char;
	reinterpret_char = reinterpret_cast<char*>(ptr_int2);
	cout << "char reinterpret_cast<char*>(int*) : " << *reinterpret_char << endl;
	printf("int reinterpret_cast<char*>(int*) : %d\n", *reinterpret_char);
	// int* -> char* 로 형변환
	// 아니나 다를까 예상값과 다른 값이 나온다.

	struct test {
		int a;
	};

	test test;
	test.a = 10;
	int* ptr_int3;
	ptr_int3 = reinterpret_cast<int*>(&test);
	cout << "reinterpret_cast<int*>(struct) : " << *ptr_int3 << endl;
	// 구조체 내에 int -> int* 형변환
	// 구조체 test 에는 정수형 변수가 하나만 있음으로 ptr_int3 는 test 구조체 내에
	// a 변수의 시작점을 잘 가르키고 있다.

}