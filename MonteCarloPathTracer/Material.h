#pragma once

#include "Vec.h"
#include "Image.h"
#include <algorithm>

/*
Kd: r g b 散射光 diffuse color
Ks: r g b 镜面光 specular color
Ns: 1 - 10 材质的光亮度 phong lobe 的指数
Le: r g b 光亮度
*/

class Material
{
public:
	Vec3f Kd;
	Vec3f Ks;
	int Ns = 0;
	Vec3f Le;
	Image* image_texture = nullptr;

	~Material()
	{
		if (nullptr != image_texture)
		{
			delete image_texture;
			image_texture = nullptr;
		}
	}

	bool isLight() const
	{
		return Le.x > epsilon && Le.y > epsilon && Le.z > epsilon;
	}

	bool isSpecular() const
	{
		return Ks.x > epsilon && Ks.y > epsilon && Ks.z > epsilon;
	}

	Vec3f brdf(Vec3f& wi, Vec3f& wo, Material* material, Vec3f& norm)
	{
		if (dot(wi, norm) > 0)
		{
			/*return Kd * std::max(0.0f, dot(wi, norm)) + 
				Ks * std::pow(std::max(dot(((wi + wo) / 2).normalization(), norm), 0.0f), Ns);*/
			return Kd / PI;
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

