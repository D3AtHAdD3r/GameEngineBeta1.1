#include<GraphicEngine/Utilities/Headers/HeadersD3D.h>
#include<string>
#include<unordered_map>


class Texture;
struct Texture_Creation_Details;

class TextureManager
{
public:
	TextureManager(const std::unordered_map<int, std::wstring>& File_Map);
	~TextureManager();

public:
	Texture* CreateTexture(Texture_Creation_Details* tex_creation_data);

private:
	Texture* CreateTextureConcrete(const int& u_ID);
	Texture* CreateTextureFromResourceViews(std::wstring tex_name, const unsigned int& width, const unsigned int& height, const int& u_ID);
	Texture* CreateTextureFromBackBuffer(std::wstring tex_name, const short& buffer_index, const int& u_ID);

public:
	bool UpdateTextureOnResize(const int& u_ID, const unsigned int& width, const unsigned int& height);
	const std::unordered_map<int, Texture*>& GetTextureList_Concrete();
	const std::unordered_map<int, Texture*>& GetTextureList_FrameBuffer();
	const std::unordered_map<int, Texture*>& GetTextureList_BackBuffer();
	void releaseAll();

public:
	bool check_Exist(const int& u_id);
	bool freeTexture(Texture* pTex);
	bool deleteTexture(Texture* pTex);
	bool deleteTexture(const int& u_id);

public:
	Texture* getTexturebyName(std::wstring tex_name);
	Texture* getTexturebyUID(const int& u_id);

private:
	void deleteTexture_Unsafe(Texture* pTex);

private:
	bool LoadTextureFromFile(const wchar_t* full_path, ID3D11Resource** pTexture, ID3D11ShaderResourceView** pShader_res_view, ID3D11SamplerState** pSampler_State); // outdated
	bool LoadTextureFromFile(const wchar_t* full_path, Texture* pTexture); //use this

private:
	bool CreateRenderTargetView(ID3D11Texture2D** p_texture, ID3D11RenderTargetView** p_RenderTargetView, const unsigned int& width, const unsigned int& height);
	bool CreatedepthStensilView(ID3D11DepthStencilView** p_DepthStencilView, const unsigned int& width, const unsigned int& height);
	bool CreateShaderResourceViewFromTexture(ID3D11Texture2D* ptexture, ID3D11ShaderResourceView** pShader_res_view);
	bool CreateSamplerState(ID3D11SamplerState** m_sampler_state);
	bool CreateRenderTargetView_BackBuffer(ID3D11RenderTargetView** pRenderTargetView, const unsigned short& buffer_index);

private:
	std::wstring GetFileName(const int& uID);

private:
	std::unordered_map<int, Texture*> TextureContainer_Concrete;
	std::unordered_map<int, Texture*> TextureContainer_FrameBuffer;
	std::unordered_map<int, Texture*> TextureContainer_BackBuffer;

private:
	std::unordered_map<int, std::wstring> FileMap;
};
