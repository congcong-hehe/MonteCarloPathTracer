#pragma once

#include "Vec3f.h"

struct Ray
{
	Vec3f origin;		// ������ʼ��
	Vec3f direction;	// ���ߵķ���
	float t = 0.f;	

	Ray(Vec3f origin_, Vec3f direction_, float t_) : origin(origin_), direction(direction_), t(t_) {}
	Ray() {}
};