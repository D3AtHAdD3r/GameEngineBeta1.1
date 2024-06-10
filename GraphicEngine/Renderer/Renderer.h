#include<vector>
#include<unordered_map>

struct Scene_descriptor;
struct EntityDesc;
class Entity;
class Scene;

class Renderer
{
	friend class GraphicEngine;
protected:
	Renderer() = default;
	virtual ~Renderer() = default;
public:
	virtual bool CreateSceneAndEntity(std::vector<Scene_descriptor*> sd_list, std::vector<EntityDesc*> ed_list) = 0;
	virtual Entity* CreateEntity(EntityDesc* pED, bool check_ent_desc = true) = 0;
	virtual bool DeleteEntity(Entity* pEnt, Scene* pScene = nullptr) = 0;
	virtual const std::unordered_map<unsigned short, Scene*>& GetSceneContainer() = 0;

public:
	virtual bool DrawFrame() = 0;
	virtual void OnResize() = 0;
};