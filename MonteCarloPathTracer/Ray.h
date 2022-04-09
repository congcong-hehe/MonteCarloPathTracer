#pragma once

#include "Vec.h"

struct Ray
{
	Vec origin;		// ������ʼ��
	Vec direction;	// ���ߵķ���
	float t = 0.f;	

	Ray(Vec origin_, Vec direction_, float t_ = 0.0f) : origin(origin_), direction(direction_), t(t_) {}
	Ray() {}
};