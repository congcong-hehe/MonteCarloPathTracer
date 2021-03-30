#pragma once

#include "Color.h"
#include <string>

class ImageTexture
{
public:
	unsigned char* data_ = nullptr;
	int width_ = 0;
	int height_ = 0;
	int bytes_of_pixel_ = 3;

	ImageTexture(std::string file_name);	
	Color getColor(float u, float v);
};