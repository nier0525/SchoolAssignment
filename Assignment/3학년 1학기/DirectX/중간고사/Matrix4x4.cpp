#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{
	memset(&Element, 0.f, sizeof(Matrix4x4Element));
}

Matrix4x4::Matrix4x4(const Matrix4x4& matrix)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			Element.Matrix[i][j] = matrix.Element.Matrix[i][j];
}

void Matrix4x4::Matrix4x4_Identity()
{
	Element.m_11 = Element.m_22 = Element.m_33 = Element.m_44 = 1.f;
}

void Matrix4x4::Matrix4x4_Identity(Matrix4x4& _matrix)
{
	_matrix.Element.m_11 = _matrix.Element.m_22 = _matrix.Element.m_33 = _matrix.Element.m_44 = 1.f;
}

void Matrix4x4::Matrix4x4_Transpose()
{
	Matrix4x4 temp;

	temp.Element.m_11 = Element.r1[0];
	temp.Element.m_12 = Element.r1[1];
	temp.Element.m_13 = Element.r1[2];
	temp.Element.m_14 = Element.r1[3];

	temp.Element.m_21 = Element.r2[0];
	temp.Element.m_22 = Element.r2[1];
	temp.Element.m_23 = Element.r2[2];
	temp.Element.m_24 = Element.r2[3];

	temp.Element.m_31 = Element.r3[0];
	temp.Element.m_32 = Element.r3[1];
	temp.Element.m_33 = Element.r3[2];
	temp.Element.m_34 = Element.r3[3];

	temp.Element.m_41 = Element.r4[0];
	temp.Element.m_42 = Element.r4[1];
	temp.Element.m_43 = Element.r4[2];
	temp.Element.m_44 = Element.r4[3];

	*this = temp;
}

void Matrix4x4::Matrix4x4_Translate(Vector3 vector)
{
	Matrix4x4_Identity();
	Element.m_14 = vector.x;
	Element.m_24 = vector.y;
	Element.m_34 = vector.z;
}

void Matrix4x4::Matrix4x4_Translate(float x, float y, float z)
{
	Matrix4x4_Identity();
	Element.m_14 = x;
	Element.m_24 = y;
	Element.m_34 = z;
}

void Matrix4x4::Matrix4x4_Scale(Vector3 vector)
{
	Matrix4x4_Identity();
	Element.m_11 = vector.x;
	Element.m_22 = vector.y;
	Element.m_33 = vector.z;
}

void Matrix4x4::Matrix4x4_Scale(float x, float y, float z)
{
	Matrix4x4_Identity();
	Element.m_11 = x;
	Element.m_22 = y;
	Element.m_33 = z;
}

void Matrix4x4::Matrix4x4_RotateX(float angle)
{
	Matrix4x4_Identity();
	float c = cosf(ToRadian(angle));
	float s = sinf(ToRadian(angle));

	Element.m_22 = c;
	Element.m_23 = -s;
	Element.m_32 = s;
	Element.m_33 = c;
}

void Matrix4x4::Matrix4x4_RotateY(float angle)
{
	Matrix4x4_Identity();
	float c = cosf(ToRadian(angle));
	float s = sinf(ToRadian(angle));

	Element.m_11 = c;
	Element.m_13 = s;
	Element.m_31 = -s;
	Element.m_33 = c;
}

void Matrix4x4::Matrix4x4_RotateZ(float angle)
{
	Matrix4x4_Identity();
	float c = cosf(ToRadian(angle));
	float s = sinf(ToRadian(angle));

	Element.m_11 = c;
	Element.m_12 = -s;
	Element.m_21 = s;
	Element.m_22 = c;
}

void Matrix4x4::Matrix4x4_RotateXYZ(Vector3 vector)
{
	Matrix4x4 X_Angle = Matrix4x4();
	X_Angle.Matrix4x4_Identity();
	X_Angle.Matrix4x4_RotateX(vector.x);


	Matrix4x4 Y_Angle = Matrix4x4();
	Y_Angle.Matrix4x4_Identity();
	Y_Angle.Matrix4x4_RotateY(vector.y);


	Matrix4x4 Z_Angle = Matrix4x4();
	Z_Angle.Matrix4x4_Identity();
	Z_Angle.Matrix4x4_RotateZ(vector.z);

	*this = X_Angle * Y_Angle * Z_Angle;
}

