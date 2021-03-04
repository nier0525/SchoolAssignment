#include "Manager.h"

int main() {
	cManager* program = new cManager(); // 객체 변수 메모리 할당
	program->Run(); // Run() 함수 실행

	delete program; // 메모리 해제
	return 0;
}