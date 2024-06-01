#pragma once
#include<vector>
#include<string>

class VertexMesh;

struct MaterialSlot
{
	size_t start_index = 0;
	size_t num_indices = 0;
	size_t material_id = 0;
};

class Mesh
{
private:
	Mesh(const int& mesh_ID, std::wstring mesh_name);
	~Mesh();

public:
	std::vector<VertexMesh> GetVertexMesh();
	std::vector<unsigned int> GetIndicesList();
	const std::vector<MaterialSlot>& GetMaterials();

public:
	const unsigned int& getSizeVertices();
	const unsigned int& getSizeIndices();
	const unsigned int& getVertexSize();
	const size_t& GetMaterialCount();

public:
	const int& GetmeshID();
	std::wstring GetMeshName();

private:
	void incrementCounter();
	void decrementCounter();

private:
	std::vector<VertexMesh> VertexAttributes;
	std::vector<unsigned int> IndicesList;
	std::vector<MaterialSlot> MaterialList;
private:
	unsigned int SizeVertices = 0;
	unsigned int SizeIndices = 0;
	unsigned int VertexSize = 0;
	size_t MaterialCount = 0;
private:
	int uID = -1;
	std::wstring meshName;
	int UserCounter = 0;
	
private:
	friend class MeshManager;
};