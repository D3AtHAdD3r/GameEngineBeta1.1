#include<GraphicEngine/Utilities/Headers/HeadersD3D.h>
#include<string>


class VertexBuffer
{
private:
	VertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list_vertex, void* shader_byte_code, size_t size_byte_shader, D3D11_INPUT_ELEMENT_DESC* layout, UINT size_layout, std::wstring mesh_Name, const int& u_ID);
	~VertexBuffer();

public:
	ID3D11Buffer* getVertexBuffer();
	ID3D11InputLayout* getInputLayout();
	std::wstring getMeshName();

private:
	void incrementCounter();
	void decrementCounter();

private:
	ID3D11Buffer* vBuffer = nullptr;
	ID3D11InputLayout* iLayout = nullptr;
	std::wstring meshName;
	unsigned int UserCounter = 0;
	int uID = -1;
private:
	friend class VertexBufferManager;
};