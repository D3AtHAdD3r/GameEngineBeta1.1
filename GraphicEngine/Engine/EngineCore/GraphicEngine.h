#include<GraphicEngine/Utilities/Headers/Headers.h>


class RendererDX11;
struct RenderData;
struct Renderer_PreBindData;
struct Renderer_BindingData;


class GraphicEngine
{
public:
	GraphicEngine();
	~GraphicEngine();

private:
	void InitializeWindows();
	void InitializeInputEvents();
	void InitializeRenderer();

	void Run();

private:
	RendererDX11* pRenderer = nullptr;
	RenderData* pRenderData = nullptr;
	Renderer_PreBindData* pRenderer_Prebind_data = nullptr;
	Renderer_BindingData* pRenderer_MainBind_data = nullptr;


private:
	unsigned int Window_Width = 0;
	unsigned int Window_Height = 0;
	HWND hWnd = NULL;
};