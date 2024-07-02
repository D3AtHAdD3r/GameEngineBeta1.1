#include "Primitive.h"
#include<GraphicEngine/D3D11/MeshAndTextureResources/Texture.h>
#include<sstream>
#include<GraphicEngine\Utilities\UtilitiyFuncs\utilityFunctions.h>

Primitive::Primitive(Mesh* mesh_Data, int primitiveID,
	VertexShader* vShad, PixelShader* pShad,
	VertexBuffer* vBuf, IndexBuffer* iBuf,
	ConstantBuffer* cBuf, void* c_Buff,
	bool frontfaceculling,
	std::wstring primitiveName,
	const std::vector<std::pair<Entity_Texture_Type, Texture*>>& TextureList)
	:
	mesh_Data(mesh_Data),
	primitive_id(primitiveID),
	vertexShader(vShad),
	pixelShader(pShad),
	vertexBuffer(vBuf),
	indexBuffer(iBuf),
	constantBuffer(cBuf),
	cBuff(c_Buff),
	front_face_culling(frontfaceculling),
	back_face_culling(!frontfaceculling),
	primitive_Name(primitiveName)
	
{
	for (auto& [TextureType, pTex] : TextureList)
	{
		switch (TextureType)
		{
		case Entity_Texture_Type::Tex_Material:
		{
			list_textures_Material.emplace(pTex->getTextureID(), pTex);
			break;
		}
		case Entity_Texture_Type::Tex_Custom:
		{
			list_textures_Custom.emplace(pTex->getTextureID(), pTex);
			break;
		}
		case Entity_Texture_Type::Tex_Normal_Map:
		{
			list_textures_Normal_Map.emplace(pTex->getTextureID(), pTex);
			break;
		}
		case Entity_Texture_Type::Tex_Height_Map:
		{
			list_textures_Height_Map.emplace(pTex->getTextureID(), pTex);
			break;
		}
		case Entity_Texture_Type::Tex_Unknown:
		{
			std::ostringstream oss;
			oss << "Primitive constructor failed, Entity_Texture_Type::Tex_Unknown, Entiy uid, name:  "
				<< primitiveID << " " << UtilityFuncs::unicodeToMultibyte(primitiveName) << "\n";
			throw NORMAL_EXCEPT(oss.str());
			break;
		}
		default:
		{
			std::ostringstream oss;
			oss << "Primitive constructor failed, Entity_Texture_Type::Tex_Unknown, Entiy uid, name:  "
				<< primitiveID << " " << UtilityFuncs::unicodeToMultibyte(primitiveName) << "\n";
			throw NORMAL_EXCEPT(oss.str());
			break;
		}

		}
	}
	
}

Primitive::~Primitive()
{
	
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

bool Primitive::GetFrontFaceCulling()
{
    return front_face_culling;
}

const std::unordered_map<int, Texture*>& Primitive::Get_Texture_List_Material() const
{
	return list_textures_Material;
}

const std::unordered_map<int, Texture*>& Primitive::Get_Texture_List_Custom() const
{
	return list_textures_Custom;
}

const std::unordered_map<int, Texture*>& Primitive::Get_Texture_List_Normal_Map() const
{
	return list_textures_Normal_Map;
}

const std::unordered_map<int, Texture*>& Primitive::Get_Texture_List_Height_Map() const
{
	return list_textures_Height_Map;
}


void Primitive::setConstantBuffer(void* c_buffer)
{
	cBuff = c_buffer;
}


bool Primitive::AddTexture(Entity_Texture_Type Texture_Type, Texture* newTex)
{
	if (!newTex || Texture_Type == Entity_Texture_Type::Tex_Unknown) return false;

	switch (Texture_Type)
	{
	case Entity_Texture_Type::Tex_Material:
	{
		list_textures_Material.emplace(newTex->getTextureID(), newTex);
		break;
	}
	case Entity_Texture_Type::Tex_Custom:
	{
		list_textures_Custom.emplace(newTex->getTextureID(), newTex);
		break;
	}
	case Entity_Texture_Type::Tex_Normal_Map:
	{
		list_textures_Normal_Map.emplace(newTex->getTextureID(), newTex);
		break;
	}
	case Entity_Texture_Type::Tex_Height_Map:
	{
		list_textures_Height_Map.emplace(newTex->getTextureID(), newTex);
		break;
	}
	default:
	{
		return false;
	}
	}
	
	
	return true;
}

bool Primitive::DeleteTexture(const int& uid)
{
	if (uid < 0) return false;

	if (list_textures_Material.find(uid) != list_textures_Material.end())
	{
		list_textures_Material.erase(uid);
		return true;
	}

	if (list_textures_Custom.find(uid) != list_textures_Custom.end())
	{
		list_textures_Custom.erase(uid);
		return true;
	}

	if (list_textures_Normal_Map.find(uid) != list_textures_Normal_Map.end())
	{
		list_textures_Normal_Map.erase(uid);
		return true;
	}

	if (list_textures_Normal_Map.find(uid) != list_textures_Normal_Map.end())
	{
		list_textures_Normal_Map.erase(uid);
		return true;
	}

	return false;
}


