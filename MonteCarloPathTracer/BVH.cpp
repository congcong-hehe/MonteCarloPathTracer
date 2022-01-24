#include "BVH.h"
#include <algorithm>
#include "utility.h"

inline bool aabbXCompare(const AABB& aabb1, const AABB& aabb2)
{
	return aabb1.v_min_.x < aabb2.v_min_.x;
}

inline bool aabbYCompare(const AABB& aabb1, const AABB& aabb2)
{
	return aabb1.v_min_.y < aabb2.v_min_.y;
}

inline bool aabbZCompare(const AABB& aabb1, const AABB& aabb2)
{
	return aabb1.v_min_.z < aabb2.v_min_.z;
}

Bvh_Node::Bvh_Node(std::vector<AABB>& aabbs, int start, int end) // 左闭右开
{
	int span = end - start;
	
	if (span == 1)
	{
		aabb_ = aabbs[start];
	}
	else 
	{
		int  rand_int = getRandIntNum(0, 2);
		if (rand_int == 0)
		{
			std::sort(aabbs.begin() + start, aabbs.begin() + end, aabbXCompare);
		}
		else if(rand_int == 1)
		{
			std::sort(aabbs.begin() + start, aabbs.begin() + end, aabbYCompare);
		}
		else
		{
			std::sort(aabbs.begin() + start, aabbs.begin() + end, aabbZCompare);
		}

		int mid = start + span / 2;
		left_ = std::make_shared<Bvh_Node>(aabbs, start, mid);
		right_ = std::make_shared<Bvh_Node>(aabbs, mid, end);

		aabb_ = left_->aabb_.add(right_->aabb_);
	}
}

void BVH::build(std::vector<TriMesh*>& triMeshs)
{
	// 为每一个三角形构建包围盒
	for (int i = 0; i < triMeshs.size(); ++i)
	{
		auto tris = triMeshs[i]->tris_;
		for (int j = 0; j < tris.size(); ++j)
		{
			aabbs_.push_back(AABB(tris[j]));
		}
	}

	// 递归地构建BVH
	root_ = std::make_shared<Bvh_Node>(aabbs_, 0, aabbs_.size());
}

bool BVH::intersection(Ray& ray, Intersection &inter)
{
	float t;
	return getIntersectPoint(ray, t, inter, root_);
}

// 求光线和场景的交点
bool BVH::getIntersectPoint(Ray& ray, float &t, Intersection& intersection, std::shared_ptr<Bvh_Node> bvh_node)
{
	if (!bvh_node->aabb_.hit(ray))	// 如果光线没有打到包围盒
	{
		return false;
	}

	if (bvh_node->left_ == nullptr && bvh_node->right_ == nullptr)	// 如果是叶子节点
	{
		bool tag = rayTriIntersect(ray, *(bvh_node->aabb_.tri_), intersection);
		if (tag)
		{
			t = ray.t;
		}
		
		return tag;
	}

	float t1 = FLT_MAX, t2 = FLT_MAX;
	Intersection intersection1, intersection2;
	bool tag1 = getIntersectPoint(ray, t1, intersection1, bvh_node->left_);
	bool tag2 = getIntersectPoint(ray, t2, intersection2, bvh_node->right_);

	intersection = t1 < t2 ? intersection1 : intersection2;
	t = std::min(t1, t2);

	return tag1 || tag2;
}

// 求光线和三角形的交点
bool BVH::rayTriIntersect(Ray& ray, Tri& tri, Intersection& intersection)
{
	Vec3f s = ray.origin - tri.v0.pos;
	Vec3f s1 = cross(ray.direction, tri.edge2);
	Vec3f s2 = cross(s, tri.edge1);

	float a = dot(s1, tri.edge1);

	if (std::fabs(a) < epsilon)
	{
		return false;
	}

	float t = dot(s2, tri.edge2) / a;
	if (t < 0)
	{
		return false;
	}

	float b1 = dot(s1, s) / a;
	if (b1 < 0 || b1 > 1)
	{
		return false;
	}

	float b2 = dot(s2, ray.direction) / a;
	if (b2 < 0 || b1 + b2 > 1)
	{
		return false;
	}

	Vec3f normal = (tri.v0.norm * (1 - b1 - b2) + tri.v1.norm * b1 + tri.v2.norm * b2).normalization();
	if (dot(ray.direction, normal) > 0)
	{
		return false;
	}

	intersection.position = ray.origin + ray.direction * t;
	intersection.material = tri.material;
	intersection.normal = normal;
	intersection.uv.u = tri.v0.uv.u * (1 - b1 - b2) + tri.v1.uv.u * b1 + tri.v2.uv.u * b2;
	intersection.uv.v = tri.v0.uv.v * (1 - b1 - b2) + tri.v1.uv.v * b1 + tri.v2.uv.v * b2;

	ray.t = t;
	return true;
}