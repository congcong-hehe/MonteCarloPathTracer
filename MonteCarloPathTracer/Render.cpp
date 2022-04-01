#include "Render.h"
#include <fstream>
#include "utility.h"

Render::Render(int w, int h, Color back_color, Camera camera, int spp)
	: width_(w), height_(h), spp_(spp)
{
	origin_ = camera.position;
	Vec direction = (camera.lookAt - camera.position).normalization();
	Vec right = cross(direction, camera.up).normalization();
	Vec up = cross(right, direction).normalization();

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

Ray Render::getRay(float x, float y) const
{
	float x_float = x / width_;
	float y_float = y / height_;
	Vec direction = (view_z_ + view_x_ * (x_float - 0.5f) + view_y_ * (y_float - 0.5f)).normalization();

	return Ray(origin_, direction, 1.0f);
}

// 渲染场景
void Render::render(Scene& scene) const
{
	std::vector<float> x_rand(spp_);
	std::vector<float> y_rand(spp_);

	for (int i = 0; i < spp_; ++i)
	{
		x_rand[i] = getRand();
		y_rand[i] = getRand();
	}

#if !DEBUG
	#pragma omp parallel for num_threads(4)
#endif
	for (int x = 0; x < width_; ++x)
	{
		printf("%d\n", x);
		for (int y = 0; y < height_; ++y)
		{
			Color color;
			for (int i = 0; i < spp_; ++i)
			{
				Ray ray = getRay(x + x_rand[i], y + y_rand[i]);
				//color += scene.castRay(ray);
				color += scene.castRayBVH(ray);		// BVH对于connelbox大概25%的提升
			}
			framebuffer_.setColor(height_ - y - 1, x, color / spp_);
		}
	}
}
