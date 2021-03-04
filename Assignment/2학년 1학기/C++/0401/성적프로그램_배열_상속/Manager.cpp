#include "Manager.h"

Manager::Manager() {
	memset(PGlist, 0, sizeof(PGlist));
	memset(GPlist, 0, sizeof(GPlist));
	memset(PDlist, 0, sizeof(PDlist));
	
	PGcount = 0;
	GPcount = 0;
	PDcount = 0;
}

Manager::~Manager() {
	for (int i = 0; i < PGcount; i++) {
		if (PGlist[i] != nullptr) {
			delete PGlist[i];
		}
	}
	for (int i = 0; i < GPcount; i++) {
		if (GPlist[i] != nullptr) {
			delete GPlist[i];
		}
	}
	for (int i = 0; i < PDcount; i++) {
		if (PDlist[i] != nullptr) {
			delete PDlist[i];
		}
	}
}