#include "AABB.h"
#include "utility.h"

AABB::AABB(std::shared_ptr<Tri> tri)
{
	Vec3f& pos0 = tri->v0.pos;
	Vec3f& pos1 = tri->v1.pos;
	Vec3f& pos2 = tri->v2.pos;

	v_min_.x = std::min(pos0.x, std::min(pos1.x, pos2.x));
	v_min_.y = std::min(pos0.y, std::min(pos1.y, pos2.y));
	v_min_.z = std::min(pos0.z, std::min(pos1.z, pos2.z));

	v_max_.x = std::max(pos0.x, std::max(pos1.x, pos2.x));
	v_max_.y = std::max(pos0.y, std::max(pos1.y, pos2.y));
	v_max_.z = std::max(pos0.z, std::max(pos1.z, pos2.z));

	this->tri_ = tri;
}

AABB AABB::add(AABB& aabb)
{
	AABB new_aabb;

	new_aabb.v_min_.x = std::min(v_min_.x, aabb.v_min_.x);
	new_aabb.v_min_.y = std::min(v_min_.y, aabb.v_min_.y);
	new_aabb.v_min_.z = std::min(v_min_.z, aabb.v_min_.z);

	new_aabb.v_max_.x = std::max(v_max_.x, aabb.v_max_.x);
	new_aabb.v_max_.y = std::max(v_max_.y, aabb.v_max_.y);
	new_aabb.v_max_.z = std::max(v_max_.z, aabb.v_max_.z);

	return new_aabb;
}

bool AABB::hit(Ray& ray)
{
	float t_enter = FLT_MAX, t_exit = -FLT_MAX;
	float t_min, t_max;

	t_min = std::min((v_min_.x - ray.origin.x) / ray.direction.x, (v_max_.x - ray.origin.x) / ray.direction.x);
	t_max = std::max((v_min_.x - ray.origin.x) / ray.direction.x, (v_max_.x - ray.origin.x) / ray.direction.x);
	t_enter = std::min(t_min, t_enter);
	t_exit = std::max(t_max, t_exit);

	t_min = std::min((v_min_.y - ray.origin.y) / ray.direction.y, (v_max_.y - ray.origin.y) / ray.direction.y);
	t_max = std::max((v_min_.y - ray.origin.y) / ray.direction.y, (v_max_.y - ray.origin.y) / ray.direction.y);
	t_enter = std::min(t_min, t_enter);
	t_exit = std::max(t_max, t_exit);

	t_min = std::min((v_min_.z - ray.origin.z) / ray.direction.z, (v_max_.z - ray.origin.z) / ray.direction.z);
	t_max = std::max((v_min_.z - ray.origin.z) / ray.direction.z, (v_max_.z - ray.origin.z) / ray.direction.z);
	t_enter = std::min(t_min, t_enter);
	t_exit = std::max(t_max, t_exit);

	return t_enter <= t_exit && t_exit >= 0;
}