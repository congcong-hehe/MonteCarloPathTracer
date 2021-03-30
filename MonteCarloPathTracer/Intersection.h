#pragma once

#include "Vec3f.h"
#include "Material.h"

// 光线和三角面片的交点类
struct Intersection
{
	Vec3f position;	// 交点的三维坐标
	Vec3f normal;	// 交点的法线
	UV uv;
	std::shared_ptr<Material> material = nullptr;	// 交点所在的网格
};