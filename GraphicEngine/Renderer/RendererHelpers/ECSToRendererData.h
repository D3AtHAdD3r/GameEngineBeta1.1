


struct Renderer_PreBindData;
struct Renderer_BindingData;
class Scene;
class Primitive;

//no need to be singleton
class ECSToRendererData
{
	friend class RendererDX11;
private:
	ECSToRendererData() {};
	~ECSToRendererData() { /*delete this;*/ }

private:
	static void Create();
	static ECSToRendererData* Get();

public:
	static bool fill_Renderer_PreBindData(Scene* pScene, Renderer_PreBindData* pRPreBind);
	static bool fill_Renderer_MainBindData(Primitive* pPrim, Renderer_BindingData* pBindData);

private:
	static ECSToRendererData* pECSToRender;
};