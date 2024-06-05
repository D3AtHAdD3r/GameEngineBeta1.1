#pragma once
#include<unordered_map>
#include<string>

struct FILEMAPS
{
	std::unordered_map<int, std::wstring> File_Map_VertexShader;
	std::unordered_map<int, std::wstring> File_Map_PixelShader;
	std::unordered_map<int, std::wstring> File_Map_Textures;
	std::unordered_map<int, std::wstring>& File_Map_Meshes;
};