#pragma once

#include "Color.h"
#include "TriMesh.h"
#include "Scene.h"
#include "Camera.h"

class Render
{
public:
	int width_{ 800 };
	int height_{ 600 };
	std::vector<Color> fb_;
	Vec3f origin_;
	Vec3f view_x_, view_y_, view_z_;	// ����Ļ����ת������������
	int count_ = 0;	// ��¼��Ⱦ�Ľ���

	Render(int w, int h, Color back_color, Camera camera);

	inline void drawPixel(int x, int y, Color &color);
	void render(Scene& scene);
	Ray getRay(float x, float y);
	void writeImage(std::string file_path);
};

