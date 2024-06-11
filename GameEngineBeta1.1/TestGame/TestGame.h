#pragma once
#include<GraphicEngine/Interface/IApplication/IApplication.h>



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


	//custom stuff
public:

};