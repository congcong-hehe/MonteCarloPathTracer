#include "Scene.h"
#include <limits.h>
#include "utility.h"
#include "Intersection.h"

Scene::~Scene()
{
	if (bvh_)
	{
		delete bvh_;
		bvh_ = nullptr;
	}

	for (int i = 0; i < triMeshs_.size(); ++i)
	{
		auto &materials = triMeshs_[i]->materials_;
		for (int j = 0; j < materials.size(); ++j)
		{
			if (materials[j].image_texture != nullptr)
			{
				delete materials[j].image_texture;
				materials[j].image_texture = nullptr;
			}
		}
	}
}

void Scene::add(TriMesh* triMesh)
{
	triMeshs_.push_back(triMesh);
}

void Scene::addSkyBox(SkyBox* skybox)
{
	skybox_ = skybox;
}

void Scene::buildBVH()
{
	bvh_ = new BVH;
	bvh_->build(triMeshs_);
}

bool Scene::getIntersection(Ray& ray, Intersection& intersection)
{
	float t_min = FLT_MAX;
	for (int i = 0; i < triMeshs_.size(); ++i)
	{
		TriMesh* trimesh = triMeshs_[i];
		for (int j = 0; j < trimesh->tris_.size(); ++j)
		{
			Intersection inter_tmep;
			auto &tri = trimesh->tris_[j];
			bool tag = rayTriIntersect(ray, tri, inter_tmep);
			if (tag && ray.t < t_min)
			{
				intersection = inter_tmep;
				t_min = ray.t;
			}
		}
	}
	return t_min != FLT_MAX;
}

// ��ʹ�ð�Χ�м���
Color Scene::castRay(Ray& ray)
{
	Color pixel_radience = Color(0, 0, 0);

	Intersection intersection;

	if (getIntersection(ray, intersection))	// ������ߺ����������н���
	{
		if(!intersection.material->isLight())	// ���û�д򵽹�Դ��
		{
			pixel_radience = trace(intersection, -ray.direction, 1);
		}
		else
		{
			pixel_radience = intersection.material->Le;
		}
	}
	else
	{
		if (skybox_ != nullptr)
		{
			pixel_radience = skybox_->sample(ray);
			pixel_radience = pixel_radience / (pixel_radience + Color(1.0f, 1.0f, 1.0f));	// HDR->LDR
		}
	}

	clampColor(pixel_radience);

	return pixel_radience;
}

// Ͷ��һ�����ߣ��õ����ߵ�redience
Color Scene::castRayBVH(Ray& ray)
{
	Color pixel_radience = Color(0, 0, 0);

	Intersection intersection;
	if (bvh_->intersection(ray, intersection))	// ������ߺ����������н���
	{
		if (!intersection.material->isLight())	// ���û�д򵽹�Դ��
		{
			pixel_radience = trace(intersection, -ray.direction, 1);
		}
		else
		{
			pixel_radience = intersection.material->Le;
		}
	}
	else
	{
		if (skybox_ != nullptr)
		{
			pixel_radience = skybox_->sample(ray);
			pixel_radience = pixel_radience / (pixel_radience + Color(1.0f, 1.0f, 1.0f));	// HDR->LDR
		}
	}

	clampColor(pixel_radience);

	return pixel_radience;
}

// ����������;��淴��ʹ�ò�ͬ�Ĳ���
Vec Scene::HemisphereSample(Intersection& p)
{
	// �ھֲ�����ϵ�а����ϲ�������ķ���
	float u1 = getRand(), u2 = getRand();
	float z;
	if(getRand() < p.material->Ks.max())
		z = std::pow(u1, 1.0f / (1.0f + p.material->Ns));
	else
		z = std::fabs(1 - 2.0f * u1);
	float r = std::sqrt(std::max(0.0f, 1 - z * z));
	float phi = 2 * PI * u2;
	Vec local_wi = Vec(r * std::cos(phi), r * std::sin(phi), z);

	// ����������ϵ��
	Vec B, C;
	Vec &normal = p.normal;
	if (std::fabs(normal.x) > std::fabs(normal.y))
	{
		float inv_len = 1.0f / std::sqrt(normal.x * normal.x + normal.z * normal.z);
		C = Vec(normal.z * inv_len, 0.0f, -normal.x * inv_len);
	}
	else
	{
		float inv_len = 1.0f / std::sqrt(normal.y * normal.y + normal.z * normal.z);
		C = Vec(0.0f, normal.z * inv_len, -normal.y * inv_len);
	}
	B = cross(C, p.normal);
	return B * local_wi.x + C * local_wi.y + normal * local_wi.z;
}

