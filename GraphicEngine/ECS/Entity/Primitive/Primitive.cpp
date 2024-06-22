#include "Primitive.h"
#include<GraphicEngine/D3D11/MeshAndTextureResources/Texture.h>

Primitive::Primitive(Mesh* mesh_Data, int primitiveID,
	VertexShader* vShad, PixelShader* pShad, VertexBuffer* vBuf, IndexBuffer* iBuf, 
	ConstantBuffer* cBuf, void* c_Buff,
	std::vector<Texture*>& listtextures, std::vector<Texture*>& listtexturesNormal, bool normalmap, unsigned int numberOftextures,
	bool frontfaceculling, std::wstring primitiveName, Primitive_texture_Binding_type primitive_tex_Type)
	:
	mesh_Data(mesh_Data),
	primitive_id(primitiveID),
	vertexShader(vShad),
	pixelShader(pShad),
	vertexBuffer(vBuf),
	indexBuffer(iBuf),
	constantBuffer(cBuf),
	cBuff(c_Buff),
	number_textures(numberOftextures),
	front_face_culling(frontfaceculling),
	back_face_culling(!frontfaceculling),
	primitive_Name(primitiveName),
	normal_map(normalmap),
	primitive_texture_Type(primitive_tex_Type)
{

	for (unsigned int i = 0; i < number_textures; ++i)
	{
		TextureDetails* td = new TextureDetails();
		td->pTexture = listtextures[i];
		td->tex_name = td->pTexture->GetTextureName();
		list_textures.push_back(td);
		direct_Texture_list.push_back(td->pTexture);
	}

	if (normalmap)
	{
		for (unsigned int i = 0; i < number_textures; ++i)
		{
			TextureDetails* td = new TextureDetails();
			td->pTexture = listtexturesNormal[i];
			td->tex_name = listtexturesNormal[i]->GetTextureName();
			list_textures_normal.push_back(td);
			direct_Texture_Normal_list.push_back(td->pTexture);
		}
	}
}

Primitive::~Primitive()
{
	if (!list_textures.empty())
	{
		for (TextureDetails* cur : list_textures)
			delete cur;
	}

	if (!list_textures_normal.empty())
	{
		for (TextureDetails* cur : list_textures_normal)
			delete cur;
	}
}

Mesh* Primitive::GetMesh()
{
    return mesh_Data;
}

void* Primitive::GetCbufferPointer()
{
    return cBuff;
}

VertexShader* Primitive::GetVertexShader()
{
    return vertexShader;
}

PixelShader* Primitive::GetPixelShader()
{
    return pixelShader;
}

VertexBuffer* Primitive::GetVertexBuffer()
{
    return vertexBuffer;
}

IndexBuffer* Primitive::GetIndexBuffer()
{
    return indexBuffer;
}

ConstantBuffer* Primitive::GetConstantBuffer()
{
    return constantBuffer;
}

const bool& Primitive::GetFrontFaceCulling()
{
    return front_face_culling;
}

const Primitive_texture_Binding_type& Primitive::Get_Primitive_texture_Type()
{
    return primitive_texture_Type;
}

const std::vector<Texture*>& Primitive::Get_Texture_List()
{
    return direct_Texture_list;
}

const std::vector<Texture*>& Primitive::Get_Texture_Normal_List()
{
    return direct_Texture_Normal_list;
}

void Primitive::setConstantBuffer(void* c_buffer)
{
	cBuff = c_buffer;
}

bool Primitive::addTexture(Texture* newTex)
{
	if (!newTex) return false;

	TextureDetails* td = new TextureDetails();
	td->pTexture = newTex;
	td->tex_name = newTex->GetTextureName();

	list_textures.push_back(td);

	number_textures = list_textures.size();
	return true;
}

bool Primitive::deleteTexture(std::wstring tex_name)
{
	if (tex_name.empty()) return false;

	for (int i = 0; i < list_textures.size(); ++i)
	{
		if (list_textures[i]->tex_name == tex_name)
		{
			delete list_textures[i];
			list_textures.erase(list_textures.begin() + i);

			number_textures = list_textures.size();
			return true;
		}
	}

	return false;
}

bool Primitive::fillTexture(Texture* newTex, std::wstring tex_name)
{
	if (tex_name.empty() || !newTex) return false;

	for (int i = 0; i < list_textures.size(); ++i)
	{
		if (list_textures[i]->tex_name == tex_name)
		{
			list_textures[i]->tex_name = tex_name;
			list_textures[i]->pTexture = newTex;
			return true;
		}
	}

	return false;
}

bool Primitive::replaceTexture(Texture* newTex, std::wstring tex_name)
{
	if (tex_name.empty() || !newTex) return false;

	for (int i = 0; i < list_textures.size(); ++i)
	{
		if (list_textures[i]->tex_name == tex_name)
		{
			list_textures[i]->tex_name = tex_name;
			list_textures[i]->pTexture = newTex;
			return true;
		}
	}

	return false;
}
