//#include "Header.h"
//#include "SpeialTemp.h"
#include "Header1.h"
//#include "Header2.h"

#include <iostream>
using namespace std;

int main(void)
{	
	//CStak<int,3>* _intstack=new CStak<int,3>();	// SpeialTemp ����� ���� ���ø� Ŭ������ �Ҵ���
	//int pdata;	// ������ ����
	//_intstack->Push(10);
	//_intstack->Push(20);
	//_intstack->Push(30);
	//_intstack->Push(44);
	//_intstack->Push(55);
	//_intstack->Push(66);	
	//// ������ ���ÿ� ���� �Լ�
	//for(int i=0; i<6; i++)
	//{
	//	_intstack->Pop(pdata);
	//	cout << pdata << endl;
	//} // ������ ���� for���� ������ ������� ������

//==============================================================

	//CStak<int, 5> stacklst;		// �Ϲ� ���ø� Ŭ����
	//CStak<char*, 5> stacklst2;	// �κ� Ư��ȭ ���ø� Ŭ����

	//char* str=new char[10];		// ������ str ���� �޸� �Ҵ�
	//char* strdata=nullptr;		// ������ strdata ���� nullptr �ʱ�ȭ

	//strcpy(str, "aaaa");		// ���� str �� aaaa ���ڿ� ����
	//stacklst2.Push(str);		// ���ÿ� ���� str ���� ( �� �� �ҷ����� �Լ��� �κ� Ư��ȭ ���ø� Ŭ���� )
	//stacklst.Push(3);			// ���ÿ� ������ 3 ���� ( �� �� �ҷ����� �Լ��� �Ϲ� ���ø� Ŭ���� )

	//int data;	// ������ ����
	//
	//stacklst.Pop(data);			// ���ÿ� ����� �� ���� ( �Ϲ� ���ø� )
	//stacklst2.Pop(strdata);		// ���ÿ� ����� �� ���� ( �κ� Ư��ȭ ���ø� )

	//
	//cout << data << endl;
	//cout << strdata << endl;
	//// ������ ���

//=======================================================

	//CStak<char*>* _charstack=new CStak<char*>();	// Header ��� ���� ���� ���ø� �Ҵ�
	//char* pdata;	// ���ڿ� ����
	//_charstack->Push("aa");	// ���ڿ� ����
	//_charstack->Push("bb");	// ``
	//_charstack->Push("cc"); // ``

	//for(int i=0; i<3; i++)
	//{
	//	_charstack->Pop(pdata);		// ���� Ư��ȭ�� ���ø� Ŭ���� ���� �Լ�
	//	cout << pdata << endl;		// ���
	//}	// ���� �� ������ ���� ����

//=======================================================

	//temp<int>* _temp = new temp<int>();		// Header1 ���� ���ø� Ŭ���� ���� �Ҵ�
	//_temp->show();							// ���ø� Ŭ���� ���� �Լ� ȣ��
	//
	//temp<char*>* _temp2=new temp<char*>("hello");	
	//// Headef1 ���� ���ø� Ŭ���� ���� �Ҵ� (error : ���ڿ��� ������ �����ؼ� �־����� ������ const char �� �б� ������ ������)
	//_temp2->show();							// ���ø� Ŭ���� ���� ���ڿ� ���ȭ�� ���� Ư��ȭ ��Ų �Լ��� �켱������ ȣ��


//=======================================================

	//temp<int, int, 3>* _temp=new temp<int, int, 3>();	
	//// �κ� Ư��ȭ ��Ų ���ø� Ŭ������ ��ȣ���� ������ ������ ������ ��
	//// T,T,3 or T,int,3 or int,T,3 ��� ������ ���̱� ������ ��ȣ���� ��
	//// ��ȣ�� �κ� Ư��ȭ�� �ּ� ó���ϰ� �����ϸ� ���������� �۵�
	//_temp->show();

	//temp<double, int, 3>* _temp=new temp<double, int, 3>();	
	//// T,int,3 or double,int,a or double,T,a
	//// �κ� Ư��ȭ���� ��ȣ���� ��� ������ ������ ��
	//_temp->show(); 

	//temp<double, double, 3>* _temp2=new temp<double, double, 3>();
	////// T,T,3 or double,T,a
	////// �κ� Ư��ȭ���� ��ȣ���� ��� ������ ������ ��
	//_temp2->show(); 

	//temp<int, double, 3>* _temp3=new temp<int, double, 3>();
	//// int,T,3
	//// ��ȣ���� ���� �ʱ� ������ ���� �۵�
	//_temp3->show();

	//temp<double, int, 10>* _temp=new temp<double, int, 10>();
	//// double,int,a or double,T,a
	//// ���� ��ȣ���� ���� ������ �����Ϸ��� double,int,a �� �� ���� �ǹ�(���� Ư��ȭ)�� �Ǵ��ϱ� ������ ������ ��
	//// ���� ������ ���ߴ�..
	//_temp->show(); 
	

//=======================================================

	temp<int>* _temp=new temp<int>();		// �Ϲ� ���ø� Ŭ���� ȣ��
	// temp
	_temp->show();
	
	temp<char*>* _temp2=new temp<char*>("hello");		// ���� Ư��ȭ ���ø� Ŭ���� ȣ��
	// temp<char*>
	_temp2->show(); 

	int a=100;
	int* ap=&a;

	temp<int*>* _temp3=new temp<int*>(ap);				// �κ� Ư��ȭ ���ø� Ŭ���� ȣ��
	// temp<T*>
	_temp3->show();

	delete _temp3;
	
	return 0;
}

