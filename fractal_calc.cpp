
#include "fractal.hpp"

Vector3 calculateFractalColor(const std::complex<double>& c, int maxIterations)
{
	std::complex<double> z = 0;
	int iterations = 0;

	while (std::abs(z) < 2 && iterations < maxIterations)
	{
		z = z * z + c;
		++iterations;
	}

	if (iterations == maxIterations)
	{
		return Vector3(0, 0, 0);
	}

	double t = static_cast<double>(iterations) / maxIterations;
	return Vector3(t, t, t);
}