#include<unordered_map>
#include<string>

class VertexShader;


class VertexShaderManager
{
public:
	VertexShaderManager(const std::unordered_map<int, std::wstring>& File_Map);
	~VertexShaderManager();

public:
	VertexShader* CreateVertexShader(std::string entryPointName, std::string shader_model, const int& u_id);
	bool freeVertexShader(VertexShader* pVShader);
	void releaseAll();

public:
	bool CreateAll(std::string entryPointName, std::string shader_model);

private:
	bool check_Exist(const int& u_id);
	bool DeleteVertexShader(VertexShader* pVShader);
	bool DeleteVertexShader(const int& u_id);
	void DeleteVertexShader_direct(VertexShader* pVShader);

private:
	std::wstring GetFileName(const int& uID);

private:
	std::unordered_map<int, VertexShader*> VertexShaderContainer;
	int ResourceCount = 0;
private:
	std::unordered_map<int, std::wstring> FileMap;
};