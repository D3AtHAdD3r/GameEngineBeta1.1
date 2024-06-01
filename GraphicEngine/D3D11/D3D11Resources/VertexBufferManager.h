#include<unordered_map>
#include<string>

class VertexBuffer;
struct D3D11_INPUT_ELEMENT_DESC;

class VertexBufferManager
{
public:
	VertexBufferManager();
	~VertexBufferManager();

public:
	VertexBuffer* CreateVertexBuffer(void* list_vertices, unsigned int size_vertex, unsigned size_list_vertex, void* shader_byte_code, size_t size_byte_shader, D3D11_INPUT_ELEMENT_DESC* layout, unsigned size_layout, std::wstring mesh_Name, const int& u_id);
	bool freeVertexBuffer(VertexBuffer* pVShader);
	void releaseAll();

private:
	bool check_Exist(const int& u_id);
	bool DeleteVertexBuffer(VertexBuffer* pVShader);
	bool DeleteVertexBuffer(const int& u_id);
	void DeleteVertexBuffer_direct(VertexBuffer* pVShader);

private:
	std::unordered_map<int, VertexBuffer*> VertexBufferContainer;
	int ResourceCount = 0;
};