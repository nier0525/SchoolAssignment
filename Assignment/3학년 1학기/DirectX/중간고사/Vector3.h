#pragma once
#include "Global.h"
#include <math.h>

class Vector3 {
public:
	float x;
	float y;
	float z;
	float w;

public:
	Vector3();
	~Vector3();
	Vector3(float, float, float);

	float Length();
	float Length(Vector3);
	Vector3 Normalize();
	Vector3 Normalize(Vector3);
	float GetAngle(Vector3, Vector3);
	float Seta(float);

	float Dot(Vector3, Vector3);
	Vector3 Cross(Vector3, Vector3);

	Vector3 Proj(Vector3, Vector3);
	Vector3 Orthogonal_Projection(Vector3, Vector3);

	friend Vector3 operator+(Vector3, Vector3);
	friend void operator+=(Vector3&, Vector3);

	friend Vector3 operator-(Vector3, Vector3);
	friend void operator-=(Vector3&, Vector3);

	friend Vector3 operator*(Vector3, float);
	friend void operator*=(Vector3&, float);

	friend bool operator==(Vector3, Vector3);
};