#pragma once

#include "Vec.h" 

struct Camera
{
	Vec3f position;
	Vec3f lookAt;
	Vec3f up;
	float fov;

	Camera(Vec3f position_, Vec3f lookAt_, Vec3f up_, float fov_) : position(position_), lookAt(lookAt_), up(up_), fov(fov_) {}
};
