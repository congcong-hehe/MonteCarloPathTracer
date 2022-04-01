#include "AABB.h"
#include "utility.h"

AABB::AABB(Tri &tri)
{
	Vec& pos0 = tri.v0.pos;
	Vec& pos1 = tri.v1.pos;
	Vec& pos2 = tri.v2.pos;

#if USE_SIMD
	v_min_.sse_val = _mm_min_ps(pos0.sse_val, _mm_min_ps(pos1.sse_val, pos2.sse_val));
	v_max_.sse_val = _mm_max_ps(pos0.sse_val, _mm_max_ps(pos1.sse_val, pos2.sse_val));
#else
	v_min_.x = std::min(pos0.x, std::min(pos1.x, pos2.x));
	v_min_.y = std::min(pos0.y, std::min(pos1.y, pos2.y));
	v_min_.z = std::min(pos0.z, std::min(pos1.z, pos2.z));

	v_max_.x = std::max(pos0.x, std::max(pos1.x, pos2.x));
	v_max_.y = std::max(pos0.y, std::max(pos1.y, pos2.y));
	v_max_.z = std::max(pos0.z, std::max(pos1.z, pos2.z));
#endif
	this->tri_ = &tri;
}

AABB AABB::add(AABB& aabb) const
{
	AABB new_aabb;

#if USE_SIMD
	new_aabb.v_min_.sse_val = _mm_min_ps(v_min_.sse_val, aabb.v_min_.sse_val);
	new_aabb.v_max_.sse_val = _mm_max_ps(v_max_.sse_val, aabb.v_max_.sse_val);
#else
	new_aabb.v_min_.x = std::min(v_min_.x, aabb.v_min_.x);
	new_aabb.v_min_.y = std::min(v_min_.y, aabb.v_min_.y);
	new_aabb.v_min_.z = std::min(v_min_.z, aabb.v_min_.z);

	new_aabb.v_max_.x = std::max(v_max_.x, aabb.v_max_.x);
	new_aabb.v_max_.y = std::max(v_max_.y, aabb.v_max_.y);
	new_aabb.v_max_.z = std::max(v_max_.z, aabb.v_max_.z);
#endif
	return new_aabb;
}

bool AABB::hit(Ray& ray) const
{
	float t_enter, t_exit;

#if USE_SIMD

	__m128 a = _mm_div_ps(_mm_sub_ps(v_min_.sse_val, ray.origin.sse_val), ray.direction.sse_val);
	__m128 b = _mm_div_ps(_mm_sub_ps(v_max_.sse_val, ray.origin.sse_val), ray.direction.sse_val);
	__m128 t_min = _mm_min_ps(a, b);
	__m128 t_max = _mm_max_ps(a, b);
	t_enter = std::max(std::max(t_min.m128_f32[0], t_min.m128_f32[1]), t_min.m128_f32[2]);
	t_exit = std::min(std::min(t_max.m128_f32[0], t_max.m128_f32[1]), t_max.m128_f32[2]);
#else
	Vec a = (v_min_ - ray.origin) / ray.direction;
	Vec b = (v_max_ - ray.origin) / ray.direction;
	t_enter = std::max(std::max(std::min(a.x, b.x), std::min(a.y, b.y)), std::min(a.z, b.z));
	t_exit = std::min(std::min(std::max(a.x, b.x), std::max(a.y, b.y)), std::max(a.z, b.z));
#endif

	return t_enter <= t_exit && t_exit >= 0;
}