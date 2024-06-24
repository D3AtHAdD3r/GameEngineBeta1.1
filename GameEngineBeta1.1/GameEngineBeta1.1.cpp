#include <iostream>
#include<GameEngineBeta1.1/TestGame/TestGame.h>
#include<GraphicEngine/Renderer/RendererHeaders/RendererStructs.h>
#include<GraphicEngine/Engine/EngineCore/GraphicEngine.h>

/*
BB-8.obj
BB-8.jpg

*/

FILEMAPS GetFileMaps()
{
    std::unordered_map<int, std::wstring> File_Map_VertexShader;
    File_Map_VertexShader.emplace(0, L"..\\Shaders\\vertexShader.hlsl");

    std::unordered_map<int, std::wstring> File_Map_PixelShader;
    File_Map_PixelShader.emplace(0, L"..\\Shaders\\pixelShader_DL.hlsl");  //pShader_stars_map.hlsl  pixelShader_No_Tex.hlsl
    File_Map_PixelShader.emplace(1, L"..\\Shaders\\pShader_stars_map.hlsl");
    File_Map_PixelShader.emplace(2, L"..\\Shaders\\pixelShader_No_Tex.hlsl");


    std::unordered_map<int, std::wstring> File_Map_Meshes;
    File_Map_Meshes.emplace(0, L"..\\AssetsAndLibs\\Assets\\Meshes\\asteroid.obj");
    File_Map_Meshes.emplace(1, L"..\\AssetsAndLibs\\Assets\\Meshes\\sphere.obj");
    File_Map_Meshes.emplace(2, L"..\\AssetsAndLibs\\Assets\\Meshes\\dragon.obj");
    File_Map_Meshes.emplace(3, L"..\\AssetsAndLibs\\Assets\\Meshes\\Video_camera.obj");


    std::unordered_map<int, std::wstring> File_Map_Textures;
    File_Map_Textures.emplace(0, L"BBFBackBuffer");
    File_Map_Textures.emplace(1, L"..\\AssetsAndLibs\\Assets\\Textures\\asteroid.jpg");
    File_Map_Textures.emplace(2, L"..\\AssetsAndLibs\\Assets\\Textures\\stars_map.jpg");
    File_Map_Textures.emplace(3, L"..\\AssetsAndLibs\\Assets\\Textures\\dragon.png");
    File_Map_Textures.emplace(4, L"..\\AssetsAndLibs\\Assets\\Textures\\Video_camera.jpg");
    

    FILEMAPS fmaps;
    fmaps.File_Map_Meshes = File_Map_Meshes;
    fmaps.File_Map_PixelShader = File_Map_PixelShader;
    fmaps.File_Map_Textures = File_Map_Textures;
    fmaps.File_Map_VertexShader = File_Map_VertexShader;

    return fmaps;
}



int main()
{
    RenderData rData;
    rData.d3dInitData.BufferCount = 1;
    rData.d3dInitData.Windowed = true;
    rData.d3dInitData.Window_Width = 1366;
    rData.d3dInitData.Window_Height = 768;

    rData.graphi_API = RenderData::GRAPHIC_API::dx11;
    rData.file_maps = GetFileMaps();

    TestGame tGame;
    GraphicEngine::Create(&tGame, &rData);

    return 0;
}

