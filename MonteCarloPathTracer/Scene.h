#pragma once
#include "TriMesh.h"
#include "BVH.h"
#include "Ray.h"
#include "Intersection.h"
#include "SkyBox.h"

class Scene
{
public:
	std::vector<TriMesh*> triMeshs_;	// �����е�����������
	BVH* bvh_ = nullptr;
	SkyBox* skybox_ = nullptr;
	int max_depth_ = 4;		// ·��׷�ٵ�������
	float p_RR = 0.8f;	// possibility of Russian Roulette	����˹����ת

	~Scene();
	void add(TriMesh* triMesh);
	void addSkyBox(SkyBox* skybox);
	void buildBVH();
	Color castRayBVH(Ray& ray);		// Ͷ��һ�����ߣ����ع��ߵ�radience
	Color castRay(Ray& ray);
	Color trace(Intersection& p, Vec wo, int depth);	// ׷�ٹ���
	Vec HemisphereSample(Intersection& p);	// ������ƽ��İ����������һ�����䷽��
	Vec HemisphereSample2(Intersection& p);

	bool isLightBlock(Intersection& p, Intersection& x, Vec& wi);	// �жϹ����Ƿ��ڵ�
	bool getIntersection(Ray& ray, Intersection& intersection);		// �������е�������Ƭ�õ��ཻ��Ƭ
};

