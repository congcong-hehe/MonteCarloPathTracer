#pragma once

#include "Vec.h"
#include "Tri.h"
#include <algorithm>
#include "Ray.h"

class AABB
{
public:
	Vec v_min_;
	Vec v_max_;
	Tri *tri_ = nullptr;

	AABB() {}
	AABB(Tri &tri);
	AABB add(AABB& aabb) const;	// ����aabb���ںϵõ�һ���µ�aabb
	bool hit(Ray& ray) const;	// �жϹ����Ƿ��aabb�ཻ
};