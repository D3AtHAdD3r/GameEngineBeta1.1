#include<Windows.h>
#include<vector>


class WindowListener;

class Window
{
	friend class GraphicEngine;
private:
	Window();
	~Window();

private:
	static void createWindow(const unsigned int& window_width, const unsigned int& window_height);
	bool init(const unsigned int& window_width, const unsigned int& window_height);

public:
	static Window* get();
	RECT getClientWindowRect();
	RECT getClientWindowRect_mousepos(); //change more appropriate name

public:
	const unsigned int& getWindowWidth() const;
	const unsigned int& getWindowHeight() const;
	const bool& get_first_time_case_size() const;
	const bool& isAlive() const;

public:
	void addListner(WindowListener* listener);
	void removeListner(WindowListener* listener);
	std::vector<WindowListener*> getListners();

public:
	void setHWND(HWND hwnd);
	void setWindowSize();
	void setWindowStatus(bool status);
	HWND getHwnd();
	void set_first_time_case_size(bool flag);
	bool release();


private:
	static Window* pWindow;

private:
	HWND hwnd = nullptr;
	bool m_is_run = false;

private:
	unsigned int windowHeight = 0;
	unsigned int windowWidth = 0;

private:
	std::vector<WindowListener*> windowListeners;
	bool first_time_case_size = true;

};

