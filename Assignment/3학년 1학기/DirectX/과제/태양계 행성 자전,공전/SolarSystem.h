#pragma once

#include "EarthSystem.h"

class CSolarSystem : public CPlanet
{
public:
	CEarthSystem* m_earth;
public:
	CSolarSystem() {
		m_earth = new CEarthSystem();
	};

	~CSolarSystem() 
	{
		Release();
	};

	void UpData()
	{
		D3DXMATRIXA16 matWolrd;		
		D3DXVECTOR3 distance = D3DXVECTOR3(0, 0, 0);
		CPlanet* ptr = Moon->find((char*)"Sun")->second;
		
		if (ptr != nullptr)
		{
			ptr->Rotating(30000, distance);
		}

		matWolrd = Moon->begin()->second->m_matWorld;

		ptr = Moon->find((char*)"Mercury")->second;

		if (ptr != nullptr)
		{
			distance = D3DXVECTOR3(250, 0, 0);
			ptr->Rotating(500, distance);
			ptr->Revolution(matWolrd, 10000, distance);
		}

		ptr = Moon->find((char*)"Venus")->second;

		if (ptr != nullptr)
		{
			distance = D3DXVECTOR3(500, 0, 0);
			ptr->Rotating(750, distance);
			ptr->Revolution(matWolrd, 20000, distance);
		}

		ptr = Moon->find((char*)"Earth")->second;

		if (ptr != nullptr)
		{
			distance = D3DXVECTOR3(750, 0, 0);
			ptr->Rotating(1000, distance);
			ptr->Revolution(matWolrd, 30000, distance);

			m_earth->Moon->begin()->second->m_matWorld = ptr->m_matWorld;
		}

		ptr = Moon->find((char*)"Mars")->second;

		if (ptr != nullptr)
		{
			distance = D3DXVECTOR3(1000, 0, 0);
			ptr->Rotating(2000, distance);
			ptr->Revolution(matWolrd, 45000, distance);
		}

		ptr = Moon->find((char*)"Jupiter")->second;

		if (ptr != nullptr)
		{
			distance = D3DXVECTOR3(1350, 0, 0);
			ptr->Rotating(5000, distance);
			ptr->Revolution(matWolrd, 85000, distance);
		}

		ptr = Moon->find((char*)"Saturn")->second;

		if (ptr != nullptr)
		{
			distance = D3DXVECTOR3(1650, 0, 0);
			ptr->Rotating(5000, distance);
			ptr->Revolution(matWolrd, 100000, distance);
		}

		ptr = Moon->find((char*)"Uranus")->second;

		if (ptr != nullptr)
		{
			distance = D3DXVECTOR3(2000, 0, 0);
			ptr->Rotating(10000, distance);
			ptr->Revolution(matWolrd, 150000, distance);
		}

		ptr = Moon->find((char*)"Neptune")->second;

		if (ptr != nullptr)
		{
			distance = D3DXVECTOR3(2500, 0, 0);
			ptr->Rotating(20000, distance);
			ptr->Revolution(matWolrd, 200000, distance);
		}

		m_earth->UpData();
	}

	void Render(LPDIRECT3DDEVICE9 device, D3DMATERIAL9 material)
	{
		for (auto i = Moon->begin(); i != Moon->end(); i++)
		{
			material.Diffuse.r = material.Ambient.r = i->second->m_Data.m_color.r;
			material.Diffuse.g = material.Ambient.g = i->second->m_Data.m_color.g;
			material.Diffuse.b = material.Ambient.b = i->second->m_Data.m_color.b;

			device->SetTransform(D3DTS_WORLD, &i->second->m_matWorld);
			device->SetMaterial(&material);
			
			i->second->m_Data.Draw();
		}

		m_earth->Render(device, material);
	}

	void Release() 
	{
		m_earth->Release();
		Moon->clear();
		delete Moon;
		delete this;
	}
};