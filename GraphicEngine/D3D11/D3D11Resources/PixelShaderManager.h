#include<unordered_map>
#include<string>

class PixelShader;


class PixelShaderManager
{
public:
	PixelShaderManager(const std::unordered_map<int, std::wstring>& File_Map);
	~PixelShaderManager();

public:
	PixelShader* CreatePixelShader(std::string entryPointName, std::string shader_model, const int& u_id);
	bool freePixelShader(PixelShader* pVShader);
	void releaseAll();

public:
	bool CreateAll(std::string entryPointName, std::string shader_model);

private:
	bool check_Exist(const int& u_id);
	bool DeletePixelShader(PixelShader* pVShader);
	bool DeletePixelShader(const int& u_id);
	void DeletePixelShader_direct(PixelShader* pVShader);

private:
	std::wstring GetFileName(const int& uID);

private:
	std::unordered_map<int, PixelShader*> PixelShaderContainer;
	int ResourceCount = 0;
private:
	std::unordered_map<int, std::wstring> FileMap;
};