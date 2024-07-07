//#include<GraphicEngine/Renderer/RendererHeaders/RendererStructs.h>
#include<Windows.h>
#include<d3d11.h>
#include<unordered_map>

class D3D11Core;
class Texture;
class ECSCore;
struct Scene_descriptor;
struct EntityDesc;
class Entity;
class Camera;
class Scene;
struct RenderData;
struct Renderer_PreBindData;
struct Renderer_BindingData;
class ECSToRendererData;


class RendererDX11
{
	friend class GraphicEngine;
private:
	RendererDX11(); 
	~RendererDX11();
public:
	bool DrawFrame();
	bool OnResize(unsigned int width, unsigned int height);
	void OnShutDown();

public:
	bool CreateSceneAndEntity(std::vector<Scene_descriptor*> sd_list, std::vector<EntityDesc*> ed_list);
	Entity* CreateEntity(EntityDesc* pED, bool check_ent_desc = true);
	bool DeleteEntity(Entity* pEnt, Scene* pScene = nullptr);
	const std::unordered_map<unsigned short, Scene*>& GetSceneContainer();

public:
	static bool Create(RenderData* pRenderData);
	static RendererDX11* Get();

public:
	unsigned int Get_WindowWidth();
	unsigned int Get_WindowHeight();
	HWND Get_WindowHandle();
private:
	bool Initialize(RenderData* pRenderData);
private:
	bool Init_Pre_Bind(Renderer_PreBindData* pData); 
	bool Init_Main_Bind(Renderer_BindingData* pData);
	void PresentFrame(bool Vsync = false);

private:
	bool CheckRenderData(RenderData* pRenderData); 
	bool CheckPreBindData(Renderer_PreBindData* pData);
	bool CheckMainBindData(Renderer_BindingData* pData);
	void Check_File_Exists(RenderData* p_RenderData);

private:
	bool SetPreBinds(Renderer_PreBindData* pData);
	bool BindToPipeLine(Renderer_BindingData* pData);
	
private:
	//setter functions
	void setViewport(UINT width, UINT height);
	void clearRenderTargetView(ID3D11RenderTargetView* prtv, float clear_color[4]);
	void clearDepthStencilView(ID3D11DepthStencilView* pDsv);
	void setRenderTargetView(ID3D11RenderTargetView* prtv, ID3D11DepthStencilView* pDsv);
	void setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology);
	void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
	void Present(bool vsync);

private:
	//binder functions
	void setShaders(ID3D11VertexShader* vShader, ID3D11PixelShader* pShader);
	void setVertexShader(ID3D11VertexShader* vShader);
	void setPixelShader(ID3D11PixelShader* pShader);
	void setVertexBufferandLayout(UINT size_vertex, ID3D11Buffer* vBuffer, ID3D11InputLayout* iLayout);
	void setIndexBuffer(ID3D11Buffer* indexbuffer);
	void setConstantBuffer(ID3D11Buffer* cbuffer);

private:
	void Set_TextureResources_VertexShader(int startslot, int numViews, ID3D11ShaderResourceView* list_res[128], ID3D11SamplerState* list_sampler[128]);
	void Set_TextureResources_PixelShader(int startslot, int numViews, ID3D11ShaderResourceView* list_res[128], ID3D11SamplerState* list_sampler[128]);
	void Set_TextureResources_VertexShader(std::unordered_map<int, Texture*>& list_textures, int start_slot);
	void Set_TextureResources_PixelShader(std::unordered_map<int, Texture*>& list_textures, int start_slot);
	void Set_TextureResources_Vertex_Pixel_Shader(std::unordered_map<int, Texture*>& list_textures, int start_slot);
	void Set_TextureResources_Vertex_Pixel_Shader(int startslot, int numViews, ID3D11ShaderResourceView* pSRV, ID3D11SamplerState* pSampler);

private:
	void initRasterizerState();
	void setRasterizerState(bool cull_front);
	void SetFullScreenMode(bool fullscreen);
private:
	void UpdateConstantBuffer(Entity* currEntity, Camera* pcam);
private:
	Renderer_PreBindData* pRenderer_PreBindData = nullptr;
	Renderer_BindingData* pRenderer_BindingData = nullptr;
private:
	D3D11Core* pD3D11Core = nullptr;
	ECSCore* pECSCore = nullptr;
	
private:
	ID3D11RasterizerState* cull_front_state = nullptr;
	ID3D11RasterizerState* cull_back_state = nullptr;
private:
	ID3D11Buffer* binded_cb = nullptr;
	ID3D11PixelShader* binded_ps = nullptr;
	ID3D11VertexShader* binded_vs = nullptr;
private:
	static RendererDX11* pRenderer;
private:
	HWND pWnd = nullptr;
	unsigned int WindowHeight = 0;
	unsigned int WindowWidth = 0;
};