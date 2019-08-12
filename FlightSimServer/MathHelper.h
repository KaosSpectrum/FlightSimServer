// /*
//  * (C)20192019 KaosSpectrum
//  * 
//  * Released under GPL v3 Licence.
//  *
//  * Do not remove this copyright!

#pragma once

#include <cmath>

#define M_E        2.71828182845904523536   // e
#define M_LOG2_E    1.44269504088896340736   // log2(e)
#define M_LOG10_E   0.434294481903251827651  // log10(e)
#define M_LN2      0.693147180559945309417  // ln(2)
#define M_LN10     2.30258509299404568402   // ln(10)
#define M_PI       3.14159265358979323846   // pi
#define M_PI_2     1.57079632679489661923   // pi/2
#define M_PI_4     0.785398163397448309616  // pi/4
#define M_1_PI     0.318309886183790671538  // 1/pi
#define M_2_PI     0.636619772367581343076  // 2/pi
#define M_2_SQRTPI 1.12837916709551257390   // 2/sqrt(pi)
#define M_SQRT2    1.41421356237309504880   // sqrt(2)
#define M_SQRT1_2  0.707106781186547524401  // 1/sqrt(2)

struct SMathHelper
{
public:
	static double Distance(double Lat1, const double Lon1, double Lat2, const double Lon2)
	{
		double Dlon = Lon2 - Lon1;
		Lat1 *= M_PI / 180.0, Lat2 *= M_PI / 180.0, Dlon *= M_PI / 180.0;
		double Dist = (sin(Lat1) * sin(Lat2)) + (cos(Lat1) * cos(Lat2) * cos(Dlon));
		if (Dist > 1.0) Dist = 1.0;
		Dist = acos(Dist) * 60 * 180 / M_PI;
		return Dist;
	}
};
