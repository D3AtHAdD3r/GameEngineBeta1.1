#include<unordered_map>
#include<string>

class PixelShader;


class PixelShaderManager
{
public:
	PixelShaderManager();
	~PixelShaderManager();

public:
	PixelShader* CreatePixelShader(std::wstring shaderfilename, std::string entryPointName, std::string shader_model, const int& u_id);
	bool freePixelShader(PixelShader* pVShader);
	void releaseAll();


private:
	bool check_Exist(const int& u_id);
	bool DeletePixelShader(PixelShader* pVShader);
	bool DeletePixelShader(const int& u_id);
	void DeletePixelShader_direct(PixelShader* pVShader);

private:
	std::unordered_map<int, PixelShader*> PixelShaderContainer;
	int ResourceCount = 0;
};