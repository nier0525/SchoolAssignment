#include "Template.h"

// �κ� Ư��ȭ�� ���� ������ ��ü
class Test {
private:
	int data;
public:
	Test(int _data) { data = _data; };
	int GetData() { return data; }
};

int main() {
	AddProgram<int>* intptr = new AddProgram<int>();
	AddProgram<double>* doubleptr = new AddProgram<double>();

	Test* test = new Test(90);
	AddProgram<Test*>* ptr = new AddProgram<Test*>();

	AddProgram<char*>* charptr = new AddProgram<char*>();

	// �Ϲ� ���ø� int , double
	intptr->Add(30, 30);
	cout << intptr->GetData() << endl;
	doubleptr->Add(3.5, 3.7);
	cout << doubleptr->GetData() << endl;

	// �κ� Ư��ȭ ��ü
	ptr->Add(test);
	cout << ptr->GetData()->GetData() << endl;

	// ���� Ư��ȭ char
	char a[20] = { "Hello" };
	char b[20] = { " C++" };

	charptr->Add(a,b);
	cout << charptr->GetData() << endl;

	delete intptr, doubleptr, charptr, test, ptr;
}