#pragma once

#include <random>
#include <time.h>

inline float getRandFloatNum(float left, float right)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<float> dis(left, right);	// ×ó±ÕÓÒ±Õ

	return dis(rng);
}

inline int getRandIntNum(int left, int right)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<int> dis(left, right);	// ×ó±ÕÓÒ±Õ

	return dis(rng);
}

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
