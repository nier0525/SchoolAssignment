#pragma once

#include "PrintApp.h"

class PrintManager {
private:
	Singleton<PrintApp>* PrintM;
public:
	PrintManager();
	~PrintManager();

	void Run();

	int Menu();
};