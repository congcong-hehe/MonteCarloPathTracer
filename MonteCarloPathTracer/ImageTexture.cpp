#include "ImageTexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"
#include <iostream>

ImageTexture::ImageTexture(std::string file_name)
{
	int bytes_of_pixel = bytes_of_pixel_;
	
	data_ = stbi_load(file_name.c_str(), &width_, &height_, &bytes_of_pixel, bytes_of_pixel);

	if (!data_)
	{
		std::cerr << "ERROR: Could not load texture image file " << file_name << ".\n";
	}

}

Color ImageTexture::getColor(float u, float v)
{
	if (u > 1.0f) u = 1.0f; if (u < 0.0f) u = 0.0f;
	if (v > 1.0f) v = 1.0f; if (v < 0.0f) v = 0.0f;
	v = 1 - v;

	int i = (int)(u * width_);
	int j = (int)(v * height_);

	if (i >= width_) i = width_ - 1;
	if (j >= height_) j = height_ - 1;

	float scale = 1 / 255.0f;
	int index = j * bytes_of_pixel_ * width_ + i * bytes_of_pixel_;

	return Color(scale * data_[index], scale * data_[index + 1], scale * data_[index + 2]);
}