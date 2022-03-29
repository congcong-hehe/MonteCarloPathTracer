#pragma once

#include "Vec.h"
#include "Tri.h"
#include <algorithm>
#include "Ray.h"

class AABB
{
public:
	Vec3f v_min_;
	Vec3f v_max_;
	Tri *tri_ = nullptr;

	AABB() {}
	AABB(Tri &tri);
	AABB add(AABB& aabb) const;	// 两个aabb盒融合得到一个新的aabb
	bool hit(Ray& ray) const;	// 判断光线是否和aabb相交
};