#define _CRT_SECURE_NO_WARNINGS

#include "SkyBox.h"
#include "external/rgbe.h"
#include <stdio.h>

SkyBox::SkyBox(const char* file_name)
{
    FILE* f = fopen(file_name, "rb");
    RGBE_ReadHeader(f, &width_, &height_, NULL);
    image_ = (float*)malloc(sizeof(float) * 3 * width_ * height_);
    RGBE_ReadPixels_RLE(f, image_, width_, height_);
}

SkyBox::~SkyBox()
{
    delete image_;
}

Color SkyBox::sample(Ray& ray)
{
    float phi = std::atan2(ray.direction.x, -ray.direction.z) / PI * 0.5f;
    float theta = std::acos(ray.direction.y) / PI;

    int width_index = phi * width_;
    int height_index = theta * height_;
    int index = height_index * width_ + width_index;

    return Color(image_[3 * index + 0], image_[3 * index + 1], image_[3 * index + 2]);
}
