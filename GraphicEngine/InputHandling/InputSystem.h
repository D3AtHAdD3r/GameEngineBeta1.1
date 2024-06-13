#pragma once
#include<GraphicEngine/InputHandling/InputListener.h>
#include<unordered_set>




class InputSystem
{
	friend class GraphicEngine;
private:
	InputSystem();
	~InputSystem();

private:
	static void Create();

public:
	void update();
	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);

	//Helper, utility funcs
	void setCursorPosition(const Point& pos);
	void showCursor(bool show);

public:
	static InputSystem* get();

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

