#include "BVH.h"
#include <algorithm>
#include "utility.h"

void deleteBvhNode(BvhNode* root)
{
	if (root == nullptr) return;
	deleteBvhNode(root->left_);
	deleteBvhNode(root->right_);
	delete root;
}

BVH::~BVH()
{
	deleteBvhNode(root_);
}

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

BvhNode::BvhNode(std::vector<AABB>& aabbs, int start, int end) // 左闭右开
{
	int span = end - start;
	
	if (span == 1)
	{
		aabb_ = aabbs[start];
	}
	else 
	{
		int rand_int = getRandIntNum(0, 2);
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
		left_ = new BvhNode(aabbs, start, mid);
		right_ = new BvhNode(aabbs, mid, end);

		aabb_ = left_->aabb_.add(right_->aabb_);
	}
}

void BVH::build(std::vector<TriMesh*>& triMeshs)
{
	// 为每一个三角形构建包围盒
	for (int i = 0; i < triMeshs.size(); ++i)
	{
		auto &tris = triMeshs[i]->tris_;
		for (int j = 0; j < tris.size(); ++j)
		{
			aabbs_.push_back(AABB(tris[j]));
		}
	}

	// 递归地构建BVH
	root_ = new BvhNode(aabbs_, 0, aabbs_.size());
}

bool BVH::intersection(Ray& ray, Intersection &inter)
{
	float t;
	return getIntersectPoint(ray, t, inter, root_);
}

// 求光线和场景的交点
bool BVH::getIntersectPoint(Ray& ray, float &t, Intersection& intersection, BvhNode* bvh_node)
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

	if (!(tag1 || tag2)) return false;

	intersection = t1 < t2 ? intersection1 : intersection2;
	t = std::min(t1, t2);

	return true;
}