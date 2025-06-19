#pragma once

#include <ostream>

//#include "constants.h"

class vec3
{
public:
	vec3();
	vec3(double e0, double e1, double e2);

	double x() const;
	double y() const;
	double z() const;

	vec3 operator-() const;
	double operator[](int ii) const;
	double& operator[](int ii);

	vec3& operator+=(const vec3& v);
	vec3& operator*=(const double t);
	vec3& operator/=(const double t);

	double length() const;
	double length_squared() const;

	bool near_zero() const;

	void Translate(const vec3& v);
	void Scale(const vec3& v);
	vec3 CrossProduct(const vec3& v) const;
	double DotProduct(const vec3& v) const;
	void Normalize();

	static vec3 random();
	static vec3 random(double min, double max);

	static vec3 random_in_unit_sphere();
	static vec3 random_unit_vector();
	static vec3 random_in_hemisphere(const vec3& normal);
	static vec3 random_in_unit_disk();

	static vec3 reflect(const vec3& v, const vec3& normal);
	static vec3 refract(const vec3& v, const vec3& normal, double etai_over_etat);

private:
	double e[3];
};

// Type aliases for vec3
using point3 = vec3; // 3D point
using color = vec3; // RGB color

// vec3 Utility Functions
inline std::ostream& operator<<(std::ostream& out, const vec3& v)
{
	return out << v.x() << " " << v.y() << " " << v.z();
}

inline vec3 operator+(const vec3& u, const vec3& v)
{
	return vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline vec3 operator-(const vec3& u, const vec3& v)
{
	return vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline vec3 operator*(const vec3& u, const vec3& v)
{
	return vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline vec3 operator*(double t, const vec3& v)
{
	return vec3(t * v.x(), t * v.y(), t * v.z());
}

inline vec3 operator*(const vec3& v, double t)
{
	return t * v;
}

inline vec3 operator/(const vec3& v, double t)
{
	return (1. / t) * v;
}

inline double DotProduct(const vec3& u, const vec3& v)
{
	return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

inline vec3 CrossProduct(const vec3& u, const vec3& v)
{
	return vec3(u.y() * v.z() - u.z() * v.y(),
		u.z() * v.x() - u.x() * v.z(), 
		u.x() * v.y() - u.y() * v.x());
}

inline vec3 unit_vector(const vec3& v)
{
	return v / v.length();
}