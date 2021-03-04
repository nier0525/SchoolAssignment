#pragma once

#include "Global.h"

class CSphere
{
public:
	SPHERE m_Sphere;
	LPD3DXMESH m_mesh;
	COLOR m_color;
public:
	CSphere();
	CSphere(const CSphere& sphere);
	CSphere(SPHERE sphere, LPD3DXMESH mesh);

	~CSphere();

	bool Create(LPDIRECT3DDEVICE9 device, float slices, float stacks);
	void Draw();
	void Release();
};