// https://blog.csdn.net/weixin_44176696/article/details/113418991?spm=1001.2014.3001.5502#_571
// ����ʵ�飬�ٶȸ�����6s->7s
Vec Scene::HemisphereSample2(Intersection& p)
{
	return (getRandomVec() + p.normal);
}


// p����ɫ��λ�ã�x�ǹ�Դλ��
bool Scene::isLightBlock(Intersection& p, Intersection& x, Vec &wi)
{
	Intersection q;
	if (dot(wi, p.normal) < 0 || dot(wi,x.normal) > 0)
	{
		return true;
	}

	Ray ray(p.position, wi, 1.0f);
	if (getIntersection(ray, q))
	{
		if ((x.position - q.position).normSquare() > epsilon)
		{
			return true;
		}
	}

	return false;
}

Color Scene::trace(Intersection& p, Vec wo, int depth)
{
	// �����������׷�����
	if (depth > max_depth_)
	{
		return Color(0, 0, 0);
	}

	Color light_dir(0, 0, 0);	// ֱ�ӹ���
	Color light_indir(0, 0, 0);		// ��ӹ���

	// �ӹ�Դ����
	for (int i = 0; i < triMeshs_.size(); ++i)
	{
		TriMesh* triMesh = triMeshs_[i];
		auto &light_tris = triMesh->light_tris_;
		for (int j = 0; j < light_tris.size(); ++j)
		{
			Intersection x;
			float pdf;	// possibility distribution function ���ʷֲ�����
			light_tris[j].sampleLight(x, pdf);		// �ӹ�Դ�в����õ������Ϣ�Ͳ�������
			Vec wi = (x.position - p.position).normalization();	// ���䷽��

			if (isLightBlock(p, x, wi))	// �����Դ����ס
			{
				continue;
			}

			float theta = std::max(dot(p.normal,wi), 0.0f);
			float thetap = std::max(dot(x.normal, -wi), 0.0f);

			light_dir += x.material->Le * p.material->brdf(wi, wo, p) * theta * thetap / (x.position - p.position).normSquare() / pdf;
		}
	}

	// ����պн��в���
	if (skybox_ != nullptr)
	{

	}

	// �������ķ�������� 
	// �ڰ��������������һ������
	float pdf_hemi = 0.5f / PI;	// �ڰ����ϵĲ�������
	Vec wi = HemisphereSample(p).normalization();

	Ray newRay(p.position, wi, 1.0f);
	Intersection x;
	if (getRandFloatNum(0, 1) > p_RR)	// ����˹����ת����
	{
		light_indir = Color(0, 0, 0);
	}

	if (getIntersection(newRay, x))		// ������ߴ򵽳����еķǹ�Դ��x
	{
		if (getRandFloatNum(0, 1) > p_RR)	// ����˹����ת����
		{
			light_indir = Color(0, 0, 0);
		}
		else
		{
			if (!x.material->isLight())
			{
				light_indir = trace(x, -wi, depth + 1) * p.material->brdf(wi, wo, p) * dot(wi, p.normal) / (x.position - p.position).normSquare()/ pdf_hemi / p_RR;
			}
		}
	}
	else
	{
		if (skybox_ != nullptr)
		{
			light_indir = skybox_->sample(newRay) * p.material->brdf(wi, wo, p) / pdf_hemi * dot(wi, p.normal);
		}
	}

	return light_indir + light_dir;
}
 