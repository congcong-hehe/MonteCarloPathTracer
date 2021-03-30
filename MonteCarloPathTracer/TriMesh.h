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
	std::vector<std::shared_ptr<Tri>> tris_;		// �洢������Ƭ
	std::vector<std::shared_ptr<Material>> materials_;	// ���������еĲ���
	std::vector<std::shared_ptr<Tri>> light_tris_;	// �������ڹ�Դ��������Ƭ
	std::map<std::string, std::shared_ptr<Material>> material_map_;		// �������ƺͲ�����������ӳ��

	int num_vtxs_{ 0 };		// ���������
	int num_tris_{ 0 };		// ��Ƭ������

	void LoadMtlFile(std::string file_path, std::string file_name);
	void LoadObjFile(std::string file_path, std::string file_name);
};

