#pragma once
#include <vector>
#include <string>
#include "Vertex.h"
#include "Tri.h"
#include "Material.h"
#include <map>

class TriMesh
{
public:
	std::vector<Tri> tris_;		// 存储三角面片
	std::vector<Material> materials_;	// 网格中所有的材质
	std::vector<Tri> light_tris_;	// 所有属于光源的三角面片
	std::map<std::string, size_t> material_map_;		// 材质名称和材质类索引的映射

	size_t num_vtxs_{ 0 };		// 顶点的数量
	size_t num_tris_{ 0 };		// 面片的数量

	void LoadMtlFile(std::string file_path, std::string file_name);
	void LoadObjFile(std::string file_path, std::string file_name);
};

