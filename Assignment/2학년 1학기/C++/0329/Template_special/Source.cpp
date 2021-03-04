//#include "Header.h"
//#include "SpeialTemp.h"
#include "Header1.h"
//#include "Header2.h"

#include <iostream>
using namespace std;

int main(void)
{	
	//CStak<int,3>* _intstack=new CStak<int,3>();	// SpeialTemp 헤더의 스택 템플릿 클래스를 할당함
	//int pdata;	// 정수형 변수
	//_intstack->Push(10);
	//_intstack->Push(20);
	//_intstack->Push(30);
	//_intstack->Push(44);
	//_intstack->Push(55);
	//_intstack->Push(66);	
	//// 정수값 스택에 삽입 함수
	//for(int i=0; i<6; i++)
	//{
	//	_intstack->Pop(pdata);
	//	cout << pdata << endl;
	//} // 삽입한 것을 for문을 돌려서 순서대로 삭제함

//==============================================================

	//CStak<int, 5> stacklst;		// 일반 템플릿 클래스
	//CStak<char*, 5> stacklst2;	// 부분 특수화 템플릿 클래스

	//char* str=new char[10];		// 문자형 str 변수 메모리 할당
	//char* strdata=nullptr;		// 문자형 strdata 변수 nullptr 초기화

	//strcpy(str, "aaaa");		// 변수 str 에 aaaa 문자열 대입
	//stacklst2.Push(str);		// 스택에 변수 str 삽입 ( 이 때 불러오는 함수는 부분 특수화 템플릿 클래스 )
	//stacklst.Push(3);			// 스택에 정수값 3 삽입 ( 이 때 불러오는 함수는 일반 템플릿 클래스 )

	//int data;	// 정수형 변수
	//
	//stacklst.Pop(data);			// 스택에 저장된 값 삭제 ( 일반 템플릿 )
	//stacklst2.Pop(strdata);		// 스택에 저장된 값 삭제 ( 부분 특수화 템플릿 )

	//
	//cout << data << endl;
	//cout << strdata << endl;
	//// 변수값 출력

//=======================================================

	//CStak<char*>* _charstack=new CStak<char*>();	// Header 헤더 파일 내에 템플릿 할당
	//char* pdata;	// 문자열 변수
	//_charstack->Push("aa");	// 문자열 삽입
	//_charstack->Push("bb");	// ``
	//_charstack->Push("cc"); // ``

	//for(int i=0; i<3; i++)
	//{
	//	_charstack->Pop(pdata);		// 완전 특수화한 템플릿 클래스 내에 함수
	//	cout << pdata << endl;		// 출력
	//}	// 스택 내 데이터 삭제 연산

//=======================================================

	//temp<int>* _temp = new temp<int>();		// Header1 내의 템플릿 클래스 동적 할당
	//_temp->show();							// 템플릿 클래스 내의 함수 호출
	//
	//temp<char*>* _temp2=new temp<char*>("hello");	
	//// Headef1 내의 템플릿 클래스 동적 할당 (error : 문자열을 변수로 선언해서 넣어주지 않으면 const char 로 읽기 때문에 에러남)
	//_temp2->show();							// 템플릿 클래스 내의 문자열 명시화한 완전 특수화 시킨 함수를 우선적으로 호출


//=======================================================

	//temp<int, int, 3>* _temp=new temp<int, int, 3>();	
	//// 부분 특수화 시킨 템플릿 클래스가 모호성을 가지기 때문에 오류가 남
	//// T,T,3 or T,int,3 or int,T,3 모두 가능한 식이기 때문에 모호성을 띔
	//// 모호한 부분 특수화를 주석 처리하고 실행하면 정상적으로 작동
	//_temp->show();

	//temp<double, int, 3>* _temp=new temp<double, int, 3>();	
	//// T,int,3 or double,int,a or double,T,a
	//// 부분 특수화에서 모호성을 띄기 때문에 오류가 남
	//_temp->show(); 

	//temp<double, double, 3>* _temp2=new temp<double, double, 3>();
	////// T,T,3 or double,T,a
	////// 부분 특수화에서 모호성을 띄기 때문에 오류가 남
	//_temp2->show(); 

	//temp<int, double, 3>* _temp3=new temp<int, double, 3>();
	//// int,T,3
	//// 모호성을 띄지 않기 때문에 정상 작동
	//_temp3->show();

	//temp<double, int, 10>* _temp=new temp<double, int, 10>();
	//// double,int,a or double,T,a
	//// 본래 모호성을 띄어야 하지만 컴파일러가 double,int,a 를 더 좁은 의미(완전 특수화)로 판단하기 때문에 실행은 댐
	//// 원래 오류가 떠야댐..
	//_temp->show(); 
	

//=======================================================

	temp<int>* _temp=new temp<int>();		// 일반 템플릿 클래스 호출
	// temp
	_temp->show();
	
	temp<char*>* _temp2=new temp<char*>("hello");		// 완전 특수화 템플릿 클래스 호출
	// temp<char*>
	_temp2->show(); 

	int a=100;
	int* ap=&a;

	temp<int*>* _temp3=new temp<int*>(ap);				// 부분 특수화 템플릿 클래스 호출
	// temp<T*>
	_temp3->show();

	delete _temp3;
	
	return 0;
}

