#include "MainManager.h"

int main() {
	MainManager* Main = new MainManager();
	Main->run();
	delete Main;
}