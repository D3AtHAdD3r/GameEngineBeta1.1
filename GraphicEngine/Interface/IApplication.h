





class IApplication
{
public:
	virtual void onInit() = 0;
	virtual void onUpdate() = 0;
	virtual void onEnd() = 0;
	virtual ~IApplication() = default;
};