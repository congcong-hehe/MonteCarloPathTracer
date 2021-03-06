#pragma once

#include "Vec.h"

class Material;

// 光线和三角面片的交点类
struct Intersection
{
	Vec position;	// 交点的三维坐标
	Vec normal;	// 交点的法线
	UV uv;
	Material* material = nullptr;	// 交点所在的网格
};