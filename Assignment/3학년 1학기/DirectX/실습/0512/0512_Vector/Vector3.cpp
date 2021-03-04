#include "Vector3.h"

MyVector3::MyVector3() {
	x = y = z = 0;
}

MyVector3::~MyVector3() {

}

MyVector3::MyVector3(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

float MyVector3::Length() {
	return sqrtf(powf(x, 2) + powf(Viewport_Convert_Y(y), 2) + powf(z, 2));
}

float MyVector3::Length(MyVector3 _vector) {
	return sqrtf(powf(_vector.x, 2) + powf(Viewport_Convert_Y(_vector.y), 2) + powf(_vector.z, 2));
}

MyVector3 MyVector3::Normalize() {
	float len = Length();
	if (len == 0)
		return *this;
	else 
		return MyVector3(x / len, y / len, z / len);
}

MyVector3 MyVector3::Normalize(MyVector3 _vector) {
	float len = Length(_vector);
	if (len == 0)
		return _vector;
	else
		return MyVector3(_vector.x / len, _vector.y / len, _vector.z / len);
}

float MyVector3::GetAngle(MyVector3 _vector1, MyVector3 _vector2) {
	float cos_Angle = Dot(_vector1, _vector2);
	cos_Angle = Seta(cos_Angle);

	MyVector3 Cross_Vector = Cross(_vector1, _vector2);
	float Angle = (Cross_Vector.z > 0.0f) ? cos_Angle : -cos_Angle;
	return Angle;
}

float MyVector3::Seta(float cos) {
	return acos(cos) * 180 / PI;
}

float MyVector3::Dot(MyVector3 _vector1, MyVector3 _vector2) {
	if (_vector1 == _vector2) return 1;
	else if (Length(_vector1 + _vector2) == 0) return -1;

	float x = _vector1.x * _vector2.x;
	float y = _vector1.y * _vector2.y;
	float z = _vector1.z * _vector2.z;

	return x + y + z;
}

MyVector3 MyVector3::Cross(MyVector3 _vector1, MyVector3 _vector2) {
	MyVector3 Result;
	Viewport_Convert_Y(&_vector1.y);
	Viewport_Convert_Y(&_vector2.y);

	Result.x = _vector1.y * _vector2.z - _vector1.z * _vector2.y;
	Result.y = _vector1.z * _vector2.x - _vector1.x * _vector2.z;
	Result.z = _vector1.x * _vector2.y - _vector1.y * _vector2.x;
	 
	return Result;
}

MyVector3 MyVector3::Proj(MyVector3 _vector1, MyVector3 _vector2) {
	MyVector3 result;

	result.x = (Dot(_vector1, _vector2) / Dot(_vector2, _vector2)) * _vector2.x;
	result.y = (Dot(_vector1, _vector2) / Dot(_vector2, _vector2)) * _vector2.y;
	result.z = (Dot(_vector1, _vector2) / Dot(_vector2, _vector2)) * _vector2.z;

	return result;
}

MyVector3 MyVector3::Orthogonal_Projection(MyVector3 _vector1, MyVector3 _vector2) {
	MyVector3 ortho = _vector1 - _vector2;
	return ortho;
}

MyVector3 operator+(MyVector3 _vector1, MyVector3 _vector2) {
	return MyVector3(_vector1.x + _vector2.x, _vector1.y + _vector2.y, _vector1.z + _vector2.z);
}

void operator+=(MyVector3& _vector1, MyVector3 _vector2) {
	_vector1.x += _vector2.x;
	_vector1.y += _vector2.y;
	_vector1.z += _vector2.z;
}

MyVector3 operator-(MyVector3 _vector1, MyVector3 _vector2) {
	return MyVector3(_vector1.x - _vector2.x, _vector1.y - _vector2.y, _vector1.z - _vector2.z);
}

void operator-=(MyVector3& _vector1, MyVector3 _vector2) {
	_vector1.x -= _vector2.x;
	_vector1.y -= _vector2.y;
	_vector1.z -= _vector2.z;
}

MyVector3 operator*(MyVector3 _vector, float _scalar) {
	return MyVector3(_vector.x * _scalar, _vector.y * _scalar, _vector.z * _scalar);
}

void operator*=(MyVector3& _vector, float _scalar) {
	_vector.x *= _scalar;
	_vector.y *= _scalar;
	_vector.z *= _scalar;
}

bool operator==(MyVector3 _vector1, MyVector3 _vector2) {
	if (_vector1.x == _vector2.x && _vector1.y == _vector2.y && _vector1.z == _vector2.z)
		return true;
	else
		return false;
}