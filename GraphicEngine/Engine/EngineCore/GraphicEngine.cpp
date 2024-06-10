#include "GraphicEngine.h"
#include<GraphicEngine/Window/Window.h>
#include<GraphicEngine/Utilities/Headers/Headers.h>
#include<GraphicEngine/Renderer/RendererHeaders/RendererStructs.h>
#include<GraphicEngine/D3D11/D3D11Headers/D3D11Structs.h>
#include<GraphicEngine/Window/WindowGlobals.h>
#include<GraphicEngine/InputHandling/InputSystem.h>
#include<GraphicEngine/Renderer/RendererDX11/RendererDX11.h>
#include<GraphicEngine/Interface/IApplication.h>


GraphicEngine* GraphicEngine::pGraphicEngine = nullptr;


GraphicEngine::GraphicEngine(IApplication* inst_App, RenderData* p_RenderData)
	:
	iApp(inst_App)
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

	//pWindow->addListner(this);
	Window::get()->addListner(this);
	InputSystem::get()->addListener(this);
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

bool GraphicEngine::MessagePump()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Sleep(1);
	return true;
}

void GraphicEngine::Run()
{
	iApp->onInit();

	while (true)
	{
		InputSystem::get()->update();
		MessagePump();
		
		iApp->onBeginFrame();

		while (!RenderFlag)
		{
			RenderFlag = true;
			pRenderer->OnResize();
		}
			
		pRenderer->DrawFrame();

		iApp->onEndFrame();
	}

	iApp->onShutdown();
}


void GraphicEngine::onSize()
{
	RenderFlag = false;
}

void GraphicEngine::onFocus()
{
}

void GraphicEngine::onKillFocus()
{
}

void GraphicEngine::onKeyDown(int key)
{
}

void GraphicEngine::onKeyUp(int key)
{
}

void GraphicEngine::onMouseMove(const Point& mouse_pos)
{
}

void GraphicEngine::onLeftMouseDown(const Point& mouse_pos)
{
}

void GraphicEngine::onLeftMouseUp(const Point& mouse_pos)
{
}

void GraphicEngine::onRightMouseDown(const Point& mouse_pos)
{
}

void GraphicEngine::onRightMouseUp(const Point& mouse_pos)
{
}
