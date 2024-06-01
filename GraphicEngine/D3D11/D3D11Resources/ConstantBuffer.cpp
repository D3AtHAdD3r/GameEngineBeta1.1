#include<GraphicEngine/D3D11/D3D11Resources/ConstantBuffer.h>
#include"GraphicEngine/D3D11/D3D11Globals/D3D11Globals.h"

ConstantBuffer::ConstantBuffer(void* buffer, const unsigned int& size_buffer, short u_ID)
	: uID(u_ID)
{
	if (u_ID < 0)
		throw NORMAL_EXCEPT("UserID < 0, Constant Buffer Constructor failed");

	HRESULT hr;

	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = size_buffer;
	buff_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = buffer;

	GFX_THROW_INFO(D3D11Globals::Get()->GetDevice()->CreateBuffer(&buff_desc, &init_data, &cBuffer));
}

ConstantBuffer::~ConstantBuffer()
{
	if (cBuffer)
	{
		cBuffer->Release();
		cBuffer = nullptr;
	}
}

ID3D11Buffer* ConstantBuffer::getConstantBuffer()
{
	return cBuffer;
}

const short& ConstantBuffer::GetUid()
{
	return uID;
}

void ConstantBuffer::incrementCounter()
{
	UserCounter++;
}

void ConstantBuffer::decrementCounter()
{
	UserCounter--;
}
