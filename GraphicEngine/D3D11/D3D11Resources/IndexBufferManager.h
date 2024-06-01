#include<unordered_map>
#include<string>

class IndexBuffer;


class IndexBufferManager
{
public:
	IndexBufferManager();
	~IndexBufferManager();

public:
	IndexBuffer* CreateIndexBuffer(void* list_indices, const unsigned int& size_list, std::wstring mesh_Name, const int& u_id);
	bool freeIndexBuffer(IndexBuffer* pIbuff);
	void releaseAll();


private:
	bool check_Exist(const int& u_id);
	bool DeleteIndexBuffer(IndexBuffer* pIbuff);
	bool DeleteIndexBuffer(const int& u_id);
	void DeleteIndexBuffer_direct(IndexBuffer* pIbuff);

private:
	std::unordered_map<int, IndexBuffer*> IndexBufferContainer;
	int ResourceCount = 0;
};

