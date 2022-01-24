#include "Material.h"
#include <math.h>

// 判断材质是可以发射光线，即是否是光源
bool Material::isLight()
{
    return Le.x  > epsilon && Le.y > epsilon && Le.z > epsilon;
}

bool Material::isSpecular()
{
    return Ks.x > epsilon && Ks.y > epsilon && Ks.z > epsilon;
}

Vec3f Material::brdf(Vec3f& wi, Vec3f& wo, std::shared_ptr<Material> material, Vec3f& norm)
{
    if (dot(wi, norm) > 0)
    {
        return Kd / PI;
    }
    else
    {
        return Vec3f(0, 0, 0);
    }
}

Color Material::getTextureColor(float u, float v)
{
    if (image_texture != nullptr)
    {
        return image_texture->getColor(u, v);
    }
    else
    {
        return Color(0.0f, 0.0f, 0.0f);
    }
}