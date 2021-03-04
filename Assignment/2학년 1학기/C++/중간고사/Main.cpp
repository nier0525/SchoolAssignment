#include "GameManager.h"

int main() {
	GameManager* System = new GameManager(); // GameManager 동적 할당
	System->Run(); // Run 함수 실행
	delete System; // Run 이 끝나면 메모리 반환
}