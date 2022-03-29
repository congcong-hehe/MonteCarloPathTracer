#pragma once
#include "Material.h"
#include <vector>
#include "Vertex.h"
#include "Intersection.h"
#include "Ray.h"

class Tri
{
public:
	Vertex v0, v1, v2;

	Material* material = nullptr;	// 三角形的材质
	Vec3f edge1;
	Vec3f edge2;
	float area = 0.0f;

	void init(std::vector<Vec3f> &vs, std::vector<Vec3f> &vns, std::vector<UV> &vts, int *v, int *vn, int *vt);
	void sampleLight(Intersection &x, float &pdf);

};

bool rayTriIntersect(Ray& ray, Tri& tri, Intersection& intersection);

