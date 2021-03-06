#include "utility.h"

static std::random_device dev;
static std::mt19937 rng(dev());

float getRandFloatNum(const float left, const float right)
{
	std::uniform_real_distribution<float> dis(left, right);	// ?????ұ?

	return dis(rng);
}

float getRand()
{
	std::uniform_real_distribution<float> dis(0.f, 1.0f);	// ?????ұ?

	return dis(rng);
}

int getRandIntNum(const int left, const int right)
{
	std::uniform_int_distribution<int> dis(left, right);	// ?????ұ?

	return dis(rng);
}

Vec getRandomVec()
{
	Vec d;
	do
	{
		d = Vec(getRand(), getRand(), getRand()) * 2.0f - Vec(1.f, 1.f, 1.f);
	} while (dot(d, d) > 1.0f);
	return d.normalization();
}

Color gammaCorrect(const Color& color)
{
	Color new_color;
	float gamma = 2.2f;
	new_color.x = std::pow(color.x, 1.0f / gamma);
	new_color.y = std::pow(color.y, 1.0f / gamma);
	new_color.z = std::pow(color.z, 1.0f / gamma);
	return new_color;
}