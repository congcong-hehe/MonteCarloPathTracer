#pragma once

#include "Vec.h"
#include "Image.h"
#include "Intersection.h"
#include <algorithm>
#include "utility.h"

/*
Kd: r g b ɢ��� diffuse color
Ks: r g b ����� specular color
Ns: 1 - 10 ���ʵĹ����� phong lobe ��ָ��
Le: r g b ������
*/

class Material
{
public:
	Vec Kd;
	Vec Ks;
	int Ns = 0;
	Vec Le;
	Image* image_texture = nullptr;

	// ���ʹ��vector���洢����ôvector���ݵ�ʱ��ͻ´�����첢����ԭ���ĺ���,��������
	~Material()
	{

	}

	bool isLight() const
	{
		return Le.x > epsilon && Le.y > epsilon && Le.z > epsilon;
	}

	bool isSpecular() const
	{
		return Ks.x > epsilon && Ks.y > epsilon && Ks.z > epsilon;
	}

	Vec brdf(Vec& wi, Vec& wo, Intersection &p)
	{
		float dot_wi_n = dot(wi, p.normal);

		if (dot_wi_n < 0) return Vec();

		Vec abedo;
		if (isSpecular())
		{
			if (getRand() < 0.5)	// �������������
			{
				abedo = Kd / PI;
			}
			else // ����߹����
			{
				abedo = Ks * std::pow(std::max(dot(reflect(wi, p.normal), wo), 0.0f), Ns);
			}
		}
		else
			abedo = Kd / PI;

		if (p.material->image_texture != nullptr)
		{
			abedo *= (p.material->image_texture->getColor(p.uv.u, p.uv.v));
		}

		return abedo;
	}

	Color getTextureColor(float u, float v)
	{
		return image_texture->getColor(u, v);
	}
};

