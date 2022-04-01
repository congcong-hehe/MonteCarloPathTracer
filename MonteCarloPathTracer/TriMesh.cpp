#include "TriMesh.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <assert.h>

void TriMesh::LoadObjFile(std::string file_path, std::string file_name)
{
	std::vector<Vec> vs;
	std::vector<Vec> vns;
	std::vector<UV> vts;

	file_name = file_path + file_name;
	std::ifstream in(file_name);
	
	assert(in);

	std::string line;
	Material* curr_material = nullptr;
	int count = 0;
	while (std::getline(in, line))
	{
		std::string tag;
		std::istringstream is(line);

		is >> tag;

		if (tag == "#")
			continue;

		if (tag == "v")
		{
			Vec pos;
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
			Vec norm;
			is >> norm.x >> norm.y >> norm.z;
			vns.push_back(norm.normalization());
		}
		if (tag == "f")
		{
			Tri tri;
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
			tri.material = curr_material;

			tri.init(vs, vns, vts, v, vn, vt);
			tris_.push_back(tri);

			if (curr_material->isLight())	// 如果是光源的三角面片，将属于光源的面片加入系列
			{
				light_tris_.push_back(tri);
			}
		}

		if (tag == "usemtl")
		{
			std::string mat_name;
			is >> mat_name;
			curr_material = &materials_[material_map_[mat_name]];
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
	int material_index = 0;	// 当前读入的材质的下标 
	while (std::getline(in, line))
	{
		std::string tag;
		std::istringstream is(line);

		is >> tag;

		if (tag == "newmtl")
		{
			std::string mat_name;
			is >> mat_name;
			material_index = materials_.size();
			materials_.emplace_back(Material());
			material_map_[mat_name] = material_index;
		}

		else if (tag == "Kd")
		{
			is >> materials_[material_index].Kd.x >> materials_[material_index].Kd.y >> materials_[material_index].Kd.z;
		}

		else if (tag == "Ks")
		{
			is >> materials_[material_index].Ks.x >> materials_[material_index].Ks.y >> materials_[material_index].Ks.z;
		}

		else if (tag == "Le")
		{
			is >> materials_[material_index].Le.x >> materials_[material_index].Le.y >> materials_[material_index].Le.z;
		}

		else if (tag == "Ns")
		{
			is >> materials_[material_index].Ns;
		}

		else if (tag == "map_Kd")
		{
			std::string texture_name;
			is >> texture_name;
			materials_[material_index].image_texture = new Image(file_path + texture_name);
		}
	}

	in.close();
}

