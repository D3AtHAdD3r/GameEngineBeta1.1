#include<GraphicEngine/Utilities/Headers/HeadersD3D.h>
#include<string>


class PixelShader
{
private:
	PixelShader(std::wstring shaderfilename, std::string entryPointName, std::string shader_model, const int& u_ID);
	~PixelShader();

public:
	ID3D11PixelShader* getPixelShader();
	std::wstring getfilename();

private:
	void incrementCounter();
	void decrementCounter();

private:
	ID3D11PixelShader* pShader = nullptr;
	ID3DBlob* psblob = nullptr;
	std::wstring file_name;
	unsigned int UserCounter = 0;
	int uID = -1;
private:
	friend class PixelShaderManager;
};