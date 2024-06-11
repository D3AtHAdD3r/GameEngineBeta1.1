#pragma once
#include<Windows.h>



struct D3D11_INIT_DATA
{
	unsigned int Window_Width = 0;
	unsigned int Window_Height = 0;
	HWND hWnd = NULL;
	unsigned int BufferCount = 1;
	bool Windowed = true;
};