#include<GraphicEngine/Utilities/Headers/Headers.h>
#include<GraphicEngine/Window/WindowListener.h>
#include<GraphicEngine/InputHandling/InputListener.h>

class Renderer;
struct RenderData;
class IApplication;
class Window;

class GraphicEngine : public InputListener, public WindowListener
{
public:
	GraphicEngine(IApplication* instApp, RenderData* p_RenderData);
	~GraphicEngine();

public:
	static bool Create(IApplication* instApp, RenderData* p_RenderData);
	static GraphicEngine* Get();

private:
	bool InitializeWindows();
	bool InitializeRenderer(RenderData* p_RenderData);
	bool MessagePump();
private:
	void Run();

public:
	//Inherited from window listener
	virtual void onSize() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;

	//Inherited via Input listener
	//KEYBOARD pure virtual callback functions 
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;

	//MOUSE pure virtual callback functions
	virtual void onMouseMove(const Point& mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;

private:
	Renderer* pRenderer = nullptr;
	Window* pWindow = nullptr;
	IApplication* iApp = nullptr;

private:
	unsigned int Window_Width = 0;
	unsigned int Window_Height = 0;
	//HWND hWnd = NULL;

private:
	static GraphicEngine* pGraphicEngine;
	bool RenderFlag = true;
};