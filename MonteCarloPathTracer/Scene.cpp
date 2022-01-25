#include "Scene.h"
#include <limits.h>
#include "utility.h"

Scene::~Scene()
{
	if (bvh_) delete bvh_;
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

// Ͷ��һ�����ߣ��õ����ߵ�redience
Color Scene::castRayBVH(Ray& ray)
{
	Color pixel_radience = Color(0, 0, 0);

	Intersection intersection;
	if (bvh_->intersection(ray, intersection))	// ������ߺ����������н���
	{
		if (intersection.material->image_texture != nullptr)
		{
			pixel_radience = intersection.material->getTextureColor(intersection.uv.u, intersection.uv.v);
		}
		else if (!intersection.material->isLight())	// ���û�д򵽹�Դ��
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
		}
	}
	

	pixel_radience.x = pixel_radience.x < 1.0f ? pixel_radience.x : 1.0f;
	pixel_radience.y = pixel_radience.y < 1.0f ? pixel_radience.y : 1.0f;
	pixel_radience.z = pixel_radience.z < 1.0f ? pixel_radience.z : 1.0f;

	return pixel_radience;
}

// ��ʹ�ð�Χ�м���
Color Scene::castRay(Ray& ray)
{
	Color pixel_radience = Color(0, 0, 0);

	Intersection intersection;

	float t_min = FLT_MAX;
	for (int i = 0; i < triMeshs_.size(); ++i)
	{
		TriMesh* trimesh = triMeshs_[i];
		for (int j = 0; j < trimesh->tris_.size(); ++j)
		{
			Intersection inter_tmep;
			auto tri = trimesh->tris_[j];
			bool tag = rayTriIntersect(ray, *tri, inter_tmep);
			if (tag && ray.t < t_min)
			{
				intersection = inter_tmep;
				t_min = ray.t;
			}
		}
	}

	if (t_min != FLT_MAX)	// ������ߺ����������н���
	{
		if (intersection.material->image_texture != nullptr)
		{
			pixel_radience = intersection.material->getTextureColor(intersection.uv.u, intersection.uv.v);
		}
		else if (!intersection.material->isLight())	// ���û�д򵽹�Դ��
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
		}
	}


	pixel_radience.x = pixel_radience.x < 1.0f ? pixel_radience.x : 1.0f;
	pixel_radience.y = pixel_radience.y < 1.0f ? pixel_radience.y : 1.0f;
	pixel_radience.z = pixel_radience.z < 1.0f ? pixel_radience.z : 1.0f;

	return pixel_radience;
}



Vec3f Scene::MonteCarloSample(Intersection& p, Vec3f &wo)
{
	// �ھֲ�����ϵ�а����ϲ�������ķ���
	float u1 = getRandFloatNum(0, 1), u2 = getRandFloatNum(0, 1);
	float z = std::fabs(1 - 2.0f * u1);
	float r = std::sqrt(std::max(0.0f, 1 - z * z));
	float phi = 2 * PI * u2;
	Vec3f local_wi = Vec3f(r * std::cos(phi), r * std::sin(phi), z);

	// ����������ϵ��
	Vec3f B, C;
	Vec3f &normal = p.normal;
	if (std::fabs(normal.x) > std::fabs(normal.y))
	{
		float inv_len = 1.0f / std::sqrt(normal.x * normal.x + normal.z * normal.z);
		C = Vec3f(normal.z * inv_len, 0.0f, -normal.x * inv_len);
	}
	else
	{
		float inv_len = 1.0f / std::sqrt(normal.y * normal.y + normal.z * normal.z);
		C = Vec3f(0.0f, normal.z * inv_len, -normal.y * inv_len);
	}

	return B * local_wi.x + C * local_wi.y + normal * local_wi.z;
}

bool Scene::isLightBlock(Intersection& p, Intersection& x, Vec3f &wi)
{
	Intersection q;
	if (dot(wi, p.normal) < 0 || dot(wi,x.normal) > 0)
	{
		return true;
	}

	Ray ray(p.position, wi, 1.0f);
	if (bvh_->intersection(ray, q))
	{
		if ((x.position - q.position).norm() > epsilon)
		{
			return true;
		}
	}

	return false;
}

Color Scene::trace(Intersection& p, Vec3f wo, int depth)
{
	// �����������׷�����
	if (depth > max_depth_)
	{
		return Color(0, 0, 0);
	}

	Color light_dir = Color(0, 0, 0);	// ֱ�ӹ���
	Color light_indir = Color(0, 0, 0);		// ��ӹ���

	// �ӹ�Դ����
	for (int i = 0; i < triMeshs_.size(); ++i)
	{
		TriMesh* triMesh = triMeshs_[i];
		auto light_tris = triMesh->light_tris_;
		for (int j = 0; j < light_tris.size(); ++j)
		{
			Intersection x;
			float pdf;	// possibility distribution function ���ʷֲ�����
			light_tris[j]->sampleLight(x, pdf);		// �ӹ�Դ�в����õ������Ϣ�Ͳ�������
			Vec3f wi = (x.position - p.position).normalization();	// ���䷽��

			if (isLightBlock(p, x, wi))	// �����Դ����ס
			{
				continue;
			}

			float theta = dot(p.normal,wi);
			float thetap = dot(x.normal, -wi);

			light_dir += x.material->Le * p.material->brdf(wi, wo, p.material, p.normal) * theta * thetap / std::pow((x.position - p.position).norm(), 2.0f) / pdf;
		}
	}

	// �������ķ�������� 
	// �ڰ��������������һ������
	float pdf_hemi = 0.5f / PI;	// �ڰ����ϵĲ�������
	Vec3f wi = MonteCarloSample(p, wo).normalization();

	Ray newRay(p.position, wi, 1.0f);
	Intersection x;
	if (bvh_->intersection(newRay, x))		// ������ߴ򵽳����еķǹ�Դ��x
	{
		if (getRandFloatNum(0, 1) > p_RR)	// ����˹����ת����
		{
			light_indir = Color(0, 0, 0);
		}
		else
		{
			if (!x.material->isLight())
			{
				light_indir = trace(x, -wi, depth + 1) * p.material->brdf(wi, wo, p.material, p.normal)* dot(wi, p.normal) / pdf_hemi / p_RR;
			}
		}
	}
	else
	{
		if (skybox_ != nullptr)
		{
			light_indir = skybox_->sample(newRay) * p.material->brdf(wi, wo, p.material, p.normal);// / pdf_hemi;
		}
	}

	Color tex_color = p.material->getTextureColor(p.uv.u, p.uv.v) * p.material->brdf(wi, wo, p.material, p.normal);

	return light_dir + light_indir + tex_color;
}
 