void Matrix4x4::Matrix4x4_RotateXYZ(float x, float y, float z)
{
	Matrix4x4 X_Angle = Matrix4x4();
	X_Angle.Matrix4x4_Identity();
	X_Angle.Matrix4x4_RotateX(x);


	Matrix4x4 Y_Angle = Matrix4x4();
	Y_Angle.Matrix4x4_Identity();
	Y_Angle.Matrix4x4_RotateY(y);


	Matrix4x4 Z_Angle = Matrix4x4();
	Z_Angle.Matrix4x4_Identity();
	Z_Angle.Matrix4x4_RotateZ(z);

	*this = X_Angle * Y_Angle * Z_Angle;
}

void Matrix4x4::Matrix4x4_SetupMatrix(int state, float angle)
{
	Matrix4x4 temp;

	switch (state)
	{
	case MATRIX_ROTATEX:
		temp.Matrix4x4_RotateX(angle);
		break;

	case MATRIX_ROTATEY:
		temp.Matrix4x4_RotateY(angle);
		break;

	case MATRIX_ROTATEZ:
		temp.Matrix4x4_RotateZ(angle);
		break;
	}

	*this *= temp;
}

void Matrix4x4::Matrix4x4_SetupMatrix(int state, Vector3 vector)
{
	Matrix4x4 temp = Matrix4x4();

	switch (state)
	{
	case MATRIX_ROTATEXYZ:
		temp.Matrix4x4_RotateXYZ(vector);
		break;

	case MATRIX_ROTATEX:
		temp.Matrix4x4_RotateX(vector.x);
		break;

	case MATRIX_ROTATEY:
		temp.Matrix4x4_RotateY(vector.y);
		break;

	case MATRIX_ROTATEZ:
		temp.Matrix4x4_RotateZ(vector.z);
		break;

	case MATRIX_SCALE:
		temp.Matrix4x4_Scale(vector);
		break;

	case MATRIX_TRANSLATE:
		temp.Matrix4x4_Translate(vector);
		break;

	default:
		return;
	}

	*this *= temp;
}

void Matrix4x4::Matrix4x4_SetupMatrix(int state, float x, float y, float z)
{
	Matrix4x4 temp;

	switch (state)
	{
	case MATRIX_ROTATEXYZ:
		temp.Matrix4x4_RotateXYZ(x, y, z);
		break;

	case MATRIX_ROTATEX:
		temp.Matrix4x4_RotateX(x);
		break;

	case MATRIX_ROTATEY:
		temp.Matrix4x4_RotateY(y);
		break;

	case MATRIX_ROTATEZ:
		temp.Matrix4x4_RotateZ(z);
		break;

	case MATRIX_SCALE:
		temp.Matrix4x4_Scale(x, y, z);
		break;

	case MATRIX_TRANSLATE:
		temp.Matrix4x4_Translate(x, y, z);
		break;
	}

	*this *= temp;
}

// 연산자 오버로딩

Matrix4x4 operator+(Matrix4x4 a, Matrix4x4 b)
{
	Matrix4x4 temp;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			temp.Element.Matrix[i][j] = a.Element.Matrix[i][j] + b.Element.Matrix[i][j];

	return temp;
}

void operator+=(Matrix4x4& a, Matrix4x4 b)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			a.Element.Matrix[i][j] += b.Element.Matrix[i][j];
}

Matrix4x4 operator-(Matrix4x4 a, Matrix4x4 b)
{
	Matrix4x4 temp;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			temp.Element.Matrix[i][j] = a.Element.Matrix[i][j] - b.Element.Matrix[i][j];

	return temp;
}

void operator-=(Matrix4x4& a, Matrix4x4 b)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			a.Element.Matrix[i][j] -= b.Element.Matrix[i][j];
}

Matrix4x4 operator*(Matrix4x4 a, Matrix4x4 b)
{
	Matrix4x4 temp;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			temp.Element.Matrix[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				temp.Element.Matrix[i][j] += a.Element.Matrix[i][k] * b.Element.Matrix[k][j];
			}
		}
	}

	return temp;
}

void operator*=(Matrix4x4& a, Matrix4x4 b)
{
	Matrix4x4 temp;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			temp.Element.Matrix[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				temp.Element.Matrix[i][j] += a.Element.Matrix[i][k] * b.Element.Matrix[k][j];
			}
		}
	}

	a = temp;
}

Vector3 operator*(Vector3 vector, Matrix4x4 m)
{
	return Vector3(
		m.Element.m_11 * vector.x + m.Element.m_12 * vector.y + m.Element.m_13 * vector.z + m.Element.m_14 * vector.w,
		m.Element.m_21 * vector.x + m.Element.m_22 * vector.y + m.Element.m_23 * vector.z + m.Element.m_24 * vector.w,
		m.Element.m_31 * vector.x + m.Element.m_32 * vector.y + m.Element.m_33 * vector.z + m.Element.m_34 * vector.w
	);
}