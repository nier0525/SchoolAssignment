#pragma once

#include "Vector3.h"

struct Matrix4x4Element
{
public:
	union
	{
		struct {
			float m_11, m_12, m_13, m_14;
			float m_21, m_22, m_23, m_24;
			float m_31, m_32, m_33, m_34;
			float m_41, m_42, m_43, m_44;
		};

		struct {
			float r1[4], r2[4], r3[4], r4[4];
		};

		struct {
			float Matrix[4][4];
		};
	};
};

class Matrix4x4
{
public:
	Matrix4x4Element Element;

private:
	void Matrix4x4_Translate(Vector3 vector);
	void Matrix4x4_Translate(float x, float y, float z);

	void Matrix4x4_Scale(Vector3 vector);
	void Matrix4x4_Scale(float x, float y, float z);

	void Matrix4x4_RotateX(float angle);
	void Matrix4x4_RotateY(float angle);
	void Matrix4x4_RotateZ(float angle);

	void Matrix4x4_RotateXYZ(Vector3 vector);
	void Matrix4x4_RotateXYZ(float x, float y, float z);

public:
	Matrix4x4();
	Matrix4x4(const Matrix4x4& matrix);

	~Matrix4x4() {}

	void Matrix4x4_Identity();
	void Matrix4x4_Identity(Matrix4x4& _matrix);

	void Matrix4x4_Transpose();

	void Matrix4x4_SetupMatrix(int state, float angle);
	void Matrix4x4_SetupMatrix(int state, Vector3 vector);
	void Matrix4x4_SetupMatrix(int state, float x, float y, float z);

	friend Matrix4x4 operator+(Matrix4x4 a, Matrix4x4 b);
	friend void operator+=(Matrix4x4& a, Matrix4x4 b);

	friend Matrix4x4 operator-(Matrix4x4 a, Matrix4x4 b);
	friend void operator-=(Matrix4x4& a, Matrix4x4 b);

	friend Matrix4x4 operator*(Matrix4x4 a, Matrix4x4 b);
	friend void operator*=(Matrix4x4& a, Matrix4x4 b);
};

Vector3 operator*(Vector3 vector, Matrix4x4 m);
