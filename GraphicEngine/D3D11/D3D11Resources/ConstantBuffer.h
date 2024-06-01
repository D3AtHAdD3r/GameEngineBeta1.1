#include<GraphicEngine/Utilities/Headers/HeadersD3D.h>




class ConstantBuffer
{
private:
	ConstantBuffer(void* buffer, const unsigned int& size_buffer, short u_ID);
	~ConstantBuffer();

public:
	ID3D11Buffer* getConstantBuffer();
	const short& GetUid();

private:
	void incrementCounter();
	void decrementCounter();

private:
	ID3D11Buffer* cBuffer = nullptr;
	unsigned int UserCounter = 0;
	short uID = -1;

private:
	friend class ConstantBufferManager;
};