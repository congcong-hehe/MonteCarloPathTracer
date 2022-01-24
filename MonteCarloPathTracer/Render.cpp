#include "Render.h"
#include <fstream>

Render::Render(int w, int h, Color back_color, Camera camera)
	: width_(w), height_(h)
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

// 渲染场景
void Render::render(Scene& scene)
{
	int spp_sqrt = 1;
	int spp = spp_sqrt * spp_sqrt;	// samples per pixel
	float offset = 1.0f / spp_sqrt;
 
	//#pragma omp parallel for
	for (int x = 0; x < width_; ++x)
	{
		printf("%d\n", x);
		for (int y = 0; y < height_; ++y)
		{

			// 投射光线， 计算framebuffer
			Color color;
			for (int i = 0; i < spp_sqrt; ++i)
			{
				for (int j = 0; j < spp_sqrt; ++j)
				{
					Ray ray = getRay(x + offset / 2.0f + i * offset, y + offset / 2.0f + j * offset);	// 生成一条光线
					color += scene.castRay(ray) / (float)spp;
				}
			}
			framebuffer_.setColor(height_ - y - 1, x, color);
		}
	}
}
