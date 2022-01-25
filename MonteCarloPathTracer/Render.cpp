#include "Render.h"
#include <fstream>
#include "utility.h"

Render::Render(int w, int h, Color back_color, Camera camera, int spp)
	: width_(w), height_(h), spp_(spp)
{
	origin_ = camera.position;
	Vec3f direction = (camera.lookAt - camera.position).normalization();
	Vec3f right = cross(direction, camera.up).normalization();
	Vec3f up = cross(right, direction).normalization();

	float aspect = (float)width_ / (float)height_;
	view_y_ = up * 2.0f * std::tan(camera.fov * PI / 360.f);
	view_x_ = right * 2.0f * std::tan(camera.fov * aspect * PI / 360.f);
	view_z_ = direction;

	framebuffer_.setHeight(h);
	framebuffer_.setWidth(w);
	framebuffer_.init();
}

void Render::writeImage(const std::string file_path)
{
	framebuffer_.write(file_path);
}

Ray Render::getRay(float x, float y)
{
	float x_float = x / width_;
	float y_float = y / height_;
	Vec3f direction = (view_z_ + view_x_ * (x_float - 0.5f) + view_y_ * (y_float - 0.5f)).normalization();

	return Ray(origin_, direction, 1.0f);
}

// ‰÷»æ≥°æ∞
void Render::render(Scene& scene)
{
	#pragma omp parallel for num_threads(4)
	for (int x = 0; x < width_; ++x)
	{
		printf("%d\n", x);
		for (int y = 0; y < height_; ++y)
		{
			Color color;
			for (int i = 0; i < spp_; ++i)
			{
				float x_offset = getRandFloatNum(0, 1);
				float y_offset = getRandFloatNum(0, 1);
				Ray ray = getRay(x + x_offset, y + y_offset);
				color += scene.castRay(ray);
			}
			framebuffer_.setColor(height_ - y - 1, x, color / spp_);
		}
	}
}
