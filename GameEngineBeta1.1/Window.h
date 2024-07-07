#include<Windows.h>
#include<vector>




class Window
{
private:
	Window();
	~Window();

private:
	bool init(const unsigned int& window_width, const unsigned int& window_height);

public:
	static void createWindow(const unsigned int& window_width, const unsigned int& window_height);
	static Window* get();
	RECT getClientWindowRect();
	RECT getClientWindowRect_mousepos(); //change more appropriate name

public:
	const unsigned int& getWindowWidth() const;
	const unsigned int& getWindowHeight() const;
	const bool& get_first_time_case_size() const;
	const bool& isAlive() const;

public:
	void setHWND(HWND hwnd);
	void setWindowSize();
	void setWindowStatus(bool status);
	HWND getHwnd();
	bool release();

private:
	static Window* pWindow;

private:
	HWND hwnd = nullptr;
	bool m_is_run = false;

private:
	unsigned int windowHeight = 0;
	unsigned int windowWidth = 0;
};

