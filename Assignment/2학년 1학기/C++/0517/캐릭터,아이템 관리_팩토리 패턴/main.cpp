
#include "MainManager.h"

int main() {
	MainManager::GetInstance()->Init();
	MainManager::GetInstance()->Run();
	MainManager::Destory();
}