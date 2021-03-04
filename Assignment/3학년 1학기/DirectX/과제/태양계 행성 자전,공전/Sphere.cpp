#include "Sphere.h"

CSphere::CSphere()
{
	m_Sphere.vCenter = D3DXVECTOR3(0, 0, 0);
	m_Sphere.fRadius = 0;

	m_mesh = nullptr;
}

CSphere::CSphere(const CSphere& sphere)
{
	*this = sphere;
}

CSphere::CSphere(SPHERE sphere, LPD3DXMESH mesh)
{
	m_Sphere = sphere;
	m_mesh = mesh;
}

CSphere::~CSphere()
{

}

bool CSphere::Create(LPDIRECT3DDEVICE9 device, float slices, float stacks)
{
	if (FAILED(D3DXCreateSphere(device, m_Sphere.fRadius, slices, stacks, &m_mesh, nullptr)))
	{
		return false;
	}

	return true;
}

void CSphere::Draw()
{
	m_mesh->DrawSubset(0);
}

void CSphere::Release()
{
	if (m_mesh != nullptr)
		m_mesh->Release();
}