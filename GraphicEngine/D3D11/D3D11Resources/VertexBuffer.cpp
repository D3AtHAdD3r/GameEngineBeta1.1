#include "VertexBuffer.h"
#include<GraphicEngine/D3D11/D3D11Globals/D3D11Globals.h>



VertexBuffer::VertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list_vertex, void* shader_byte_code, size_t size_byte_shader, D3D11_INPUT_ELEMENT_DESC* layout, UINT size_layout, std::wstring mesh_Name, const int& u_ID)
	:
	meshName(mesh_Name),
	uID(u_ID)
{
	HRESULT hr;

	if(!list_vertices || !size_vertex || !size_list_vertex || !shader_byte_code || !size_byte_shader || !layout || mesh_Name.empty() || u_ID < 0)
		throw NORMAL_EXCEPT("VertexBuffer constructor failed");

	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = size_vertex * size_list_vertex;
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_vertices;

	GFX_THROW_INFO(D3D11Globals::Get()->GetDevice()->CreateBuffer(&buff_desc, &init_data, &vBuffer));
	GFX_THROW_INFO(D3D11Globals::Get()->GetDevice()->CreateInputLayout(layout, size_layout, shader_byte_code, size_byte_shader, &iLayout));

}

VertexBuffer::~VertexBuffer()
{
	if (vBuffer)
	{
		vBuffer->Release();
		vBuffer = nullptr;
	}
	if (iLayout)
	{
		iLayout->Release();
		iLayout = nullptr;
	}
}

ID3D11Buffer* VertexBuffer::getVertexBuffer()
{
	return vBuffer;
}

ID3D11InputLayout* VertexBuffer::getInputLayout()
{
	return iLayout;
}

std::wstring VertexBuffer::getMeshName()
{
	return meshName;
}

void VertexBuffer::incrementCounter()
{
	UserCounter++;
}

void VertexBuffer::decrementCounter()
{
	UserCounter--;
}
