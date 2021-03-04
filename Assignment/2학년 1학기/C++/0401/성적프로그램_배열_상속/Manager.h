#pragma once

#include "Student.h"

class Manager {
protected:
	CStudent* PGlist[STUDENTSIZE];
	CStudent* GPlist[STUDENTSIZE];
	CStudent* PDlist[STUDENTSIZE];

	int PGcount;
	int GPcount;
	int PDcount;

public:
	Manager();
	~Manager();
};