#pragma once
#include "Sector.h"
#include "ClientSection.h"

char Map[18][21] = {
	"####################",
	"####################",
	"####################",
	"#####   ############",
	"#####O  ############",
	"#####  O############",
	"###  O@  ###########",
	"### # ## ###########",
	"#   # ## #####  ..##",
	"# O             ..##",
	"##### ### # ##  ..##",
	"#####     ##########",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################"
};

class CSectorManager
{
private:
	vector<CSector*>* vSector;
public:
	CSectorManager()
	{
		vSector = nullptr;
	}
	~CSectorManager()
	{
		if (vSector != nullptr) delete vSector;
	}

	void Begin();
	void BeginSector();

	void End()
	{
		vSector->clear();
	}


	void UpdateSector(CClientSection* ptr);
	void MoveSector(CClientSection* ptr);
};
