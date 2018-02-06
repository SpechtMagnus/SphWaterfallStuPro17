#pragma once
#include "Vector3.h"
#include <math.h>

Vector3::Vector3() :
	x(0.0),
	y(0.0),
	z(0.0)
{
}

Vector3::Vector3(const double x, const double y, const double z) :
	x(x),
	y(y),
	z(z) 
{
}

Vector3::~Vector3()
{
}

Vector3 operator*(const Vector3& vector, const double factor) {
	return Vector3(vector.x * factor, vector.y * factor, vector.z * factor);
}

Vector3 operator*(const double factor, const Vector3& vector) {
	return vector * factor;
}

Vector3 operator*(const Vector3& a, const Vector3& b) {
	return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
}

Vector3 operator/(const Vector3& vector, const double factor) {
	return Vector3(vector.x / factor, vector.y / factor, vector.z / factor);
}

Vector3 operator/(const Vector3& a, const Vector3 b) {
	return Vector3(a.x / b.x, a.y / b.y, a.z / b.z);
}

Vector3 operator%(const Vector3& vector, const double factor) {
	return Vector3(fmod(vector.x, factor), fmod(vector.y, factor), fmod(vector.z, factor));
}

Vector3 operator%(const Vector3& a, const Vector3& b) {
	return Vector3(fmod(a.x, b.x), fmod(a.y, b.y), fmod(a.z, b.z));
}

Vector3 operator+(const Vector3& a, const Vector3& b)
{
	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3& operator+=(Vector3& a, const Vector3& b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

Vector3 operator-(const Vector3& a)
{
	return Vector3(-a.x, -a.y, -a.z);
}

Vector3 operator-(const Vector3& a, const Vector3& b)
{
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

std::ostream& operator<<(std::ostream & out, const Vector3& vector)
{
	return out << "x:" << vector.x << " y:" << vector.y << " z:" << vector.z;
}


bool Vector3::in_range_of(const Vector3& b) const
{
	return x >= 0 && x < b.x && y >= 0 && y < b.y && z >= 0 && z <= b.z;
}

double Vector3::length() const {
	return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::normalize() const {
	return *this / length();
}

Vector3 Vector3::roundDownward() const{
	return Vector3(floor(x), floor(y), floor(z));
}

Vector3 Vector3::distanceTo(const Vector3& v, const Vector3& v1) {
	abs(v.x - v1.x), abs(v.y - v1.y), abs(v.z - v1.z);
}
