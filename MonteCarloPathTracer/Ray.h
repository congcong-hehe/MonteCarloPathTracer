#pragma once

#include "Vec.h"

struct Ray
{
	Vec origin;		// 光线起始点
	Vec direction;	// 光线的方向
	float t = 0.f;	

	Ray(Vec origin_, Vec direction_, float t_) : origin(origin_), direction(direction_), t(t_) {}
	Ray() {}
};