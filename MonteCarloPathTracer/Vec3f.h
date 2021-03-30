#pragma once
#include <iostream>
#include "global.h"

struct Vec3f
{
	float x, y, z;

	Vec3f(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}

	Vec3f operator - ()
	{
		return Vec3f(-x, -y, -z);
	}

	Vec3f operator + (const Vec3f& v)
	{
		return Vec3f(x + v.x, y + v.y, z + v.z);
	}

	Vec3f operator += (const Vec3f& v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	Vec3f operator - (const Vec3f& v)
	{
		return Vec3f(x - v.x, y - v.y, z - v.z);
	}

	Vec3f operator / (const float a)
	{
		return Vec3f(x / a, y / a, z / a);
	}

	Vec3f operator * (const float a)
	{
		return Vec3f(x * a, y * a, z * a);
	}

	Vec3f operator * (const Vec3f& v)
	{
		return Vec3f(x * v.x, y * v.y, z * v.z);
	}

	float norm()
	{
		return (float)sqrt(x * x + y * y + z * z);
	}

	Vec3f& normalization()
	{
		*this = (*this) / norm();
		return *this;
	}

	float dot(const Vec3f& v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	Vec3f cross(const Vec3f& v)
	{
		return Vec3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	void print()
	{
		std::cout << x << " " << y << " " << z << std::endl;
	}
};
