#include "Planet.h"

CPlanet::CPlanet()
{
	ZeroMemory(name, sizeof(name));
	m_Data = CSphere();
	D3DXMatrixIdentity(&m_matWorld);

	Moon = new map<char*, CPlanet*>();
}

CPlanet::CPlanet(const CPlanet& ptr)
{
	*this = ptr;
}

CPlanet::CPlanet(const char* _name, CSphere data)
{
	strcpy(name, _name);
	m_Data = data;
	D3DXMatrixIdentity(&m_matWorld);

	Moon = new map<char*, CPlanet*>();
}

void CPlanet::CreateMoon(char* name, CPlanet* planet)
{
	Moon->insert(make_pair(name, planet));
}

void CPlanet::Rotating(const int Rotating_speed, const D3DXVECTOR3 dis)
{
	D3DXMATRIXA16 MatRot, MatRot2, MatTran;

	D3DXMatrixRotationY(&MatRot, fAngle(Rotating_speed));
	D3DXMatrixTranslation(&MatTran, dis.x, dis.y, dis.z);

	m_matWorld = MatRot * MatTran;
}

void CPlanet::Revolution(const D3DXMATRIXA16& matrix ,const int Revolution_speed, const D3DXVECTOR3 dis)
{
	D3DXMATRIXA16 MatRot, MatTran, MatWorld;
	D3DXMatrixRotationY(&MatRot, fAngle(Revolution_speed));
	D3DXMatrixTranslation(&MatTran, dis.x, dis.y, dis.z);

	MatWorld = m_matWorld * MatRot * matrix;
	m_matWorld = MatWorld;
}

void CPlanet::RotatingRevolution(const D3DXMATRIXA16& matrix, const int Rotating_speed, const int Revolution_speed, const D3DXVECTOR3 dis)
{
	D3DXMATRIXA16 MatRot, MatRot2, MatTran;

	D3DXMatrixRotationY(&MatRot, fAngle(Rotating_speed));
	D3DXMatrixRotationY(&MatRot2, fAngle(Revolution_speed));
	D3DXMatrixTranslation(&MatTran, dis.x, dis.y, dis.z);

	m_matWorld = MatRot * MatTran * matrix;
}
