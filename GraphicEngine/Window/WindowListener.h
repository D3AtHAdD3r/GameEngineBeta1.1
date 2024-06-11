#pragma once


#ifdef PROJ_EXPORT
#define ATT_WindowListener __declspec(dllexport)
#else
#define ATT_WindowListener __declspec(dllimport)
#endif


class ATT_WindowListener WindowListener
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

