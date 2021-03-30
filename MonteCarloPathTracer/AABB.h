#pragma once

#include "Vec3f.h"
#include "Tri.h"
#include <algorithm>
#include "Ray.h"

class AABB
{
public:
	Vec3f v_min_;
	Vec3f v_max_;
	std::shared_ptr<Tri> tri_;

	AABB() {}
	AABB(std::shared_ptr<Tri> tri);
	AABB add(AABB& aabb);	// 两个aabb盒融合得到一个新的aabb
	bool hit(Ray& ray);	// 判断光线是否和aabb相交
};