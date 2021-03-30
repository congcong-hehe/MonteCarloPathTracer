#pragma once

#include "Color.h"
#include "Ray.h"

class SkyBox
{
public:
	int width_ = 0;
	int height_ = 0;
	float* image_ = nullptr;

	SkyBox(const char* file_name);
	~SkyBox();
	Color sample(Ray& ray);
};

