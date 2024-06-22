#include "GraphicEngine.h"
#include<GraphicEngine/Window/Window.h>
#include<GraphicEngine/Utilities/Headers/Headers.h>
#include<GraphicEngine/Renderer/RendererHeaders/RendererStructs.h>
#include<GraphicEngine/D3D11/D3D11Headers/D3D11Structs.h>
#include<GraphicEngine/Window/WindowGlobals.h>
#include<GraphicEngine/InputHandling/InputSystem.h>
#include<GraphicEngine/Renderer/RendererDX11/RendererDX11.h>
#include<GraphicEngine/Interface/IApplication/IApplication.h>
#include<GraphicEngine/Utilities/UtilitiyFuncs/utilityFunctions.h>
#include"sstream"

GraphicEngine* GraphicEngine::pGraphicEngine = nullptr;
//IApplication* GraphicEngine::iApp = nullptr;

GraphicEngine::GraphicEngine(IApplication* inst_App, RenderData* p_RenderData)
	:
	iApp(inst_App)
{
	if (!inst_App || !p_RenderData)
		throw NORMAL_EXCEPT("GraphicEngine Creation failed. Invalid input in constructor");

	//Check Concrete File Exist
	Check_File_Exists(p_RenderData);

	Window_Width = p_RenderData->d3dInitData.Window_Width;
	Window_Height = p_RenderData->d3dInitData.Window_Height;
	if (!Window_Width || !Window_Height)
		throw NORMAL_EXCEPT("GraphicEngine Creation failed. Invalid input in constructor");


	//Init Window Globals
	WindowGlobals::Create();

	//Init Windows
	if (!InitializeWindows())
		throw NORMAL_EXCEPT("GraphicEngine::InitializeWindows() failed.");

	p_RenderData->d3dInitData.hWnd = pWindow->getHwnd();

	//Init Input Sysytem
	InputSystem::Create();


	//Init Renderer
	if (!InitializeRenderer(p_RenderData))
		throw NORMAL_EXCEPT("GraphicEngine::InitializeRenderer() failed.");

	inst_App->Init_IApp();

	//pWindow->addListner(this);
	Window::get()->addListner(this);
	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false); 
}

GraphicEngine::~GraphicEngine()
{
}

bool GraphicEngine::Create(IApplication* instApp, RenderData* p_RenderData)
{
	try
	{
		if (pGraphicEngine) return false;
		pGraphicEngine = new GraphicEngine(instApp, p_RenderData);
		pGraphicEngine->Run();
		return true;
	}
	catch (const CustomException& e)
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION | MB_TOPMOST);
		return false; 
	}
	
}

GraphicEngine* GraphicEngine::Get()
{
	return pGraphicEngine;
}

Renderer* GraphicEngine::GetRenderer() const
{
	return pRenderer;
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
		WindowGlobals::Get()->pRenderer = pRenderer = RendererDX11::Get();
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

void GraphicEngine::Check_File_Exists(RenderData* p_RenderData)
{
	FILEMAPS& fMaps = p_RenderData->file_maps;

	std::unordered_map<int, std::wstring>& File_Map_VertexShader = fMaps.File_Map_VertexShader;
	std::unordered_map<int, std::wstring>& File_Map_PixelShader = fMaps.File_Map_PixelShader;
	std::unordered_map<int, std::wstring>& File_Map_Textures = fMaps.File_Map_Textures;
	std::unordered_map<int, std::wstring>& File_Map_Meshes = fMaps.File_Map_Meshes;

	for (auto& [uid, FileName] : File_Map_VertexShader)
	{
		if (!UtilityFuncs::Check_File_Exist(FileName))
		{
			std::ostringstream oss;
			oss << "File Not Found : " << UtilityFuncs::unicodeToMultibyte(FileName) << std::endl;
			throw NORMAL_EXCEPT(oss.str());
		}
	}

	for (auto& [uid, FileName] : File_Map_PixelShader)
	{
		if (!UtilityFuncs::Check_File_Exist(FileName))
		{
			std::ostringstream oss;
			oss << "File Not Found : " << UtilityFuncs::unicodeToMultibyte(FileName) << std::endl;
			throw NORMAL_EXCEPT(oss.str());
		}
	}

	for (auto& [uid, FileName] : File_Map_Textures)
	{
		//Check if RTV or BackBuffer texture
		std::wstring firstThree = FileName.substr(0, 3);
		if (firstThree == HARDCODINGS::RTV_NAME_START || firstThree == HARDCODINGS::BackBuffer_NAME_START)
			continue;

		if (!UtilityFuncs::Check_File_Exist(FileName))
		{
			std::ostringstream oss;
			oss << "File Not Found : " << UtilityFuncs::unicodeToMultibyte(FileName) << std::endl;
			throw NORMAL_EXCEPT(oss.str());
		}
	}

	for (auto& [uid, FileName] : File_Map_Meshes)
	{
		if (!UtilityFuncs::Check_File_Exist(FileName))
		{
			std::ostringstream oss;
			oss << "File Not Found : " << UtilityFuncs::unicodeToMultibyte(FileName) << std::endl;
			throw NORMAL_EXCEPT(oss.str());
		}
	}


}

void GraphicEngine::Run()
{
	iApp->onInit();

	while (true)
	{
		if (!pWindow->isAlive()) break;

		InputSystem::get()->update();
		MessagePump();

		iApp->onBeginFrame();

		while (!RenderFlag)
		{
			RenderFlag = true;
			pRenderer->OnResize();
		}
			
		if (!pRenderer->DrawFrame())
		{
			throw NORMAL_EXCEPT("pRenderer->DrawFrame() failed");
		}

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
	iApp->onKeyDown(key);
}

void GraphicEngine::onKeyUp(int key)
{
	if (key == VK_ESCAPE)
	{
		if (!Set_Play_State())
			throw NORMAL_EXCEPT("GraphicEngine::onKeyUp - Set_Play_State Failed");
	}

	iApp->onKeyUp(key);
}

void GraphicEngine::onMouseMove(const Point& mouse_pos)
{
	iApp->onMouseMove(mouse_pos);
	if (play_state)
		Fix_Cursor();
}

void GraphicEngine::onLeftMouseDown(const Point& mouse_pos)
{
	iApp->onLeftMouseDown(mouse_pos);
}

void GraphicEngine::onLeftMouseUp(const Point& mouse_pos)
{
	iApp->onLeftMouseUp(mouse_pos);
}

void GraphicEngine::onRightMouseDown(const Point& mouse_pos)
{
	iApp->onRightMouseDown(mouse_pos);
}

void GraphicEngine::onRightMouseUp(const Point& mouse_pos)
{
	iApp->onRightMouseUp(mouse_pos);
}

bool GraphicEngine::Set_Play_State()
{
	play_state = !play_state;

	if (!play_state)
	{
		InputSystem::get()->showCursor(true);
	}
	else
	{
		InputSystem::get()->showCursor(false);
	}

	return true;
}

bool GraphicEngine::Fix_Cursor()
{
	//fix cursor in the middle of client window
	int windowWidth = WindowGlobals::Get()->Get_WindowWidth();
	int windowHeight = WindowGlobals::Get()->Get_WindowHeight();
	Point pt = { windowWidth / 2, windowHeight / 2 };
	ClientToScreen(Window::get()->getHwnd(), (LPPOINT)&pt);
	InputSystem::get()->setCursorPosition(Point(pt.m_x, pt.m_y));

	return true;
}
