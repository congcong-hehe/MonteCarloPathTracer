#include "TriMesh.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <assert.h>

void TriMesh::LoadObjFile(std::string file_path, std::string file_name)
{
	std::vector<Vec3f> vs;
	std::vector<Vec3f> vns;
	std::vector<UV> vts;

	file_name = file_path + file_name;
	std::ifstream in(file_name);
	
	assert(in);

	std::string line;
	std::shared_ptr<Material> curr_material = nullptr;
	int count = 0;
	while (std::getline(in, line))
	{
		std::string tag;
		std::istringstream is(line);

		is >> tag;

		if (tag == "v")
		{
			Vec3f pos;
			is >> pos.x >> pos.y >> pos.z;
			vs.push_back(pos);
		}
		if (tag == "vt")
		{
			UV uv;
			is >> uv.u >> uv.v;

			uv.u -= std::floor(uv.u);
			uv.v -= std::floor(uv.v);

			vts.push_back(uv);
		}
		if (tag == "vn")
		{
			Vec3f norm;
			is >> norm.x >> norm.y >> norm.z;
			vns.push_back(norm);
		}
		if (tag == "f")
		{
			std::shared_ptr<Tri> tri = std::make_shared<Tri>();
			int v[3];
			int vn[3];
			int vt[3];
			for (int i = 0; i < 3; ++i)
			{
				char trash;
				is >> v[i] >> trash >> vn[i] >> trash >> vt[i];
				v[i] --;
				vn[i] --;
				vt[i] --;
			}
			tri->material = curr_material;

			tri->init(vs, vns, vts, v, vn, vt);
			tris_.push_back(tri);

			if (curr_material->isLight())	// ����ǹ�Դ��������Ƭ�������ڹ�Դ����Ƭ����ϵ��
			{
				light_tris_.push_back(tri);
			}
		}

		if (tag == "usemtl")
		{
			std::string mat_name;
			is >> mat_name;
			curr_material = material_map_[mat_name];
		}
	}
	num_tris_ = tris_.size();
	num_vtxs_ = vs.size();

	in.close();

	std::cout << "Load succees! num of vtx: " << num_vtxs_ << " num of tris: " << num_tris_ << std::endl;

}

void TriMesh::LoadMtlFile(std::string file_path, std::string file_name)
{
	std::string mtl_name = file_path + file_name;
	std::ifstream in(mtl_name);

	std::string line;
	int material_index = 0;	// ��ǰ����Ĳ��ʵ��±� 
	while (std::getline(in, line))
	{
		std::string tag;
		std::istringstream is(line);

		is >> tag;

		if (tag == "newmtl")
		{
			std::shared_ptr<Material> material = std::make_shared<Material>();
			std::string mat_name;
			is >> mat_name;
			material_index = materials_.size();
			materials_.push_back(material);
			material_map_[mat_name] = material;
		}

		if (tag == "Kd")
		{
			is >> materials_[material_index]->Kd.x >> materials_[material_index]->Kd.y >> materials_[material_index]->Kd.z;
		}

		if (tag == "Ks")
		{
			is >> materials_[material_index]->Ks.x >> materials_[material_index]->Ks.y >> materials_[material_index]->Ks.z;
		}

		if (tag == "Le")
		{
			is >> materials_[material_index]->Le.x >> materials_[material_index]->Le.y >> materials_[material_index]->Le.z;
		}

		if (tag == "Ns")
		{
			is >> materials_[material_index]->Ns;
		}

		if (tag == "map_Kd")
		{
			std::string texture_name;
			is >> texture_name;
			materials_[material_index]->image_texture = std::make_shared<ImageTexture>(file_path + texture_name);
		}
	}

	in.close();
}
