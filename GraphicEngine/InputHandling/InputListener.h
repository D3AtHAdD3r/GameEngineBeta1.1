#pragma once
#include<GraphicEngine/Utilities/Headers/GlobalStructs.h>


#ifdef PROJ_EXPORT
#define ATT_InputListener __declspec(dllexport)
#else
#define ATT_InputListener __declspec(dllimport)
#endif

class ATT_InputListener InputListener
{
public:
	InputListener()
	{
	}
	~InputListener()
	{
	}

	//KEYBOARD pure virtual callback functions 
	virtual void onKeyDown(int key) = 0;
	virtual void onKeyUp(int key) = 0;

	//MOUSE pure virtual callback functions
	virtual void onMouseMove(const Point& mouse_pos) = 0;

	virtual void onLeftMouseDown(const Point& mouse_pos) = 0;
	virtual void onLeftMouseUp(const Point& mouse_pos) = 0;

	virtual void onRightMouseDown(const Point& mouse_pos) = 0;
	virtual void onRightMouseUp(const Point& mouse_pos) = 0;
};