#include "vec3.h"

#include "constants.h"

#include <cmath>

vec3::vec3()
	: e{0, 0, 0}
{
}

vec3::vec3(double e0, double e1, double e2)
	: e{e0, e1, e2}
{
}

double vec3::x() const
{
	return e[0];
}

double vec3::y() const
{
	return e[1];
}

double vec3::z() const
{
	return e[2];
}

vec3 vec3::operator-() const
{
	return vec3(-e[0], -e[1], -e[2]);
}

double vec3::operator[](int ii) const
{
	return e[ii];
}

double& vec3::operator[](int ii)
{
	return e[ii];
}

vec3& vec3::operator+=(const vec3& v)
{
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

vec3& vec3::operator*=(const double t)
{
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

vec3& vec3::operator/=(const double t)
{
	return *this *= 1 / t;
}

double vec3::length() const
{
	return std::sqrt(length_squared());
}

double vec3::length_squared() const
{
	return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

bool vec3::near_zero() const
{
	const double eps(1.e-8);
	return (abs(e[0]) < eps) && (abs(e[1]) < eps) && (abs(e[2]) < eps);
}

void vec3::Translate(const vec3& v)
{
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
}

void vec3::Scale(const vec3& v)
{
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
}

vec3 vec3::CrossProduct(const vec3& v) const
{
	return ::CrossProduct(*this, v);
}

double vec3::DotProduct(const vec3& v) const
{
	return ::DotProduct(*this, v);
}

void vec3::Normalize()
{
	*this /= this->length();
}

vec3 vec3::random()
{
	return vec3(random_double(), random_double(), random_double());
}

vec3 vec3::random(double min, double max)
{
	return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

vec3 vec3::random_in_unit_sphere()
{
	while (true)
	{
		vec3 p(vec3::random(-1., 1));
		if (p.length_squared() >= 1.)
		{
			continue;
		}
		return p;
	}
}

vec3 vec3::random_unit_vector()
{
	return unit_vector(random_in_unit_sphere());
}

vec3 vec3::random_in_hemisphere(const vec3& normal)
{
	vec3 in_unit_sphere = random_in_unit_sphere();
	if (::DotProduct(in_unit_sphere, normal) > 0.)
	{
		return in_unit_sphere;
	}
	else
	{
		return -in_unit_sphere;
	}
}

vec3 vec3::random_in_unit_disk()
{
	while (true)
	{
		auto p = vec3(random_double(-1., 1.),
			random_double(-1., 1.), 0.);
		if (p.length_squared() >= 1.)
			continue;
		return p;
	}
}

vec3 vec3::reflect(const vec3& v, const vec3& normal)
{
	return v - 2. * ::DotProduct(v, normal) * normal;
}

vec3 vec3::refract(const vec3& v, const vec3& normal, double etai_over_etat)
{
	double cos_theta = fmin(::DotProduct(-v, normal), 1.);
	vec3 r_out_perp = etai_over_etat * (v + cos_theta * normal);
	vec3 r_out_parall = -sqrt(fabs(1. - r_out_perp.length_squared())) * normal;
	return r_out_perp + r_out_parall;
}
