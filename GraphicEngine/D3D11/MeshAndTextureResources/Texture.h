#include<GraphicEngine/Utilities/Headers/HeadersD3D.h>
#include<string>
//#include<Windows.h>

enum TextureType
{
	NORMAL_TEXTURE = 0,		//concrete file
	RTV_TEXTURE,			//framebuffer
	BACK_BUFFER_TEXTURE		//backbuffer
};


class Texture
{
private:
	Texture(std::wstring texture_name, TextureType texture_Type, const int& u_ID);
	~Texture();

public:
	const std::wstring& GetTextureName();
	const int& getTextureID();
	const TextureType& getTextureType();
public:
	void incrementCounter();
	void decrementCounter();
	void ReleaseAll();

public:
	ID3D11Resource** GetTextureResourcePP();
	ID3D11ShaderResourceView** GetShaderResourceViewPP();
	ID3D11SamplerState** GetSamplerStatePP();
public:
	ID3D11Resource* GetTextureResource();
	ID3D11RenderTargetView* GetRenderTargetView();
	ID3D11DepthStencilView* GetDepthStencilView();
	ID3D11ShaderResourceView* GetShaderResourceView();
	RECT Get_Size() const;
private:
	ID3D11Resource* pTexture = nullptr;
	ID3D11RenderTargetView* pRTV = nullptr;
	ID3D11DepthStencilView* pDSV = nullptr;
	ID3D11ShaderResourceView* pShader_res_view = nullptr;
	ID3D11SamplerState* pSamplerState = nullptr;

private:
	std::wstring textureName;
	TextureType textureType;
	int UserCounter = 0;
	int uID = -1;
	unsigned short BackBufferIndex = 0;
	RECT Size;
private:
	friend class TextureManager;
};
