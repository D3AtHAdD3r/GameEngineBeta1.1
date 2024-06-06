#include "WindowGlobals.h"
#include<GraphicEngine/Utilities/Headers/Headers.h>



WindowGlobals* WindowGlobals::pWindowGlobals = nullptr;


WindowGlobals::WindowGlobals()
{
}

WindowGlobals::~WindowGlobals()
{
}

void WindowGlobals::Create()
{
    if (pWindowGlobals)
        throw NORMAL_EXCEPT("pWindowGlobals already exist");

    pWindowGlobals = new WindowGlobals();
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
