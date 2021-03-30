#pragma once

#include "Vec3f.h"

struct Ray
{
	Vec3f origin;		// 光线起始点
	Vec3f direction;	// 光线的方向
	float t = 0.f;	

	Ray(Vec3f origin_, Vec3f direction_, float t_) : origin(origin_), direction(direction_), t(t_) {}
	Ray() {}
};