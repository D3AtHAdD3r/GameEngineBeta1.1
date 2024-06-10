#pragma once
//#include<GraphicEngine/Utilities/Headers/GlobalStructs.h>

class Point
{
public:
	Point() :m_x(0), m_y(0)
	{
	}
	Point(int x, int y) :m_x(x), m_y(y)
	{
	}
	Point(const Point& point) :m_x(point.m_x), m_y(point.m_y)
	{
	}
	~Point()
	{
	}
public:
	int m_x = 0, m_y = 0;
};


class InputListener
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