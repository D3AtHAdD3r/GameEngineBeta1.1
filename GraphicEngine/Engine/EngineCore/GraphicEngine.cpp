#include "GraphicEngine.h"
#include<GraphicEngine/Window/Window.h>
#include<GraphicEngine/Utilities/Headers/Headers.h>
#include<GraphicEngine/Renderer/RendererHeaders/RendererStructs.h>
#include<GraphicEngine/D3D11/D3D11Headers/D3D11Structs.h>
#include<GraphicEngine/Window/WindowGlobals.h>
#include<GraphicEngine/InputHandling/InputSystem.h>
#include<GraphicEngine/Renderer/RendererDX11/RendererDX11.h>


GraphicEngine* GraphicEngine::pGraphicEngine = nullptr;


GraphicEngine::GraphicEngine(IApplication* inst_App, RenderData* p_RenderData)
	:
	instApp(inst_App)
{
	if (!inst_App || !p_RenderData)
		throw NORMAL_EXCEPT("GraphicEngine Creation failed. Invalid input in constructor");

	Window_Width = p_RenderData->d3dInitData.Window_Width;
	Window_Height = p_RenderData->d3dInitData.Window_Height;
	if (!Window_Width || !Window_Height)
		throw NORMAL_EXCEPT("GraphicEngine Creation failed. Invalid input in constructor");

	//Init Windows
	if (!InitializeWindows())
		throw NORMAL_EXCEPT("GraphicEngine::InitializeWindows() failed.");

	//Init WindowGlobals and Input Sysytem
	WindowGlobals::Create(Window_Width, Window_Height);
	InputSystem::Create();

	//Init Renderer
	if (!InitializeRenderer(p_RenderData))
		throw NORMAL_EXCEPT("GraphicEngine::InitializeRenderer() failed.");

	
}

GraphicEngine::~GraphicEngine()
{
}

bool GraphicEngine::Create(IApplication* instApp, RenderData* p_RenderData)
{
	if (pGraphicEngine) return false;
	pGraphicEngine = new GraphicEngine(instApp, p_RenderData);
	return true;
}

GraphicEngine* GraphicEngine::Get()
{
	return pGraphicEngine;
}

void GraphicEngine::onSize()
{
	pRenderer->Resize();
}

void GraphicEngine::onFocus()
{
}

void GraphicEngine::onKillFocus()
{
}

bool GraphicEngine::InitializeWindows()
{
	Window::createWindow(Window_Width, Window_Height);
	pWindow = Window::get();
	if (!pWindow) return false;

	return true;
}

bool GraphicEngine::InitializeRenderer(RenderData* p_RenderData)
{
	switch (p_RenderData->graphi_API)
	{
	case RenderData::GRAPHIC_API::dx11:
	{
		if (!RendererDX11::Create(p_RenderData)) return false;
		pRenderer = RendererDX11::Get();
		break;
	}
	case RenderData::GRAPHIC_API::dx12:
	{
		//
		break;
	}
	case RenderData::GRAPHIC_API::vulkan:
	{
		//
		break;
	}
	}

	if (!pRenderer) return false;

	return true;
}

void GraphicEngine::Run()
{
	while (true)
	{

	}

/*
	for(mainloop)
	{
		while(drawflag == false) {resize()};

		Inputsystemupdate()
		msgpump()
		usercallback -> onBeginFrame()
		
		BeginFrame->	
			for(auto scene : scenemanager)
			{
				fill renderdataprebind from scenedata 
				renderer-setprebinds

				usercallback -> onrender() //not needed

				for(auto entity : entitycontainer of current scene)
				{
					fill renderdatabinding from entity
					renderer - bind to pipeline
				}
			}

			EndFrame-> present()
			usercallback -> onEndFrame()
	}
*/


}
