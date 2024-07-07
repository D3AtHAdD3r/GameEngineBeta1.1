#include "Window.h"
#include<exception>

Window* Window::pWindow = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

Window::Window()
{
}

Window::~Window()
{
}

void Window::createWindow(const unsigned int& window_width, const unsigned int& window_height)
{
    if (Window::pWindow)
        throw std::exception("Window already exsists..");
    pWindow = new Window();
    if (!pWindow->init(window_width, window_height))
        throw std::exception("Window creation failed..");
}

bool Window::init(const unsigned int& window_width, const unsigned int& window_height)
{
    //Setting up WNDCLASSEX object
    WNDCLASSEX wc;
    wc.cbClsExtra = NULL;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = NULL;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = NULL;
    wc.lpszClassName = L"MyWindowClass";
    wc.lpszMenuName = L"";
    wc.style = NULL;
    wc.lpfnWndProc = &WndProc;

    if (!RegisterClassEx(&wc)) // If the registration of class will fail, the function will return false
        return false;

    hwnd = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"DirectX Application",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, window_width, window_height,
        NULL, NULL, NULL, this);

    if (!hwnd)
        return false;

    setWindowSize();

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    //set this flag to true to indicate that the window is initialized and running
    m_is_run = true;
    return true;
}

Window* Window::get()
{
    return pWindow;
}

RECT Window::getClientWindowRect()
{
    RECT rc;
    GetClientRect(this->hwnd, &rc);
    return rc;
}

RECT Window::getClientWindowRect_mousepos()
{
    RECT rc;
    GetClientRect(this->hwnd, &rc);
    ClientToScreen(this->hwnd, (LPPOINT)&rc.left);
    ClientToScreen(this->hwnd, (LPPOINT)&rc.right);
    return rc;
}

const unsigned int& Window::getWindowWidth() const
{
    return windowWidth;
}

const unsigned int& Window::getWindowHeight() const
{
    return windowHeight;
}

const bool& Window::isAlive() const
{
    return m_is_run;
}


void Window::setHWND(HWND hwnd)
{
    this->hwnd = hwnd;
}

void Window::setWindowSize()
{
    RECT rc = getClientWindowRect();
    windowWidth = rc.right - rc.left;
    windowHeight = rc.bottom - rc.top;
}

void Window::setWindowStatus(bool status)
{
    m_is_run = status;
}

HWND Window::getHwnd()
{
    return hwnd;
}


bool Window::release()
{
    if (!DestroyWindow(hwnd))
    {
        return false;
    }

    m_is_run = false;
    return true;
}

