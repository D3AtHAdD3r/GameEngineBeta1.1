#include "IndexBuffer.h"
#include<GraphicEngine/D3D11/D3D11Globals/D3D11Globals.h>




IndexBuffer::IndexBuffer(void* list_indices, const unsigned int& size_list, const int& u_ID, std::wstring mesh_Name)
	:
	meshName(mesh_Name),
	uID(u_ID)
{
	HRESULT hr;

	if (mesh_Name.empty())
		throw NORMAL_EXCEPT("mesh_Name.empty, IndexBuffer constructor failed");

	if(u_ID < 0)
		throw NORMAL_EXCEPT("u_ID < 0, IndexBuffer constructor failed");

	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = SIZE_VARIABLE * size_list;
	buff_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_indices;

	GFX_THROW_INFO(D3D11Globals::Get()->GetDevice()->CreateBuffer(&buff_desc, &init_data, &iBuffer));

}

IndexBuffer::~IndexBuffer()
{
	if (iBuffer)
	{
		iBuffer->Release();
		iBuffer = nullptr;
	}
}

ID3D11Buffer* IndexBuffer::getIndexBuffer()
{
	return iBuffer;
}

std::wstring IndexBuffer::getMeshName()
{
	return meshName;
}

const int& IndexBuffer::GetUid()
{
	return uID;
}

void IndexBuffer::incrementCounter()
{
	UserCounter++;
}

void IndexBuffer::decrementCounter()
{
	UserCounter--;
}
