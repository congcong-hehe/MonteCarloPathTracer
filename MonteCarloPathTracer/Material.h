#pragma once

#include "Vec.h"
#include "Image.h"
#include "Intersection.h"
#include <algorithm>
#include "utility.h"

/*
Kd: r g b 散射光 diffuse color
Ks: r g b 镜面光 specular color
Ns: 1 - 10 材质的光亮度 phong lobe 的指数
Le: r g b 光亮度
*/

class Material
{
public:
	Vec Kd;
	Vec Ks;
	int Ns = 0;
	Vec Le;
	Image* image_texture = nullptr;

	// 如果使用vector来存储，那么vector扩容的时候就会拷贝构造并析构原来的函数,不能析构
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
			if (getRand() < 0.5)	// 反射漫反射光线
			{
				abedo = Kd / PI;
			}
			else // 发射高光光线
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

