



class WindowGlobals
{
private:
	WindowGlobals();
	~WindowGlobals();

public:
	static void Create();
	static WindowGlobals* Get();

public:
	const unsigned int& Get_WindowWidth() const;
	const unsigned int& Get_WindowHeight() const;

private:
	static WindowGlobals* pWindowGlobals;

private:
	unsigned int WindowWidth = 0;
	unsigned int WindowHeight = 0;

};