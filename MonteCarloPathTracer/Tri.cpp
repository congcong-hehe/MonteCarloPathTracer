#include "Tri.h"
#include "utility.h"

#if DEBUG
int count_tri_light_inter = 0;	// 统计光线和三角形求交的次数
#endif

void Tri::init(std::vector<Vec3f> &vs, std::vector<Vec3f> &vns, std::vector<UV> &vts, int *v, int *vn, int *vt)
{
	v0.norm = vns[vn[0]];
	v1.norm = vns[vn[1]];
	v2.norm = vns[vn[2]];

	v0.pos = vs[v[0]];
	v1.pos = vs[v[1]];
	v2.pos = vs[v[2]];

	v0.uv = vts[vt[0]];
	v1.uv = vts[vt[1]];
	v2.uv = vts[vt[2]];

	edge1 = v1.pos - v0.pos;
	edge2 = v2.pos - v0.pos;

	area = cross(edge1,edge2).norm() * 0.5f;
}

// 采样光源，从光源上随机采一点，得到交点和采样概率，pdf = 1 / area
void Tri::sampleLight(Intersection &x, float &pdf)
{
	float rand1 = std::sqrt(getRandFloatNum(0, 1)),  rand2 = getRandFloatNum(0, 1);

	float a0 = 1 - rand1;
	float a1 = rand1 * (1 - rand2);
	float a2 = rand1 * rand2;

	x.position = v0.pos * a0 + v1.pos * a1 + v2.pos * a2;
	x.material = material;
	x.normal = (v0.norm * a0 + v1.norm * a1 + v2.norm * a2).normalization();

	pdf = 1.0f / area;
}

// 求光线和三角形的交点
bool rayTriIntersect(Ray& ray, Tri& tri, Intersection& intersection)
{
#if DEBUG
	count_tri_light_inter++;
#endif

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