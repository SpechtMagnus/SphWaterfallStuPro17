#include "Vector3.h"

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

Vector3 operator*(const Vector3& vector, const double& factor) {
	return Vector3(vector.x * factor, vector.y * factor, vector.z * factor);
}

Vector3 operator*(const double& factor, const Vector3& vector) {
	return Vector3(vector.x * factor, vector.y * factor, vector.z * factor);
}

Vector3 operator*(const Vector3& a, const Vector3& b) {
	return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
}

Vector3 operator/(const Vector3& vector, const double& factor) {
	return Vector3(vector.x / factor, vector.y / factor, vector.z / factor);
}

Vector3 operator/(const Vector3& a, const Vector3 b) {
	return Vector3(a.x / b.x, a.y / b.y, a.z / b.z);
}

Vector3 operator%(const Vector3& vector, const double& factor) {
	return Vector3(fmod(vector.x, factor), fmod(vector.y, factor), fmod(vector.z, factor));
}

Vector3 operator%(const Vector3& a, const Vector3& b) {
	return Vector3(fmod(a.x, b.x), fmod(a.y, b.y), fmod(a.z, b.z));
}

Vector3 operator+(const Vector3& a, const Vector3& b) {
	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3 operator-(const Vector3& a) {
	return Vector3(-a.x, -a.y, -a.z);
}

Vector3 operator-(const Vector3& a, const Vector3& b) {
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector3& operator+=(Vector3& a, const Vector3& b) {
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

Vector3 & operator+=(Vector3 & vector, const double & factor) {
	vector.x += factor;
	vector.y += factor;
	vector.z += factor;
	return vector;
}

Vector3& operator-=(Vector3& a, const Vector3& b) {
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return a;
}

Vector3& operator*=(Vector3& vector, const double& factor) {
	vector.x *= factor;
	vector.y *= factor;
	vector.z *= factor;
	return vector;
}

std::ostream& operator<<(std::ostream& out, const Vector3& vector) {
	return out << "<" << "x:" << vector.x << " | y:" << vector.y << " | z:" << vector.z << ">";
}

bool operator==(const Vector3& a, const Vector3& b) {
	return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
}

bool operator!=(const Vector3 & a, const Vector3 & b) {
	return ((a.x != b.x) || (a.y != b.y) || (a.z != b.z));
}

bool Vector3::isInRangeOf(const Vector3& vector) const {
	double epsilon = 1e-6;
	return ((abs(x) >= 0) && ((x - vector.x) < epsilon) && (abs(y) >= 0) && ((y - vector.y) < epsilon) && (abs(z) >= 0) && ((z - vector.z) < epsilon));
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
	return Vector3(abs(v.x - v1.x), abs(v.y - v1.y), abs(v.z - v1.z));
}

double Vector3::dot(const Vector3& a) const {
	return this->x * a.x + this->y * a.y + this->z * a.z;
}


Vector3 Vector3::cross(const Vector3& a) {
	return Vector3(
		this->y * a.z - this->z * a.y,
		this->z * a.x - this->x * a.z,
		this->x * a.y - this->y * a.x
	);
}

Vector3 Vector3::absolute() {
	return Vector3(abs(this->x), abs(this->y), abs(this->z));
}
