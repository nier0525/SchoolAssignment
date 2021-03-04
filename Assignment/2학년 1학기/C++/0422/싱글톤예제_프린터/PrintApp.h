#pragma once

#include "Singleton.h"

class PrintApp : Singleton<PrintApp> {
private:
	char write[CHARSIZE];
public:
	PrintApp();
	~PrintApp();

	void SetWrite(const char*);
	const char* GetWrite();
};