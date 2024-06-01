#include "D3DManager.h"
#include<GraphicEngine/D3D11/D3D11Resources/ConstantBufferManager.h>
#include<GraphicEngine/D3D11/D3D11Resources/VertexShaderManager.h>
#include<GraphicEngine/D3D11/D3D11Resources/PixelShaderManager.h>
#include<GraphicEngine/D3D11/D3D11Resources/VertexBufferManager.h>
#include<GraphicEngine/D3D11/D3D11Resources/IndexBufferManager.h>

D3D11Manager::D3D11Manager()
{
	pVertexBufferManager = new VertexBufferManager();
	pIndexBufferManager = new IndexBufferManager();
	pVShaderManager = new VertexShaderManager();
	pPShaderManager = new PixelShaderManager();
	pConstantBufferManager = new ConstantBufferManager();
}

D3D11Manager::~D3D11Manager()
{
	if (pVertexBufferManager)			delete pVertexBufferManager;
	if (pIndexBufferManager)			delete pIndexBufferManager;
	if (pVShaderManager)				delete pVShaderManager;
	if (pPShaderManager)				delete pPShaderManager;
	if (pConstantBufferManager)			delete pConstantBufferManager;
}

void D3D11Manager::releaseAll()
{
	if (pVertexBufferManager)			pVertexBufferManager->releaseAll();
	if (pIndexBufferManager)			pIndexBufferManager->releaseAll();
	if (pVShaderManager)				pVShaderManager->releaseAll();
	if (pPShaderManager)				pPShaderManager->releaseAll();
	if (pConstantBufferManager)			pConstantBufferManager->releaseAll();
}
