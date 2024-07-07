#pragma once
#include<GraphicEngine/InputHandling/InputListener.h>
#include<unordered_set>




class InputSystem
{
private:
	InputSystem();
	~InputSystem();

public:
	static void Create();
	static InputSystem* get();
public:
	void update();
	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);

	//Helper, utility funcs
	void setCursorPosition(const Point& pos);
	void showCursor(bool show);
private:
	static InputSystem* pInputSystem;
private:
	std::unordered_set<InputListener*> m_set_listeners;
	std::unordered_set<InputListener*> m_set_listenersPaused;
	short m_keys_state[256] = {};
	short m_old_keys_state[256] = {};
	Point m_old_mouse_pos;  //unused
	bool m_first_time = true;
};

