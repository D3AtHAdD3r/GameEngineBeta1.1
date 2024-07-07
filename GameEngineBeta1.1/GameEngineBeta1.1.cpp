#include <iostream>
#include<GameEngineBeta1.1/TestGame/TestGame.h>
#include<GameEngineBeta1.1\RendererDx11Structs.h>
#include<GraphicEngine\Interface\IRenderer\IRenderer.h>
#include<Windows.h>
#include"Window.h"


FILEMAPS GetFileMaps()
{
    std::unordered_map<int, std::wstring> File_Map_VertexShader;
    File_Map_VertexShader.emplace(0, L"..\\Shaders\\vertexShader.hlsl");
    
    std::unordered_map<int, std::wstring> File_Map_PixelShader;
    File_Map_PixelShader.emplace(0, L"..\\Shaders\\pixelShader_DL.hlsl");  //pShader_stars_map.hlsl  pixelShader_No_Tex.hlsl
   
    std::unordered_map<int, std::wstring> File_Map_Meshes;
    File_Map_Meshes.emplace(0, L"..\\AssetsAndLibs\\Assets\\Meshes\\asteroid.obj");
   
    std::unordered_map<int, std::wstring> File_Map_Textures;
    File_Map_Textures.emplace(0, L"BBFBackBuffer");
    File_Map_Textures.emplace(1, L"..\\AssetsAndLibs\\Assets\\Textures\\asteroid.jpg");
   

    FILEMAPS fmaps;
    fmaps.File_Map_Meshes = File_Map_Meshes;
    fmaps.File_Map_PixelShader = File_Map_PixelShader;
    fmaps.File_Map_Textures = File_Map_Textures;
    fmaps.File_Map_VertexShader = File_Map_VertexShader;

    return fmaps;
}



int main()
{
    Window::createWindow(1366, 768);

    RenderData rData;
    rData.d3dInitData.BufferCount = 1;
    rData.d3dInitData.Windowed = true;
    rData.d3dInitData.Window_Width = rData.WindowWidth = 1366;
    rData.d3dInitData.Window_Height = rData.WindowHeight = 768;

    rData.graphi_API = RenderData::GRAPHIC_API::dx11;
    rData.file_maps = GetFileMaps();

    rData.pHandle = rData.d3dInitData.hWnd = Window::get()->getHwnd();
    

    if (!CreateRenderer(&rData))
        printf("Renderer Creation failed");


    //Create Scene and entity
    constant* cb = new constant;
    std::vector<Scene_descriptor*> sd_list;
    std::vector<EntityDesc*> ed_list;

    static D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        //SEMANTIC NAME - SEMANTIC INDEX - FORMAT - INPUT SLOT - ALIGNED BYTE OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"TEXCOORD", 0,  DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA ,0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    Scene_descriptor sd;
    sd.scene_name = L"Scene1";
    sd.scene_id = 0;
    sd.scene_texture_uid = 0;
    sd.connect_backbuffer = true;
    sd.BackBuffer_Index = 0;
    sd.window_client_width = 0;
    sd.window_client_height = 0;
    sd.width_ratio = 1;
    sd.height_ratio = 1;
    sd.clearRenderTargetView = true;
    sd.useDepthStencil = true;
    sd.clearDepthStencil = true;

    float fov = 0.785398f;
    float zNear = 0.1f;
    float zFar = 10000.0f;
    float aspectRatio = (float)rData.WindowWidth / (float)rData.WindowHeight;

    CameraInitData cd;
    cd.GetInput = false;
    cd.isProjecting = true;
    cd.uID = 0;
    cd.fov = fov;
    cd.zNear = zNear;
    cd.zFar = zFar;
    cd.aspect_ratio = aspectRatio;
    cd.SmoothRotation = true;
    cd.SmoothTranslation = true;
    cd.world_pos = { 0, 0,-100 };

    sd.camData.push_back(&cd);

    sd_list.push_back(&sd);

    //Asteroid
    EntityDesc ed;
    ed.mesh_uid = 0;
    ed.getMeshfromFile = true;

    ed.Texture_Concrete_uIDs.push_back({ Entity_Texture_Type::Tex_Material, 1 });
    //ed.Texture_Concrete_uIDs.push_back({ Entity_Texture_Type::Tex_Custom, 2 });
    ed.Entity_type = ENTITY_TYPE::ENUM_NORMAL_ENTITY;
    ed.model_initialPosition = { 0,0,0.0f };
    ed.model_initialScaling = { 7.0f,7.0f,7.0f };

    ed.vertex_Shader_uid = 0;
    ed.inLayout = layout;
    ed.sizeLayout = ARRAYSIZE(layout);
    ed.pixel_Shader_uid = 0;

    ed.constant_buffer = cb;
    ed.size_constant_buffer = sizeof(constant);
    ed.constant_buffer_uid = 0;

    ed.primitive_name = L"Asteroid";
    ed.primitive_uid = 0;
    ed.Scene_Id = 0;
    ed_list.push_back(&ed);

    if (!CreateSceneAndEntity(sd_list, ed_list))
        printf("CreateSceneAndEntity failed");

    while (true)
    {
        DrawFrame();
        Sleep(10);
    }


    return 0;
}

