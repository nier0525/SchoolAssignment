#include "SectorManager.h"

void CSectorManager::Begin()
{
	vSector = new vector<CSector*>();
	int id = 0;

	for (int i = 0; i < (18 * 21); i++)
	{
		CSector* sector = new CSector(id++);
		vSector->push_back(sector);
	}

	int vx = 0;
	int vy = 0;

	for (int i = 0; i < vSector->size(); i++)
	{
		vx = i % 21;
		if (i % 21 == 0 && i != 0)
			++vy;

		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				int check_x = vx + x;
				int check_y = vy + y;

				if (check_x >= 0 && check_x < 21 && check_y >= 0 && check_y < 18)
				{
					int index = check_x + (check_y * 21);
					vSector->at(i)->Insert_AlmostSector(vSector->at(index));
				}
			}
		}
	}

	BeginSector();
}

void CSectorManager::BeginSector()
{
	int x = 0;
	int y = 0;

	for (int i = 0; i < vSector->size(); i++)
	{
		x = i % 21;
		if (i % 21 == 0 && i != 0)
			++y;

		if (Map[y][x] == '@' || Map[y][x] == 'O')
		{
			vSector->at(i)->OnEnter(Map[y][x]);
		}
	}
}

void CSectorManager::UpdateSector(CClientSection* ptr)
{
	/*
	if (GetProtocol() == MOVE)
		MoveSector(ptr);

	for (auto data = vSector->at(ptr->GetID)->GetAlmostSectorList()->begin(); data != vSector->at(ptr->GetID())->GetAlmostSectorList()->end(); data++)
	{
		CSector* almost = *data;
		almost->Send();
	}
	*/
}

void CSectorManager::MoveSector(CClientSection* ptr)
{
	/*
	// 현재 플레이어가 자신의 셉터 영역을 이탈하고 다른 셉터 영역에 들어갔는 지 확인
	if (. . .)
	{
		int pre_id = ptr->GetID();

		// 어느 방향으로 이동했는 지 확인 후, Client 의 SectorID 값 변경
		if ()
		else if ()
		. . .

		vSector->at(pre_id)->OnExit(ptr);
		vSector->at(ptr->GetID())->OnEnter(ptr);

		// 이전 셉터의 인급 셉터들에게 데이터 전송
		for (auto data = vSector->at(pre_id)->GetAlmostSectorList()->begin(); data != vSector->at(pre_id)->GetAlmostSectorList()->end(); data++)
		{
			CSector* almost = *data;
			almost->Send();
		}
	}
	else
		return;
	*/
}
