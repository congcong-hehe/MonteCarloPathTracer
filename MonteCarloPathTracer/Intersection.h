#pragma once

#include "Vec.h"
#include "Material.h"

// ���ߺ�������Ƭ�Ľ�����
struct Intersection
{
	Vec position;	// �������ά����
	Vec normal;	// ����ķ���
	UV uv;
	Material* material = nullptr;	// �������ڵ�����
};