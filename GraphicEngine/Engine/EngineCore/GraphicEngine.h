#pragma once
#include<GraphicEngine/Window/WindowListener.h>
#include<GraphicEngine/InputHandling/InputListener.h>

class Renderer;
struct RenderData;
class IApplication;
class Window;

#ifdef PROJ_EXPORT
#define ATT_GraphicEngine __declspec(dllexport)
#else
#define ATT_GraphicEngine __declspec(dllimport)
#endif

class ATT_GraphicEngine GraphicEngine : public InputListener, public WindowListener
{
private:
	GraphicEngine(IApplication* instApp, RenderData* p_RenderData);
	~GraphicEngine();

public:
	static bool Create(IApplication* instApp, RenderData* p_RenderData);
	static GraphicEngine* Get();

private:
	Renderer* GetRenderer() const;
	void Run();

private:
	bool InitializeWindows();
	bool InitializeRenderer(RenderData* p_RenderData);
	bool MessagePump();

private:
	void Check_File_Exists(RenderData* p_RenderData);

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
	bool Set_Play_State();	//show hide cursor, fix cursor in the middle of screen - Toggle
	bool Fix_Cursor();		//fix cursor in the middle of client window

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
private:
	bool play_state = true;
};