#include "Tri.h"

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

	area = edge1.cross(edge2).norm() * 0.5f;
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