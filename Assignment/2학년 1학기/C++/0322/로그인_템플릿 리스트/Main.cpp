#include "MainManager.h"

int main() {
	Manager* Main = new Manager();
	Main->Run();
	delete Main;
}