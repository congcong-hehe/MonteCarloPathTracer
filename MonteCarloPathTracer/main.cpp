#include <iostream>
#include <time.h>
#include "TriMesh.h"
#include "Render.h"
#include "SkyBox.h"
#include "utility.h"

void cornellbox();
void car();
void vis();

void hdr();


#if DEBUG
extern std::vector<size_t> count_tri_light_inter;
#endif
int main()
{
	cornellbox();
#if DEBUG
	for (auto num : count_tri_light_inter)
	{
		std::cout << num << std::endl;
	}
#endif
	return 0;
}

void cornellbox()
{
	// cornellbox
	clock_t begin, end;
	int width = 256, height = 256;
	Vec position(0.f, 0.f, 2.5f);
	Vec lookAt(0.f, 0.f, 0.f);
	Vec up(0.f, 1.0f, 0.f);
	float fov = 60.0f;
	std::string file_path = std::string("../image/") + "cornellbox.png";
	TriMesh* triMesh = new TriMesh;
	triMesh->LoadMtlFile("../example-scenes/cornellbox/", "cornellbox.mtl");
	triMesh->LoadObjFile("../example-scenes/cornellbox/", "cornellbox.obj");
	Scene scene;
	scene.add(triMesh);
	Camera camera(position, lookAt, up, fov);
	Render render(width, height, Color(1, 0, 0), camera, 4);
	scene.buildBVH();
	begin = clock();
	render.render(scene);
	end = clock();
	std::cout << "Render cost time : " << end - begin << "ms" << std::endl;
	render.writeImage(file_path);
	delete triMesh;
}

void vis()
{
	// vis
	clock_t begin, end;
	int width = 256, height = 256;
	Vec position(2.f, 11.464f, 23.8682f);
	Vec lookAt(2.f, 9.464f, 18.8682f);
	Vec up(0.f, 1.0f, 0.f);
	float fov = 50.0f;
	std::string file_path = std::string("../image/") + "vis.png";
	TriMesh* triMesh = new TriMesh;
	triMesh->LoadMtlFile("../example-scenes/vis/", "vis.mtl");
	triMesh->LoadObjFile("../example-scenes/vis/", "vis.obj");
	Scene scene;
	scene.add(triMesh);
	Camera camera(position, lookAt, up, fov);
	Render render(width, height, Color(1, 0, 0), camera, 1);
	scene.buildBVH();
	begin = clock();
	render.render(scene);
	end = clock();
	std::cout << "Render cost time : " << end - begin << "ms" << std::endl;
	render.writeImage(file_path);
	delete triMesh;
}

void car()
{
	// car 
	clock_t begin, end;
	int width = 400, height = 240;
	Vec position(8.22f, -0.61f, -9.80f);
	Vec lookAt(7.514f, -0.702f, -9.097f);
	Vec up(-0.065f, 0.996f, 0.065f);
	float fov = 45.0f;
	std::string file_path = std::string("../image/") + "car.png";
	TriMesh* triMesh = new TriMesh;
	triMesh->LoadMtlFile("../example-scenes/car/", "car.mtl");
	triMesh->LoadObjFile("../example-scenes/car/", "car.obj");
	const char* skybox_name = "../example-scenes/car/environment_day.hdr";
	SkyBox skyBox(skybox_name);
	Scene scene;
	scene.add(triMesh);
	scene.addSkyBox(&skyBox);

	Camera camera(position, lookAt, up, fov);
	Render render(width, height, Color(1, 0, 0), camera, 1);

	scene.buildBVH();

	begin = clock();
	render.render(scene);
	end = clock();

	std::cout << "Render cost time : " << end - begin << "ms" << std::endl;
	render.writeImage(file_path);

	delete triMesh;
}


void hdr()
{
	const char* skybox_name1 = "../example-scenes/car/environment_day.hdr";
	SkyBox skyBox(skybox_name1);

	Image hdr;
	hdr.setWidth(skyBox.width_);
	hdr.setHeight(skyBox.height_);
	hdr.init();

	for(int i = 0; i < hdr.getHeight(); ++i)
		for (int j = 0; j < hdr.getWidth(); ++j)
		{
			hdr.setColor(i, j, gammaCorrect(skyBox.getColor(i, j) / (skyBox.getColor(i, j) + Color(1.0f, 1.0f, 1.0f))));
		}
	hdr.write("../image/hdr.png");	
	

	Image sample;
	sample.setWidth(skyBox.width_);
	sample.setHeight(skyBox.height_);
	sample.init();

	for (int i = 0; i < sample.getHeight(); ++i)
		for (int j = 0; j < sample.getWidth(); ++j)
		{
			int x, y;
			Vec dir;
			float pdf;
			skyBox.hdrSample(dir, y, x, pdf);
			sample.setColor(x, y, Color(1.0f, 1.0f, 1.0f));
		}
	sample.write("../image/sample.png");

}