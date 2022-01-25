#pragma once
#include "TriMesh.h"
#include "BVH.h"
#include "Ray.h"
#include "Intersection.h"
#include "SkyBox.h"

class Scene
{
public:
	std::vector<TriMesh*> triMeshs_;	// 场景中的三角形网格
	BVH* bvh_ = nullptr;
	SkyBox* skybox_ = nullptr;
	int max_depth_ = 3;		// 路径追踪的最大深度
	float p_RR = 0.6f;	// possibility of Russian Roulette	俄罗斯轮盘转

	~Scene();
	void add(TriMesh* triMesh);
	void addSkyBox(SkyBox* skybox);
	void buildBVH();
	Color castRayBVH(Ray& ray);		// 投射一根光线，返回光线的radience
	Color castRay(Ray& ray);
	Color trace(Intersection& p, Vec3f wo, int depth);	// 追踪光线
	Vec3f MonteCarloSample(Intersection& p, Vec3f &wo);	// 在所在平面的半球上随机找一条出射方向
	bool isLightBlock(Intersection &p, Intersection &x, Vec3f &wi);	// 判断光线是否被遮挡
};

