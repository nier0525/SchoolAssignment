#include "Vector3.h"

Vector3::Vector3() {
	x = y = z = 0;
	w = 1;
}

Vector3::~Vector3() {

}

Vector3::Vector3(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
	w = 1;
}

float Vector3::Length() {
	return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
}

float Vector3::Length(Vector3 _vector) {
	return sqrtf(powf(_vector.x, 2) + powf(_vector.y, 2) + powf(_vector.z, 2));
}

Vector3 Vector3::Normalize() {
	float len = Length();
	if (len == 0)
		return *this;
	else
		return Vector3(x / len, y / len, z / len);
}

Vector3 Vector3::Normalize(Vector3 _vector) {
	float len = Length(_vector);
	if (len == 0)
		return _vector;
	else
		return Vector3(_vector.x / len, _vector.y / len, _vector.z / len);
}

float Vector3::GetAngle(Vector3 _vector1, Vector3 _vector2) {
	float cos_Angle = Dot(_vector1, _vector2);
	cos_Angle = Seta(cos_Angle);

	Vector3 Cross_Vector = Cross(_vector1, _vector2);
	float Angle = (Cross_Vector.z > 0.0f) ? cos_Angle : -cos_Angle;
	return Angle;
}

float Vector3::Seta(float cos) {
	return acos(cos) * 180 / PI;
}

float Vector3::Dot(Vector3 _vector1, Vector3 _vector2) {
	_vector1 = Normalize(_vector1);
	_vector2 = Normalize(_vector2);

	if (_vector1 == _vector2) return 1;
	else if (Length(_vector1 + _vector2) == 0) return -1;

	float x = _vector1.x * _vector2.x;
	float y = _vector1.y * _vector2.y;
	float z = _vector1.z * _vector2.z;

	return x + y + z;
}

Vector3 Vector3::Cross(Vector3 _vector1, Vector3 _vector2) {
	Vector3 Result;

	Result.x = _vector1.y * _vector2.z - _vector1.z * _vector2.y;
	Result.y = _vector1.z * _vector2.x - _vector1.x * _vector2.z;
	Result.z = _vector1.x * _vector2.y - _vector1.y * _vector2.x;

	return Result;
}

Vector3 Vector3::Proj(Vector3 _vector1, Vector3 _vector2) {
	Vector3 result;

	result.x = (Dot(_vector1, _vector2) / Dot(_vector2, _vector2)) * _vector2.x;
	result.y = (Dot(_vector1, _vector2) / Dot(_vector2, _vector2)) * _vector2.y;
	result.z = (Dot(_vector1, _vector2) / Dot(_vector2, _vector2)) * _vector2.z;

	return result;
}

Vector3 Vector3::Orthogonal_Projection(Vector3 _vector1, Vector3 _vector2) {
	Vector3 ortho = _vector1 - _vector2;
	return ortho;
}

Vector3 operator+(Vector3 _vector1, Vector3 _vector2) {
	return Vector3(_vector1.x + _vector2.x, _vector1.y + _vector2.y, _vector1.z + _vector2.z);
}

void operator+=(Vector3& _vector1, Vector3 _vector2) {
	_vector1.x += _vector2.x;
	_vector1.y += _vector2.y;
	_vector1.z += _vector2.z;
}

Vector3 operator-(Vector3 _vector1, Vector3 _vector2) {
	return Vector3(_vector1.x - _vector2.x, _vector1.y - _vector2.y, _vector1.z - _vector2.z);
}

void operator-=(Vector3& _vector1, Vector3 _vector2) {
	_vector1.x -= _vector2.x;
	_vector1.y -= _vector2.y;
	_vector1.z -= _vector2.z;
}

Vector3 operator*(Vector3 _vector, float _scalar) {
	return Vector3(_vector.x * _scalar, _vector.y * _scalar, _vector.z * _scalar);
}

void operator*=(Vector3& _vector, float _scalar) {
	_vector.x *= _scalar;
	_vector.y *= _scalar;
	_vector.z *= _scalar;
}

bool operator==(Vector3 _vector1, Vector3 _vector2) {
	if (_vector1.x == _vector2.x && _vector1.y == _vector2.y && _vector1.z == _vector2.z)
		return true;
	else
		return false;
}