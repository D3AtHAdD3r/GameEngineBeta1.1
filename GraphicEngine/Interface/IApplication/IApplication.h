#pragma once
#include<unordered_map>
#include<GraphicEngine/Utilities/Headers/GlobalStructs.h>

class Entity;
class Scene;
class Renderer;
struct Scene_descriptor;
struct EntityDesc;

#ifdef PROJ_EXPORT
#define ATT_Interface __declspec(dllexport)
#else
#define ATT_Interface __declspec(dllimport)
#endif


class ATT_Interface IApplication
{
	friend class GraphicEngine;
protected:
	//IApplication();
	virtual ~IApplication() = default;

public:
	//engine to user
	virtual void onInit() = 0;
	virtual void onBeginFrame() = 0;
	virtual void onEndFrame() = 0;
	virtual void onShutdown() = 0;


public:
	//Input Handling Forwarded through engine to user
	virtual void onKeyDown(int key) = 0;
	virtual void onKeyUp(int key) = 0;
	virtual void onMouseMove(const Point& mouse_pos) = 0;
	virtual void onLeftMouseDown(const Point& mouse_pos) = 0;
	virtual void onLeftMouseUp(const Point& mouse_pos) = 0;
	virtual void onRightMouseDown(const Point& mouse_pos) = 0;
	virtual void onRightMouseUp(const Point& mouse_pos) = 0;


public:
	//user to engine
	bool CreateSceneAndEntity(std::vector<Scene_descriptor*> sd_list, std::vector<EntityDesc*> ed_list);
	Entity* CreateEntity(EntityDesc* pED, bool check_ent_desc = true);
	bool DeleteEntity(Entity* pEnt, Scene* pScene = nullptr);
	const std::unordered_map<unsigned short, Scene*>& GetSceneContainer();
	float Get_AspectRatio(); //temporary

private:
	void Init_IApp();

private:
	Renderer* pRenderer = nullptr;
};