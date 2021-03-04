#include "PrintManager.h"

int main() {
	cout << "SingleTon Test\n";
	PrintManager* Main = new PrintManager();
	Main->Run();
	delete Main;
}