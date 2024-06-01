#include<GraphicEngine/Utilities/Headers/HeadersD3D.h>
#include<string>
#include<unordered_map>

class Texture;


class TextureManager
{
public:
	TextureManager();
	~TextureManager();

public:
	Texture* CreateTextureFromFile(std::wstring filePath, const int& u_ID);
	Texture* CreateTextureFromResourceViews(std::wstring tex_name, unsigned int width, unsigned int height, const int& u_ID);
	Texture* CreateTextureFromBackBuffer(std::wstring tex_name, unsigned short buffer_index, const int& u_ID);

public:
	bool UpdateTextureOnResize(const int& u_ID, unsigned int width, unsigned int height, Texture* pTex = nullptr);
	const std::unordered_map<int, Texture*>& getTextureList();
	void releaseAll();

public:
	bool check_Exist(const int& u_id);
	bool freeTexture(Texture* pTex);
	bool deleteTexture(Texture* pTex);
	bool deleteTexture(const int& u_id);

public:
	Texture* getTexturebyName(std::wstring tex_name);
	Texture* getTexturebyAddress(Texture* pTex);
	Texture* getTexturebyUID(const int& u_id);

private:
	void deleteTexture_direct(Texture* pTex);

private:
	bool LoadTextureFromFile(const wchar_t* full_path, ID3D11Resource** pTexture, ID3D11ShaderResourceView** pShader_res_view, ID3D11SamplerState** pSampler_State);

private:
	bool CreateRenderTargetView(ID3D11Texture2D** p_texture, ID3D11RenderTargetView** p_RenderTargetView, const unsigned int& width, const unsigned int& height);
	bool CreatedepthStensilView(ID3D11DepthStencilView** p_DepthStencilView, const unsigned int& width, const unsigned int& height);
	bool CreateShaderResourceViewFromTexture(ID3D11Texture2D* ptexture, ID3D11ShaderResourceView** pShader_res_view);
	bool CreateSamplerState(ID3D11SamplerState** m_sampler_state);
	bool CreateRenderTargetView_BackBuffer(ID3D11RenderTargetView** pRenderTargetView, const unsigned short& buffer_index);
private:
	std::unordered_map<int, Texture*> TextureContainer;
	unsigned int ResourceCount = 0;
};