#pragma once
#include "MyMath.h"

class MyVector3 {
private:
	float x;
	float y;
	float z;
public:
	MyVector3();
	~MyVector3();
	MyVector3(float, float, float);

	float Length();
	float Length(MyVector3);
	MyVector3 Normalize();
	MyVector3 Normalize(MyVector3);
	float GetAngle(MyVector3, MyVector3);
	float Seta(float);

	float Dot(MyVector3, MyVector3);
	MyVector3 Cross(MyVector3, MyVector3);

	MyVector3 Proj(MyVector3, MyVector3);
	MyVector3 Orthogonal_Projection(MyVector3, MyVector3);

	void SetVector(float _x, float _y, float _z) { x = _x; y = Viewport_Convert_Y(_y); z = _z; };
	void SetX(float _x) { x = _x; };
	void SetY(float _y) { y = Viewport_Convert_Y(_y); };
	void SetZ(float _z) { z = _z; };

	float GetX() { return x; };
	float GetY() { return Viewport_Convert_Y(y); };
	float GetZ() { return z; };

	friend MyVector3 operator+(MyVector3, MyVector3);
	friend void operator+=(MyVector3&, MyVector3);

	friend MyVector3 operator-(MyVector3, MyVector3);
	friend void operator-=(MyVector3&, MyVector3);

	friend MyVector3 operator*(MyVector3, float);
	friend void operator*=(MyVector3&, float);

	friend bool operator==(MyVector3, MyVector3);
};