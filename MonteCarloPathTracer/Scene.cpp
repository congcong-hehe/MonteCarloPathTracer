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

// 投射一根光线，得到光线的redience
Color Scene::castRayBVH(Ray& ray)
{
	Color pixel_radience = Color(0, 0, 0);

	Intersection intersection;
	if (bvh_->intersection(ray, intersection))	// 如果光线和三角网格有交点
	{
		if (intersection.material->image_texture != nullptr)
		{
			pixel_radience = intersection.material->getTextureColor(intersection.uv.u, intersection.uv.v);
		}
		else if (!intersection.material->isLight())	// 如果没有打到光源上
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

// 不使用包围盒加速
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

	if (t_min != FLT_MAX)	// 如果光线和三角网格有交点
	{
		if (intersection.material->image_texture != nullptr)
		{
			pixel_radience = intersection.material->getTextureColor(intersection.uv.u, intersection.uv.v);
		}
		else if (!intersection.material->isLight())	// 如果没有打到光源上
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
	// 在局部坐标系中半球上产生随机的方向
	float u1 = getRandFloatNum(0, 1), u2 = getRandFloatNum(0, 1);
	float z = std::fabs(1 - 2.0f * u1);
	float r = std::sqrt(std::max(0.0f, 1 - z * z));
	float phi = 2 * PI * u2;
	Vec3f local_wi = Vec3f(r * std::cos(phi), r * std::sin(phi), z);

	// 在世界坐标系下
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
	// 如果超过最大的追踪深度
	if (depth > max_depth_)
	{
		return Color(0, 0, 0);
	}

	Color light_dir = Color(0, 0, 0);	// 直接光照
	Color light_indir = Color(0, 0, 0);		// 间接光照

	// 从光源采样
	for (int i = 0; i < triMeshs_.size(); ++i)
	{
		TriMesh* triMesh = triMeshs_[i];
		auto light_tris = triMesh->light_tris_;
		for (int j = 0; j < light_tris.size(); ++j)
		{
			Intersection x;
			float pdf;	// possibility distribution function 概率分布函数
			light_tris[j]->sampleLight(x, pdf);		// 从光源中采样得到点的信息和采样概率
			Vec3f wi = (x.position - p.position).normalization();	// 入射方向

			if (isLightBlock(p, x, wi))	// 如果光源被遮住
			{
				continue;
			}

			float theta = dot(p.normal,wi);
			float thetap = dot(x.normal, -wi);

			light_dir += x.material->Le * p.material->brdf(wi, wo, p.material, p.normal) * theta * thetap / std::pow((x.position - p.position).norm(), 2.0f) / pdf;
		}
	}

	// 从其他的反射物采样 
	// 在半球面上随机产生一个方向
	float pdf_hemi = 0.5f / PI;	// 在半球上的采样概率
	Vec3f wi = MonteCarloSample(p, wo).normalization();

	Ray newRay(p.position, wi, 1.0f);
	Intersection x;
	if (bvh_->intersection(newRay, x))		// 如果光线打到场景中的非光源点x
	{
		if (getRandFloatNum(0, 1) > p_RR)	// 俄罗斯轮盘转测试
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
 