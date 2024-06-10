#include<GraphicEngine/Utilities/Headers/Headers.h>
#include<GraphicEngine/Window/WindowListener.h>

class Renderer;
struct RenderData;
class IApplication;
class Window;

class GraphicEngine : public WindowListener
{
public:
	GraphicEngine(IApplication* instApp, RenderData* p_RenderData);
	~GraphicEngine();

public:
	static bool Create(IApplication* instApp, RenderData* p_RenderData);
	static GraphicEngine* Get();

public:
	//Inherited from window listener
	virtual void onSize() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;
private:
	bool InitializeWindows();
	bool InitializeRenderer(RenderData* p_RenderData);

private:
	void Run();

private:
	Renderer* pRenderer = nullptr;
	Window* pWindow = nullptr;
	IApplication* instApp = nullptr;

private:
	unsigned int Window_Width = 0;
	unsigned int Window_Height = 0;
	//HWND hWnd = NULL;

private:
	static GraphicEngine* pGraphicEngine;
	bool RenderFlag = true;
};