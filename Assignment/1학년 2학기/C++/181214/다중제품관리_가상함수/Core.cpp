#include "MainManager.h"

int main() {
	cMain* Program = new cMain();
	Program->Run();

	delete Program;
	return 0;
}