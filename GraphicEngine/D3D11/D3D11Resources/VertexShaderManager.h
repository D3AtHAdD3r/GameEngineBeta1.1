#include<unordered_map>
#include<string>

class VertexShader;


class VertexShaderManager
{
public:
	VertexShaderManager();
	~VertexShaderManager();

public:
	VertexShader* CreateVertexShader(std::wstring shaderfilename, std::string entryPointName, std::string shader_model, const int& u_id);
	bool freeVertexShader(VertexShader* pVShader);
	void releaseAll();

private:
	bool check_Exist(const int& u_id);
	bool DeleteVertexShader(VertexShader* pVShader);
	bool DeleteVertexShader(const int& u_id);
	void DeleteVertexShader_direct(VertexShader* pVShader);

private:
	std::unordered_map<int, VertexShader*> VertexShaderContainer;
	int ResourceCount = 0;
};