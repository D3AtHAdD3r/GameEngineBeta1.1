#include "WindowGlobals.h"
#include<GraphicEngine/Utilities/Headers/Headers.h>



WindowGlobals* WindowGlobals::pWindowGlobals = nullptr;


WindowGlobals::WindowGlobals()
{
}

WindowGlobals::~WindowGlobals()
{
}

void WindowGlobals::Create(unsigned int wind_width, unsigned int wind_height)
{
    if (pWindowGlobals)
        throw NORMAL_EXCEPT("pWindowGlobals already exist");

    pWindowGlobals = new WindowGlobals();
    pWindowGlobals->WindowWidth = wind_width;
    pWindowGlobals->WindowHeight = wind_height;
}

WindowGlobals* WindowGlobals::Get()
{
    return pWindowGlobals;
}

const unsigned int& WindowGlobals::Get_WindowWidth() const
{
    return WindowWidth;
}

const unsigned int& WindowGlobals::Get_WindowHeight() const
{
    return WindowHeight;
}

void WindowGlobals::Set_Window_Coords(const unsigned int& width, const unsigned int& height)
{
    WindowWidth = width;
    WindowHeight = height;
}
