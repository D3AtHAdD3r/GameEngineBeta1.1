//#include<GraphicEngine/Utilities/Headers/Headers.h>
#include<unordered_map>


class ConstantBuffer;

class ConstantBufferManager
{
public:
	ConstantBufferManager();
	~ConstantBufferManager();

public:
	//we just create one constant buffer for all primitivers for now on
	ConstantBuffer* CreateConstantBuffer(void* buffer, const unsigned int& size_buffer, const unsigned short& u_id);
	void releaseAll();

public:
	bool freeConstantBuffer(ConstantBuffer* pCbuff);

private:
	bool check_Exist(const unsigned short& u_id);

private:
	bool DeleteConstantBuffer(ConstantBuffer* pCbuff);
	bool DeleteConstantBuffer(short uID);
	void DeleteConstantBuffer_direct(ConstantBuffer* pCbuff);

private:
	std::unordered_map<unsigned short, ConstantBuffer*> ConstantBufferContainer;
	unsigned short ResourceCount = 0;
};

