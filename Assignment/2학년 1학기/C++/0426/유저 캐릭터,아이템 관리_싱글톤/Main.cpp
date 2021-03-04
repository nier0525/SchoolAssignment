#include "MainManager.h"

int main() {
	MainManager::GetInstance()->Run();
	MainManager::Destory();
}