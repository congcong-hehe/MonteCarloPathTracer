#include <iostream>
#include <time.h>
#include "TriMesh.h"
#include "Render.h"
#include "SkyBox.h"
#include "utility.h"

void cornellbox();
void car();
void diningroom();

#if DEBUG
extern int count_tri_light_inter;
#endif
int main()
{
	cornellbox();

#if DEBUG
	printf("%d\n", count_tri_light_inter);
#endif
	return 0;
}

void cornellbox()
{
	// cornellbox
	clock_t begin, end;
	int width = 64, height = 64;
	Vec3f position(0.f, 0.f, 2.5f);
	Vec3f lookAt(0.f, 0.f, 0.f);
	Vec3f up(0.f, 1.0f, 0.f);
	float fov = 60.0f;
	std::string file_path = std::string("../image/") + "cornellbox.png";
	TriMesh* triMesh = new TriMesh;
	triMesh->LoadMtlFile("../example-scenes/cornellbox/", "cornellbox.mtl");
	triMesh->LoadObjFile("../example-scenes/cornellbox/", "cornellbox.obj");
	Scene scene;
	scene.add(triMesh);
	Camera camera(position, lookAt, up, fov);
	Render render(width, height, Color(1, 0, 0), camera, 10);
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
	int width = 200, height = 100;
	Vec3f position(8.22f, -0.61f, -9.80f);
	Vec3f lookAt(7.514f, -0.702f, -9.097f);
	Vec3f up(-0.065f, 0.996f, 0.065f);
	float fov = 45.0f;
	std::string file_path = std::string("../image/") + "car.ppm";
	TriMesh* triMesh = new TriMesh;
	triMesh->LoadMtlFile("../example-scenes/car/", "car.mtl");
	triMesh->LoadObjFile("../example-scenes/car/", "car.obj");
	const char* skybox_name = "../example-scenes/car/environment_day.hdr";
	SkyBox skyBox(skybox_name);
	Scene scene;
	scene.add(triMesh);
	scene.addSkyBox(&skyBox);

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

void diningroom()
{
	// dining room
	clock_t begin, end;
	int width1 = 200, height1 = 100;
	Vec3f position1(0.0f, 12.720f, 31.850f);
	Vec3f lookAt1(0.0f, 12.546f, 30.865f);
	Vec3f up1(0.0f, 0.985f, -0.174f);
	float fov1 = 45.0f;
	std::string file_path1 = std::string("../image/") + "diningroom.ppm";
	TriMesh* triMesh1 = new TriMesh;
	triMesh1->LoadMtlFile("../example-scenes/diningroom/", "diningroom.mtl");
	triMesh1->LoadObjFile("../example-scenes/diningroom/", "diningroom.obj");
	const char* skybox_name1 = "../example-scenes/diningroom/environment.hdr";
	SkyBox skyBox1(skybox_name1);
	Scene scene1;
	scene1.add(triMesh1);
	scene1.addSkyBox(&skyBox1);

	Camera camera1(position1, lookAt1, up1, fov1);
	Render render1(width1, height1, Color(1, 0, 0), camera1, 4);

	scene1.buildBVH();

	begin = clock();
	render1.render(scene1);
	end = clock();

	std::cout << "Render cost time : " << end - begin << "ms" << std::endl;
	render1.writeImage(file_path1);

	delete triMesh1;
}