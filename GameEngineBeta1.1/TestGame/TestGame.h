#pragma once
#include<GraphicEngine/Interface/IApplication/IApplication.h>
#include<GraphicEngine/InputHandling/InputListener.h>

struct constant;


class TestGame : public IApplication
{
public:
	TestGame();
	~TestGame();
public:
	//engine to user
	virtual void onInit() override;
	virtual void onBeginFrame() override;
	virtual void onEndFrame() override;
	virtual void onShutdown()override;

	//InputHandling
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;

	//custom stuff
public:
	bool Create_Scene_And_Entity();
	bool Update();
	bool Update_NormalEntity(std::vector<Entity*>& EntityContainer, Scene* currScene);
	bool Update_LocalPlayer(std::vector<Entity*>& EntityContainer, Scene* currScene);
public:
	bool UpdateEntitiesOnInit(std::unordered_map<unsigned short, Scene*>& SceneContainer);

public:
	bool changeCameraFlag = true;

public:
	constant* cb = nullptr;
	bool ShowCursorFlag = false;
public:
	std::unordered_map<unsigned short, Scene*> SceneContainer;

	//custom
public:
	int val_translation = 0;

};