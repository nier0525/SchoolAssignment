#pragma once

#include "Sphere.h"

class CPlanet
{
public:
	char name[128];

	CSphere m_Data;
	D3DXMATRIXA16 m_matWorld;

	map<char*, CPlanet*>* Moon;
public:
	CPlanet();
	CPlanet(const CPlanet& ptr);
	CPlanet(const char* _name, CSphere data);

	void CreateMoon(char* name, CPlanet* planet);
	void Rotating(const int Rotating_speed, const D3DXVECTOR3 dis);
	void Revolution(const D3DXMATRIXA16& matrix, const int Revolution_speed, const D3DXVECTOR3 dis);
	void RotatingRevolution(const D3DXMATRIXA16& matrix, const int Rotating_speed, const int Revolution_speed, const D3DXVECTOR3 dis);

	~CPlanet()
	{
		Moon->clear();
	}
};