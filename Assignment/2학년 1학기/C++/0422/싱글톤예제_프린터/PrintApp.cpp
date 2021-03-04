#include "PrintApp.h"

PrintApp::PrintApp() {
	memset(write, 0, sizeof(write));
}

PrintApp::~PrintApp() {

}

void PrintApp::SetWrite(const char* _write) {
	strcpy(write, _write);
}

const char* PrintApp::GetWrite() {
	return write;
}