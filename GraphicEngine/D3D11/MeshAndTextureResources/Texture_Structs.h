#pragma once
#include<string>




enum TEXTURE_TYPE
{
	Concrete_Texture = 0,
	FrameBuffer = 1,
	BackBuffer = 2,
	Unknown_Texture
};


struct Texture_Creation_Details
{
	const int& u_ID = 1;
	std::wstring tex_name;
	unsigned int width = 0;
	unsigned int height = 0;
	short buffer_index = -1;
	TEXTURE_TYPE tex_type = TEXTURE_TYPE::Unknown_Texture;
};
