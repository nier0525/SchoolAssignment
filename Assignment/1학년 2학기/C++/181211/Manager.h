#ifndef CMANAGER_H
#define CMANAGER_H
#include "Base.h"
#include "Data.h"

class cManager {
private:
	cFruit*		mFruit[ASIZE]; // 객체 멤버 변수
public:
	cManager(); // 생성자
	~cManager(); // 소멸자

	void Input(); 
	void Output();
	void Show();
	void ShowMenu();
	
	void Run();
	// 함수
};

#endif
