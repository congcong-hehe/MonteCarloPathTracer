#pragma once

#include "Vec.h"
#include "Material.h"

// ���ߺ�������Ƭ�Ľ�����
struct Intersection
{
	Vec3f position;	// �������ά����
	Vec3f normal;	// ����ķ���
	UV uv;
	Material* material = nullptr;	// �������ڵ�����
};