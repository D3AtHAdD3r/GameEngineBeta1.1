#include "Window.h"
#include"GraphicEngine/Utilities/Headers/Headers.h"
#include<GraphicEngine/Window/WindowListener.h>
#include<GraphicEngine/Window/WindowGlobals.h>

Window* Window::pWindow = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        // Event fired when the window is created
        // collected here..
        Window* window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
        // .. and then stored for later lookup
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
        window->setHWND(hwnd);
        break;
    }
    case WM_SETFOCUS:
    {
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (!window->getListners().empty())
        {
            for (auto wlistner : window->getListners())
            {
                wlistner->onFocus();
            }
        }
        break;
    }
    case WM_KILLFOCUS:
    {
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (!window->getListners().empty())
        {
            for (auto wlistner : window->getListners())
            {
                wlistner->onKillFocus();
            }
        }
        break;
    }
    case WM_SIZE:
    {
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        window->setWindowSize();
        WindowGlobals::Get()->Set_Window_Coords(window->getWindowWidth(), window->getWindowHeight());

        if (window->get_first_time_case_size())
        {
            window->set_first_time_case_size(false);
            break;
        }

        if (!window->getListners().empty())
        {
            for (auto wlistner : window->getListners())
            {
                wlistner->onSize();
            }
        }
        break;
    }
    case WM_DESTROY:
    {
        // Event fired when the window is destroyed
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        PostQuitMessage(0);
        window->setWindowStatus(false);
        break;
    }
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    return NULL;
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
        throw NORMAL_EXCEPT("Window already exsists..");
    pWindow = new Window();
    if (!pWindow->init(window_width, window_height))
        throw NORMAL_EXCEPT("Window creation failed..");
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

const bool& Window::get_first_time_case_size() const
{
    return first_time_case_size;
}

const bool& Window::isAlive() const
{
    return m_is_run;
}

void Window::addListner(WindowListener* listener)
{
    windowListeners.push_back(listener);
}

void Window::removeListner(WindowListener* listener)
{
    for (int i = 0; i < windowListeners.size(); ++i)
    {
        if (windowListeners[i] == listener)
        {
            windowListeners.erase(windowListeners.begin() + i);
            i--;
        }
    }
}

std::vector<WindowListener*> Window::getListners()
{
    return windowListeners;
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

void Window::set_first_time_case_size(bool flag)
{
    first_time_case_size = flag;
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
