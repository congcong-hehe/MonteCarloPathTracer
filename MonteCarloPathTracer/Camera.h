#pragma once

#include "Vec.h" 

struct Camera
{
	Vec position;
	Vec lookAt;
	Vec up;
	float fov;

	Camera(Vec position_, Vec lookAt_, Vec up_, float fov_) : position(position_), lookAt(lookAt_), up(up_), fov(fov_) {}
};
