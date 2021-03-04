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
	// static_cast �� ���� �Ǽ����� ����, �������� �Ǽ��� ���� ����ȯ�� �ʿ��� �� �����.
	// ��� ���� ��� ����� ������ �ٿ�ĳ������ ����ϴ� �� ����� �����ϰ� ��������.
	int static_int = 0;
	double static_double = 3.5;
	float static_float = 5.5f;

	cout << "< static_cast >" << endl;
	static_int = static_cast<int>(static_double);	// double -> int �� ��ȯ
	static_double = static_cast<double>(static_float);	// float -> double �� ��ȯ. ���� �ݴ뵵 ����	
	
	cout << "static_cast<int>(double) : " << static_int << endl;
	cout << "static_cast<double>(float) : " << static_double << endl;
	// ���������� ��ȯ�� �ȴ�.
	// �߰��� static_cast<int*>(int[]) , static_cast<int>(enum) �� ���� �����ε� ��� ����

	//int* ptr;
	//ptr = static_cast<int*>(5);
	//cout << "ptr : " << ptr << endl;
	// ������ Ÿ���� ���� Ÿ������ ��ȯ�Ѵٴ� ���� ���� �ʴ� ��ȯ�� ������.

	Base *static_base;
	Derived static_der;
	static_base = static_cast<Derived*>(&static_der);
	cout << "static_cast UpCasting ";
	static_base->print();
	static_base->printBase();
	// ��ĳ����
	// ���� ���� ��� ����

	Base static_base2;
	Derived* static_der2;

	static_der2 = static_cast<Derived*>(&static_base2);
	cout << "static_cast DownCasting ";
	static_der2->print();
	static_der2->printDerived(); // �ڽĳ��� ����Լ��� ���� ���󰪰� �ٸ�
	// �ٿ�ĳ����
	// �ٿ�ĳ������ ����� ������, �ڽ� Ŭ������ ��� �Լ��� �θ��� �ȴٸ� ���󰪰� �ٸ��� ���´�.
	// �θ� Ŭ���� �ν��Ͻ��� �ڽĸ���Լ��� �θ��� ���� ���������� ����� �ʷ��� �� �ִ�.
	// �׷��� ������ �ٿ�ĳ������ ����ϱ⿡ static_cast �� ����� �����ϰ� �������ϴ�.
	cout << endl;
	



	// dynamic_cast
	// dynamic_cast �� �ٿ�ĳ������ ����� �� �����ϰ� ���� ���ؼ� ����Ѵ�. (safe DownCasting)
	// ��Ÿ�� �ð��� ������ �ش� Ÿ���� �ٿ�ĳ������ ������ �� �˻��ϱ� ������ ��Ÿ�� �����
	// ���� ���ٴ� ���� �ִ�
	cout << "< dynamic_cast >" << endl;


	// �θ�Ŭ������ �����ڷ� �����Ǿ��� �θ� �����Ͱ� ����Ű�� �ִ� Ŭ������ �ڽ�Ŭ����
	// �����ͷ� ����ȯ �� ��
	cout << "< case 1 >" << endl;
	Base* dynamic_base = new Base(); // �θ� ��ü ����

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
	// �� ��� Base ��ü�� �̹� �����Ǿ��� ������ Derived �����ڰ� �Ҹ��� �ʴ´�.
	// �����ڰ� �Ҹ��� �ʾұ� ������ ����ȯ �� Ÿ���� ����. dynamic_Derived �� nullptr


	// �ڽ�Ŭ������ �����ڷ� ������ �θ�Ŭ���� �����Ͱ� ����Ű�� �ִ� Ŭ������ �ڽ�Ŭ������
	// ����ȯ �� ��
	cout << "< case 2 >" << endl;
	Base* dynamic_base2 = new Derived(); // ��ĳ����

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
	// ���� �ٸ� ���� �θ�Ŭ������ ��ü ������ �� ��ĳ������ �ߴٴ� ���̴�.
	// �θ�Ŭ������ ���� ����Ű�� Ŭ����(new �� ���� ���� Ŭ����)�� �ڱ� �ڽ�Ŭ���� �̹Ƿ�
	// dynamic_cast �� ���ؼ� �����ϰ� �ٿ�ĳ������ �����ϴ�.
	
	// dynamic_cast<Base*>(Derived) �� ���� ������� ��ĳ���õ� �����ϴ�.
	cout << endl;



	// const_cast
	// const_cast �� ������ or �������� �����(const) �� ��� �������ִµ� ���ȴ�.
	// volatile Ű���带 ��� �����ؾ� �� ���� ����Ѵ�.
	// �ٸ� ĳ��Ʈ �����ڰ� �ϴ� ����ȯ�� �Ұ����ϰ� ������ ������� �����ϴ� ��쿡�� ���.
	// �ٸ� ĳ��Ʈ �����ڴ� ������� �������� ���Ѵ�.
	// �Լ� �����Ϳ��� ����� �Ұ���
	cout << "< const_cast >" << endl;


	// ������ ����� ���� (const char*)
	cout << "< case 1 >" << endl;
	char str[] = "ABC Text";
	const char* ptr = str;
	cout << "Before : " << str << endl;

	// ptr[0] = 'Q';  �̷������� ����ϴ� ��� �翬������ ������ ����.
	
	char* const_char = const_cast<char*>(ptr);
	const_char[0] = 'Q';
	cout << "After : " << str << endl;


	// ������ ����� ���� (const int&)
	cout << "< case 2 >" << endl;
	int const_int = 15;
	const int& ref = const_int;
	cout << "Before : " << const_int << endl;

	int& ref_int = const_cast<int&>(ref);
	ref_int = 30;
	cout << "After : " << const_int << endl;

	// �̷������� ������ or ���۷��� ������� ��� �����ϴ� ��쿡�� ����Ѵ�.
	cout << endl;



	// reinterpret_cast
	// reinterpret_cast �� ������ Ÿ�Գ��� ��ȯ�� ����ϴ� ĳ��Ʈ �������̴�.
	// �߿��� �� ���������� �����ͷ� �ٲ���� �� �ִٴ� ���̴�. ( �翬�� ���� )
	// �ٸ� ������ �����ͷ� ������ �����ϱ� ������ �߸� ����ϴ� ��쿡�� ��� ����
	// ���󰪰� �ٸ� �� �ִ�. �� �����ڴ� Ȯ���ϰ� ��ȯ�� ������ ��Ȳ���� ����������
	// ����ϴ� ���� �ٶ����ϴ�.

	cout << "< reinterpret_cast >" << endl;
	int reinterpret_int = 12456789;
	int* ptr_int;
	ptr_int = reinterpret_cast<int*>(reinterpret_int);
	// int -> int* �� ����ȯ
	// �� ��� ���� reinterpret_int �� ���� �����ּҸ� �޴� ptr_int ������ ������
	// ��� ���� ����ų �� �𸣱� ������ �����ϴ�.

	int* ptr_int2 = &reinterpret_int;
	char* reinterpret_char;
	reinterpret_char = reinterpret_cast<char*>(ptr_int2);
	cout << "char reinterpret_cast<char*>(int*) : " << *reinterpret_char << endl;
	printf("int reinterpret_cast<char*>(int*) : %d\n", *reinterpret_char);
	// int* -> char* �� ����ȯ
	// �ƴϳ� �ٸ��� ���󰪰� �ٸ� ���� ���´�.

	struct test {
		int a;
	};

	test test;
	test.a = 10;
	int* ptr_int3;
	ptr_int3 = reinterpret_cast<int*>(&test);
	cout << "reinterpret_cast<int*>(struct) : " << *ptr_int3 << endl;
	// ����ü ���� int -> int* ����ȯ
	// ����ü test ���� ������ ������ �ϳ��� �������� ptr_int3 �� test ����ü ����
	// a ������ �������� �� ����Ű�� �ִ�.

}