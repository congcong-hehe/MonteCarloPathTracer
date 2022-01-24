#pragma once

#include "Vec3f.h"
#include "Image.h"

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
	std::shared_ptr<Image> image_texture = nullptr;

	bool isLight();
	bool isSpecular();
	Vec3f brdf(Vec3f& wi, Vec3f& wo, std::shared_ptr<Material> material, Vec3f &norm);
	Color getTextureColor(float u, float v);
};

