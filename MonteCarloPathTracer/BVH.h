#pragma once

#include "AABB.h"
#include "TriMesh.h"
#include <memory>
#include "Ray.h"

class BvhNode
{
public:
	AABB aabb_;	// 轴对齐包围盒
	BvhNode* left_ = nullptr;
	BvhNode* right_ = nullptr;

	BvhNode(std::vector<AABB>& aabbs, int left, int right);
};

class BVH
{
public:
	BvhNode* root_ = nullptr;
	std::vector<AABB> aabbs_;
	~BVH();

	void build(std::vector<TriMesh*>& triMeshs);
	bool intersection(Ray& ray, Intersection& inter);
	bool getIntersectPoint(Ray& ray, float& t, Intersection& intersection, BvhNode* bvh_node);	// 得到光线和场景相交的最近点，后面用BVH加速？
};

