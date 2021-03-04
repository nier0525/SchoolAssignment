#pragma once

#include "Planet.h"

class CEarthSystem : public CPlanet
{
public:

public:
	CEarthSystem() {

	};

	~CEarthSystem()
	{
		Release();
	};

	void UpData()
	{
		D3DXVECTOR3 distance = D3DXVECTOR3(50, 0, 0);

		D3DXMATRIXA16 matWorld = Moon->begin()->second->m_matWorld;
		CPlanet* ptr = Moon->find((char*)"Moon")->second;

		if (ptr != nullptr)
		{
			ptr->Rotating(2000, distance);
			ptr->Revolution(matWorld, 2000, distance);
		}
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
	}

	void Release()
	{
		Moon->clear();
		delete Moon;
		delete this;
	}
};