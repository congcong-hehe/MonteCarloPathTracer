#pragma once

#include "TriMesh.h"
#include "Scene.h"
#include "Camera.h"

class Render
{
public:
	int width_{ 800 };
	int height_{ 600 };

	Vec3f origin_;
	Vec3f view_x_, view_y_, view_z_;	// 将屏幕坐标转换到世界坐标
	int count_ = 0;	// 记录渲染的进度

	Render(int w, int h, Color back_color, Camera camera, int spp_sqrt);

	void render(Scene& scene) const;
	Ray getRay(float x, float y) const;
	void writeImage(const std::string file_path);

private:
	int spp_;
	Image framebuffer_;
};

