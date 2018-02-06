#pragma once
#include <iostream>

class Vector3 {
	public:
		// x is right, y is up, z is back
		double x, y, z;
		Vector3();
		Vector3(const double, const double, const double);
		~Vector3();

		friend Vector3 operator*(const Vector3&, const double);
		friend Vector3 operator*(const double, const Vector3&);
		friend Vector3 operator*(const Vector3&, const Vector3&);

		friend Vector3 operator/(const Vector3&, const double);
		friend Vector3 operator/(const Vector3&, const Vector3);

		friend Vector3 operator%(const Vector3&, const double);
		friend Vector3 operator%(const Vector3&, const Vector3&);

		friend Vector3 operator+(const Vector3&, const Vector3&);

		friend Vector3 operator-(const Vector3&);
		friend Vector3 operator-(const Vector3&, const Vector3&);

		friend Vector3& operator+=(Vector3&, const Vector3&);

		friend std::ostream& operator<<(std::ostream &out, const Vector3&);

		bool in_range_of(const Vector3&) const;
		double length() const;
		Vector3 normalize() const;
		Vector3 roundDownward() const;
		Vector3 distanceTo(const Vector3&, const Vector3&);

	private:
};
