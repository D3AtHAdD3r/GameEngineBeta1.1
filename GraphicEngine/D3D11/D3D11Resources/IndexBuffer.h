#include<GraphicEngine/Utilities/Headers/HeadersD3D.h>




#define SIZE_VARIABLE 4


class IndexBuffer
{
private:
	IndexBuffer(void* list_indices, const unsigned int& size_list, const int& u_ID, std::wstring mesh_Name);
	~IndexBuffer();

public:
	ID3D11Buffer* getIndexBuffer();
	std::wstring getMeshName();
	const int& GetUid();
private:
	void incrementCounter();
	void decrementCounter();

private:
	ID3D11Buffer* iBuffer = nullptr;
	std::wstring meshName;
	unsigned int UserCounter = 0;
	int uID = -1;
private:
	friend class IndexBufferManager;
};
