#pragma once
#include "TriMesh.h"
#include "BVH.h"
#include "Color.h"
#include "Ray.h"
#include "Intersection.h"
#include "SkyBox.h"

class Scene
{
public:
	std::vector<TriMesh*> triMeshs_;	// �����е�����������
	BVH* bvh_ = nullptr;
	SkyBox* skybox_ = nullptr;
	int max_depth_ = 3;		// ·��׷�ٵ�������
	float p_RR = 0.6f;	// possibility of Russian Roulette	����˹����ת

	~Scene();
	void add(TriMesh* triMesh);
	void addSkyBox(SkyBox* skybox);
	void buildBVH();
	Color castRay(Ray& ray);		// Ͷ��һ�����ߣ����ع��ߵ�radience
	Color trace(Intersection& p, Vec3f wo, int depth);	// ׷�ٹ���
	Vec3f MonteCarloSample(Intersection& p, Vec3f &wo);	// ������ƽ��İ����������һ�����䷽��
	bool isLightBlock(Intersection &p, Intersection &x, Vec3f &wi);	// �жϹ����Ƿ��ڵ�
};

