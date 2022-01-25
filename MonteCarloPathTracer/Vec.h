#pragma once
#include <iostream>
#include "global.h"

struct Vec3f
{
	union
	{
		struct { float x, y, z; };
		struct { float r, g, b; };
		struct { float Vec[3]; };
	};

	Vec3f(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}
	Vec3f operator - ()	const { return Vec3f(-x, -y, -z); }
	Vec3f operator + (const Vec3f& v) const { return Vec3f(x + v.x, y + v.y, z + v.z);  }
	Vec3f operator - (const Vec3f& v) const { return Vec3f(x - v.x, y - v.y, z - v.z); }
	Vec3f operator / (const float a) const { return Vec3f(x / a, y / a, z / a); }
	Vec3f operator * (const float a) const { return Vec3f(x * a, y * a, z * a); }
	Vec3f& operator += (const Vec3f& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vec3f& operator /= (const float a) { x /= a; y /= a; z /= a; return *this; }
	Vec3f operator * (const Vec3f& v) const { return Vec3f(x * v.x, y * v.y, z * v.z); }
	float norm() const { return (float)sqrt(x * x + y * y + z * z); }
	Vec3f& normalization() { float a = norm(); x /= a; y /= a; z /= a; return *this; }
	void print() const { printf("%f %f %f\n", x, y, z); }
};

inline float dot(const Vec3f& v1, const Vec3f& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
inline Vec3f cross(const Vec3f& v1, const Vec3f& v2) {
	return Vec3f(v1.y * v2.z - v2.y * v1.z, v1.z * v2.x - v2.z * v1.x, v1.x * v2.y - v2.x * v1.y);
}

struct Vec3i
{
	union
	{
		struct { int x, y, z; };
		struct { int Vec[3]; };
	};

	Vec3i(int _x = 0, int _y = 0, int _z = 0) : x(_x), y(_y), z(_z) {}
};

struct Vec2f
{
	union
	{
		struct { float x, y; };
		struct { float u, v; };
		struct { float Vec[2]; };
	};

	Vec2f(float _u = 0, float _v = 0) : u(_u), v(_v) {}
};

using Color = Vec3f;
using UV = Vec2f;
