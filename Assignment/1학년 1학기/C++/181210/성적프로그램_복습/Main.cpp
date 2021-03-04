#include "MainManager.h"

int main() {
	cMainManager* program = new cMainManager();
	program->run();
	delete program;
	return 0;
}