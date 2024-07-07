//#include<GraphicEngine\Headers\RendererDx11Headers.h>
#include<unordered_map>

#ifdef PROJ_EXPORT
#define IR_Interface __declspec(dllexport)
#else
#define IR_Interface __declspec(dllimport)
#endif

class RendererDX11;
struct RenderData;
class Entity;
class Scene;

struct Scene_descriptor;
struct EntityDesc;

static RendererDX11* pRenderer = nullptr;

IR_Interface bool CreateRenderer(RenderData* pRenderData);
IR_Interface bool OnResize(unsigned int width, unsigned int height);
IR_Interface bool DrawFrame();
IR_Interface void OnShutDown();

IR_Interface bool CreateSceneAndEntity(std::vector<Scene_descriptor*> sd_list, std::vector<EntityDesc*> ed_list);
IR_Interface Entity* CreateEntity(EntityDesc* pED, bool check_ent_desc = true);
IR_Interface bool DeleteEntity(Entity* pEnt, Scene* pScene = nullptr);
IR_Interface const std::unordered_map<unsigned short, Scene*>& GetSceneContainer();