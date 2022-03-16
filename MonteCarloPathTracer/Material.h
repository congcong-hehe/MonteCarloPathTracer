#pragma once

#include "Vec.h"
#include "Image.h"
#include <algorithm>

/*
Kd: r g b ɢ��� diffuse color
Ks: r g b ����� specular color
Ns: 1 - 10 ���ʵĹ����� phong lobe ��ָ��
Le: r g b ������
*/

class Material
{
public:
	Vec3f Kd;
	Vec3f Ks;
	int Ns = 0;
	Vec3f Le;
	std::shared_ptr<Image> image_texture = nullptr;

	bool isLight()
	{
		return Le.x > epsilon && Le.y > epsilon && Le.z > epsilon;
	}

	bool isSpecular()
	{
		return Ks.x > epsilon && Ks.y > epsilon && Ks.z > epsilon;
	}

	Vec3f brdf(Vec3f& wi, Vec3f& wo, std::shared_ptr<Material> material, Vec3f& norm)
	{
		if (dot(wi, norm) > 0)
		{
			/*return Kd * std::max(0.0f, dot(wi, norm)) + 
				Ks * std::pow(std::max(dot(((wi + wo) / 2).normalization(), norm), 0.0f), Ns);*/
			return Kd;
		}
		else
		{
			return Vec3f(0, 0, 0);
		}
	}

	Color getTextureColor(float u, float v)
	{
		return image_texture->getColor(u, v);
	}
};

