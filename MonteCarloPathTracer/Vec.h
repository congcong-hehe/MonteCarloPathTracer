#pragma once
#include <iostream>
#include "global.h"
#include <emmintrin.h>
#include <smmintrin.h>

#define vFFFFMask (_mm_set_epi32(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF))
#define vFFFFfMask _mm_castsi128_ps(vFFFFMask)
#define vFFF0Mask (_mm_set_epi32(0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF))
#define vFFF0fMask _mm_castsi128_ps(vFFF0Mask)
#define vFF0FMask (_mm_set_epi32(0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF))
#define vFF0FfMask _mm_castsi128_ps(vFF0FMask)
#define vF0FFMask (_mm_set_epi32(0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF))
#define vF0FFfMask _mm_castsi128_ps(vF0FFMask)
#define v0FFFMask (_mm_set_epi32(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000))
#define v0FFFfMask _mm_castsi128_ps(v0FFFMask)

#define MakeShuffleMask(x,y,z,w)           (x | (y<<2) | (z<<4) | (w<<6))
#define VecSwizzleMask(vec, mask)          _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(vec), mask))
#define VecSwizzle(vec, x, y, z, w)        VecSwizzleMask(vec, MakeShuffleMask(x,y,z,w))

struct Vec
{
	union
	{
		struct { float x, y, z; };
		struct { float r, g, b; };
		struct { float vec[3]; };
#if USE_SIMD
		__m128 sse_val;
#endif
	};
	Vec()
	{
#if USE_SIMD
		sse_val = _mm_setzero_ps();
#else
		x = y = z =  0.0;
#endif
	}

#if USE_SIMD
	Vec(const __m128& _val)
	{
		sse_val = _val;
	}
#endif

	Vec(float _x, float _y, float _z) 
	{
#if USE_SIMD
		sse_val = _mm_set_ps(0.0, _z, _y, _x);
#else
		x = _x;	y = _y; z = _z;
#endif
	}

	Vec operator - ()	const 
	{ 
#if USE_SIMD
		__m128 r = _mm_xor_ps(sse_val, _mm_set_ps(-0.0f, -0.0f, -0.0f, -0.0f));
		return Vec(_mm_and_ps(r, vFFF0fMask));
#else
		return Vec(-x, -y, -z); 
#endif
	}

	Vec operator + (const Vec& v) const
	{
#if USE_SIMD
		return Vec(_mm_add_ps(sse_val, v.sse_val));
#else
		return Vec(x + v.x, y + v.y, z + v.z);  
#endif
	}

	Vec operator - (const Vec& v) const 
	{ 
#if USE_SIMD
		return Vec(_mm_sub_ps(sse_val, v.sse_val));
#else
		return Vec(x - v.x, y - v.y, z - v.z); 
#endif
	}

	Vec operator / (const float a) const
	{
#if USE_SIMD
		__m128 m = _mm_set1_ps(a);
		return Vec(_mm_div_ps(sse_val, m));
#else
		return Vec(x / a, y / a, z / a); 
#endif
	}

	Vec operator * (const float a) const 
	{
#if USE_SIMD
		__m128 m = _mm_set1_ps(a);
		return Vec(_mm_mul_ps(m, sse_val));
#else
		return Vec(x * a, y * a, z * a); 
#endif
	}

	Vec& operator += (const Vec& v) 
	{
#if USE_SIMD
		sse_val = _mm_add_ps(sse_val, v.sse_val);
		return *this;
#else
		x += v.x; y += v.y; z += v.z; return *this; 
#endif
	}

	Vec& operator /= (const float a) 
	{
#if USE_SIMD
		__m128 m = _mm_set1_ps(a);
		sse_val = _mm_div_ps(sse_val , m);
		return *this;
#else
		x /= a;	y /= a; z /= a;
		return *this;
#endif
	}

	Vec operator * (const Vec& v) const 
	{
#if USE_SIMD
		return Vec(_mm_mul_ps(v.sse_val, sse_val));
#else
		return Vec(x * v.x, y * v.y, z * v.z); 
#endif
	}

	float norm() const { return (float)sqrt(x * x + y * y + z * z); }

	Vec& normalization() {  
		if (x * x + y * y + z * z == 0.f) return *this;
		float a = norm(); x /= a; y /= a; z /= a;  return *this; 
	}

	void print() const { printf("%f %f %f\n", x, y, z); }
};

inline float dot(const Vec& v1, const Vec& v2) { 
#if USE_SIMD
	 return _mm_dp_ps(v1.sse_val, v2.sse_val, 0xFF).m128_f32[0];
#else
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; 
#endif
}

inline Vec cross(const Vec& v1, const Vec& v2) {
#if USE_SIMD
	return Vec(VecSwizzle(_mm_sub_ps(_mm_mul_ps(VecSwizzle(v1.sse_val, 2, 0, 1, 3), v2.sse_val),
		_mm_mul_ps(VecSwizzle(v2.sse_val, 2, 0, 1, 3), v1.sse_val)),
		2, 0, 1, 3));
#else
	return Vec(v1.y * v2.z - v2.y * v1.z, v1.z * v2.x - v2.z * v1.x, v1.x * v2.y - v2.x * v1.y);
#endif
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

using Color = Vec;
using UV = Vec2f;
