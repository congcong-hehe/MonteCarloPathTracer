#pragma once

#include "Vec3f.h"
#include "Image.h"

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

	bool isLight();
	bool isSpecular();
	Vec3f brdf(Vec3f& wi, Vec3f& wo, std::shared_ptr<Material> material, Vec3f &norm);
	Color getTextureColor(float u, float v);
};

