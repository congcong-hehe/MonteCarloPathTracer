#pragma once

#include "AABB.h"
#include "TriMesh.h"
#include <memory>
#include "Ray.h"

class Bvh_Node
{
public:
	AABB aabb_;	// ������Χ��
	std::shared_ptr<Bvh_Node> left_ = nullptr;
	std::shared_ptr<Bvh_Node> right_ = nullptr;

	Bvh_Node(std::vector<AABB>& aabbs, int left, int right);
};

class BVH
{
public:
	std::shared_ptr<Bvh_Node> root_;
	std::vector<AABB> aabbs_;

	void build(std::vector<TriMesh*>& triMeshs);
	bool intersection(Ray& ray, Intersection& inter);
	bool getIntersectPoint(Ray& ray, float& t, Intersection& intersection, std::shared_ptr<Bvh_Node> bvh_node);	// �õ����ߺͳ����ཻ������㣬������BVH���٣�
};

