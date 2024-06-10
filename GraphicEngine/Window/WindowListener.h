#pragma once





class WindowListener
{
public:
	WindowListener()
	{
	}

	~WindowListener()
	{
	}

	//pure virtual callback functions
	virtual void onFocus() = 0;
	virtual void onKillFocus() = 0;
	virtual void onSize() = 0;
};

