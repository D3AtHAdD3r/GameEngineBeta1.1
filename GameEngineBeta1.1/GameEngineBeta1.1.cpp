#include <iostream>
#include<GameEngineBeta1.1/TestGame/TestGame.h>
#include<GraphicEngine/Renderer/RendererHeaders/RendererStructs.h>
#include<GraphicEngine/Engine/EngineCore/GraphicEngine.h>



FILEMAPS GetFileMaps()
{
    std::unordered_map<int, std::wstring> File_Map_VertexShader;
    File_Map_VertexShader.emplace(0, L"..\\Shaders\\vertexShader.hlsl");

    std::unordered_map<int, std::wstring> File_Map_PixelShader;
    File_Map_PixelShader.emplace(0, L"..\\Shaders\\pixelShader_DL.hlsl");

    std::unordered_map<int, std::wstring> File_Map_Textures;
    File_Map_Textures.emplace(0, L"BackBuffer");
    File_Map_Textures.emplace(1, L"..\\AssetsAndLibs\\Assets\\Textures\\asteroid.jpg");

    std::unordered_map<int, std::wstring> File_Map_Meshes;
    File_Map_Meshes.emplace(0, L"..\\AssetsAndLibs\\Assets\\Meshes\\asteroid.obj");

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
    rData.d3dInitData.Window_Width = 1024;
    rData.d3dInitData.Window_Height = 768;

    rData.graphi_API = RenderData::GRAPHIC_API::dx11;
    rData.file_maps = GetFileMaps();

    TestGame tGame;
    GraphicEngine::Create(&tGame, &rData);

    return 0;
}

