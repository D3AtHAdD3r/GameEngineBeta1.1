#include "ECSToRendererData.h"
#include <GraphicEngine/Utilities/Headers/Headers.h>
#include<GraphicEngine/ECS/Scene/Scene.h>
#include<GraphicEngine/ECS/Entity/Primitive/Primitive.h>
#include<GraphicEngine/Renderer/RendererHeaders/RendererStructs.h>

#include<GraphicEngine/D3D11/D3D11Resources/VertexShader.h>
#include<GraphicEngine/D3D11/D3D11Resources/PixelShader.h>
#include<GraphicEngine/D3D11/D3D11Resources/IndexBuffer.h>
#include<GraphicEngine/D3D11/D3D11Resources/ConstantBuffer.h>
#include<GraphicEngine/D3D11/D3D11Resources/VertexBuffer.h>
#include<GraphicEngine/D3D11/MeshAndTextureResources/Mesh.h>

ECSToRendererData* ECSToRendererData::pECSToRender = nullptr;

void ECSToRendererData::Create()
{
	if (pECSToRender)
		throw NORMAL_EXCEPT("ECSToRender already exist.");

	pECSToRender = new ECSToRendererData();
}

ECSToRendererData* ECSToRendererData::Get()
{
	return pECSToRender;
}

bool ECSToRendererData::fill_Renderer_PreBindData(Scene* pScene, Renderer_PreBindData* pBindData)
{
	if (!pScene || !pBindData) return false;

	color_Scene color = pScene->getColorBackground();
	pBindData->color[0] = color.r; 
	pBindData->color[1] = color.g; 
	pBindData->color[2] = color.b; 
	pBindData->color[3] = color.a;

	pBindData->ClearRenderTargetView = pScene->getclearRenderTargetView();
	pBindData->ClearDepthStencil = pScene->getclearDepthStencil();
	pBindData->UseDepthStencil = pScene->getUseDepthStencil();
	pBindData->pRTV = pScene->getRTV();
	pBindData->pDSV = pScene->getDSV();
	pBindData->scene_width = pScene->getWidth();
	pBindData->scene_height = pScene->getHeight();
	//primitive topology = default;

	return true;
}

bool ECSToRendererData::fill_Renderer_MainBindData(Primitive* pPrim, Renderer_BindingData* pBindData)
{
	if (!pPrim || !pBindData) return false;

	pBindData->vShader = pPrim->GetVertexShader()->getVertexShader();
	pBindData->pShader = pPrim->GetPixelShader()->getPixelShader();
	pBindData->pIndexbuffer = pPrim->GetIndexBuffer()->getIndexBuffer();

	pBindData->cbuffer = pPrim->GetConstantBuffer()->getConstantBuffer();
	pBindData->pCBuffer_data = pPrim->GetCbufferPointer();

	pBindData->vBuffer = pPrim->GetVertexBuffer()->getVertexBuffer();
	pBindData->iLayout = pPrim->GetVertexBuffer()->getInputLayout();
	pBindData->size_vertex = pPrim->GetMesh()->getVertexSize();

	pBindData->FrontFaceCull = pPrim->GetFrontFaceCulling();
	pBindData->MaterialCount = pPrim->GetMesh()->GetMaterialCount();

	for (size_t m = 0; m < pPrim->GetMesh()->GetMaterialCount(); ++m)
	{
		SIZE_T start_index_location = pPrim->GetMesh()->GetMaterials()[m].start_index;
		SIZE_T index_count = pPrim->GetMesh()->GetMaterials()[m].num_indices;
		pBindData->Material_Draw_Details.push_back({ start_index_location , index_count });
	}

	pBindData->list_textures_Material = pPrim->Get_Texture_List_Material();
	pBindData->list_textures_Custom = pPrim->Get_Texture_List_Custom();
	pBindData->list_textures_Normal_Map = pPrim->Get_Texture_List_Normal_Map();
	pBindData->list_textures_Height_Map = pPrim->Get_Texture_List_Height_Map();

	return true;
}
