#pragma once

#include "Ray.h"
#include <vector>

class SkyBox
{
public:
	int width_ = 0;
	int height_ = 0;
	float* image_ = nullptr;

	std::vector<std::vector<int>> sample_x;
	std::vector<std::vector<int>> sample_y;
	std::vector<std::vector<float>> sample_p;

	SkyBox(const char* file_name);
	~SkyBox();
	Color sample(Ray& ray);
	Color hdrSample(Vec &dir, int &x, int &y);
	Color getColor(const int u, const int v);
};

