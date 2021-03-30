#pragma once

#include "Vec3f.h"
#include "Material.h"

// ���ߺ�������Ƭ�Ľ�����
struct Intersection
{
	Vec3f position;	// �������ά����
	Vec3f normal;	// ����ķ���
	UV uv;
	std::shared_ptr<Material> material = nullptr;	// �������ڵ�����
};