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
