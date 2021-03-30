#include "Render.h"
#include <fstream>

Render::Render(int w, int h, Color back_color, Camera camera)
	: width_(w), height_(h)
{
	fb_.resize(w * h);
	origin_ = camera.position;
	Vec3f direction = (camera.lookAt - camera.position).normalization();
	Vec3f right = direction.cross(camera.up).normalization();
	Vec3f up = right.cross(direction).normalization();

	float aspect = (float)width_ / (float)height_;
	view_y_ = up * 2.0f * std::tan(camera.fov * PI / 360.f);
	view_x_ = right * 2.0f * std::tan(camera.fov * aspect * PI / 360.f);
	view_z_ = direction;
}

void Render::writeImage(std::string file_path)
{
	std::ofstream out(file_path);

	out << "P3\n";
	out << width_ << " " << height_ << "\n";
	out << 255 << "\n";

	for (int i = 0; i < fb_.size(); ++i)
	{
		Color& color = fb_[i];
		out << (int)(color.x * 255.99f) << " " << (int)(color.y * 255.99f) << " " << (int)(color.z * 255.99f) << "\n";
	}

	out.close();
}

void Render::drawPixel(int x, int y, Color &color)
{
	if (x < 0 || x >= width_ || y < 0 || y >= height_) return;
	fb_[y * width_ + x] = color;

	count_++;
	if (count_ % 100 == 0)
	{
		printf("%.4f\n", (float)count_ / (width_ * height_));
	}
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
	int spp = spp_sqrt * spp_sqrt;	// sample per pixel
	float offset = 1.0f / spp_sqrt;
 
	#pragma omp parallel for
	for (int x = 0; x < width_; ++x)
	{
		for (int y = 0; y < height_; ++y)
		{

			// 投射光线， 计算framebuffer
			// std::cout << "\n" << x << " " << y << std::endl;
			Color color;
			for (int i = 0; i < spp_sqrt; ++i)
			{
				for (int j = 0; j < spp_sqrt; ++j)
				{
					Ray ray = getRay(x + offset / 2.0f + i * offset, y + offset / 2.0f + j * offset);	// 生成一条光线
					color += scene.castRay(ray) / spp;
				}
			}
			//color.print();
			drawPixel(x, height_ - y, color);
		}
	}
}
