#include<string>
#include<unordered_map>

class Mesh;
class VertexMesh;
struct MaterialSlot;

class MeshManager
{
public:
	MeshManager(const std::unordered_map<int, std::wstring>& File_Map);
	~MeshManager();

public:
	Mesh* CreateMesh(const int& u_id);
	Mesh* CreateMesh(
		VertexMesh* vertex_list_data, unsigned int vertex_list_size,
		unsigned int* index_list_data, unsigned int index_list_size,
		MaterialSlot* material_slot_list, unsigned int material_slot_list_size,
		std::wstring MeshName, const int& u_id
	);

	//only supports 1 material(yet) //for testing
	Mesh* CreateTerrainMesh(unsigned int w, unsigned int h, const int& u_id, std::wstring MeshName);
	
public:
	void releaseAll();
	bool freeMesh(Mesh* pMesh);


private:
	bool check_Exist(const int& u_id);
	bool deleteMesh(Mesh* pMesh);
	bool deleteMesh(const int& u_id);
	void deleteMesh_direct(Mesh* pMesh);

private:
	std::wstring GetFileName(const int& uID);

private:
	std::unordered_map<int, Mesh*> MeshContainer;
	unsigned int ResourceCount = 0;  //number of meshes

private:
	std::unordered_map<int, std::wstring> FileMap;
};