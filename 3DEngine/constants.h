#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>

// Constants
const double infinity(std::numeric_limits<double>::infinity());
const double pi(3.1415926535897932385);

// Utility functions
inline double degrees_to_radians(double degrees)
{
	return degrees * pi / 180.;
}

inline double random_double()
{
	// Returns a random real in [0., 1.[
	return static_cast<double>(rand()) / (RAND_MAX + 1.);
}

inline double random_double(double min, double max)
{
	// Returns a random real in [min, max[
	return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

inline int random_int(int min, int max)
{
	// Returns a random integer in [min, max]
	return static_cast<int>(random_double(min, max + 1));
}