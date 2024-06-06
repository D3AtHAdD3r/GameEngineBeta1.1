#pragma once
#include<GraphicEngine/Utilities/Math/Matrix4x4.h>





class Camera
{
private:
	Camera();
	~Camera();

public:
	 Matrix4x4 getWorldMatrix();
	 bool isturboMode();
};