





class IApplication
{
public:
	virtual void onInit() = 0;
	virtual void onBeginFrame() = 0;
	virtual void onEndFrame() = 0;
	virtual void onShutdown() = 0;
	virtual ~IApplication() = default;
};