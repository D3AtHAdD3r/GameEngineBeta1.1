#include<GraphicEngine/Utilities/Headers/HeadersD3D.h>
#include<string>


class VertexShader
{
private:
	VertexShader( std::wstring shaderfilename, std::string entryPointName, std::string shader_model, const int& u_ID);
	~VertexShader();

public:
	ID3D11VertexShader* getVertexShader();
	std::wstring getfilename();
	void* GetShader_Byte_Code();
	const unsigned long long& GetBufferSize();

private:
	void incrementCounter();
	void decrementCounter();

private:
	void* shader_byte_code = nullptr;
	unsigned long long bufferSize = 0;
	std::wstring file_name;

private:
	ID3D11VertexShader* vShader = nullptr;
	ID3DBlob* vsblob = nullptr;
	unsigned int UserCounter = 0;
	int uID = -1;

private:
	friend class VertexShaderManager;
};

