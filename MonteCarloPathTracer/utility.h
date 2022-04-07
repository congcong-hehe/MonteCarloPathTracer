#pragma once

#include <random>
#include <time.h>
#include "Vec.h"

float getRandFloatNum(const float left, const float right);

int getRandIntNum(const int left, const int right);

float getRand();

Vec getRandomVec();

Color gammaCorrect(const Color& color);

inline void clamp(float &a, const float left, const float right)
{
	a = a > left ? a : left;
	a = a < right ? a : right;
}

inline void clampColor(Vec& color)
{
	clamp(color.x, 0.0f, 1.0f);
	clamp(color.y, 0.0f, 1.0f);
	clamp(color.z, 0.0f, 1.0f);
}

inline Vec reflect(const Vec& wi, const Vec& norm)
{
	return wi - norm * 2 * dot(norm, wi);
}
