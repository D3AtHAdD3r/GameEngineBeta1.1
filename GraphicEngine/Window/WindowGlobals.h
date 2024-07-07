
//
//class Renderer;
//
//class WindowGlobals
//{
//	friend class GraphicEngine;
//private:
//	WindowGlobals();
//	~WindowGlobals();
//
//private:
//	static void Create();
//public:
//	static WindowGlobals* Get();
//
//public:
//	const unsigned int& Get_WindowWidth() const;
//	const unsigned int& Get_WindowHeight() const;
//	Renderer* Get_Renderer() const;
//
//public:
//	void Set_Window_Coords(const unsigned int& width, const unsigned int& height);
//
//private:
//	Renderer* pRenderer = nullptr;
//
//private:
//	static WindowGlobals* pWindowGlobals;
//
//private:
//	unsigned int WindowWidth = 0;
//	unsigned int WindowHeight = 0;
//};