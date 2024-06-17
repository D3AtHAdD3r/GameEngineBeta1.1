#include "TestGame.h"
#include<GraphicEngine/ECS/ECSHeaders/EntityStructs.h>
#include<GraphicEngine/ECS/Scene/Scene.h>
#include<GraphicEngine/ECS/Components/Camera.h>
#include<GraphicEngine/ECS/Entity/Entity.h>
#include<GraphicEngine/ECS/Entity/EntityChilds/NormalEntity.h>
#include<GraphicEngine/ECS/Entity/EntityChilds/LocalPlayer.h>
#include<d3d11.h>

TestGame::TestGame()
{
	
}

TestGame::~TestGame()
{
	if (cb) delete cb;
	
}

void TestGame::onInit()
{
	if (!Create_Scene_And_Entity())
		printf("Create_Scene_And_Entity failed");

	SceneContainer = GetSceneContainer();
	if (SceneContainer.empty())
		printf("Scene container is empty\n");

	UpdateEntitiesOnInit(SceneContainer);
}

void TestGame::onBeginFrame()
{
	SceneContainer.clear();
	SceneContainer = GetSceneContainer();
	if (SceneContainer.empty())
		printf("Scene container is empty\n");

	if (!Update())
		printf("TestGame::Update Failed\n");
}

void TestGame::onEndFrame()
{
}

void TestGame::onShutdown()
{
}

void TestGame::onKeyDown(int key)
{
}

void TestGame::onKeyUp(int key)
{
	if (key == VK_ESCAPE)
	{
		ShowCursorFlag = !ShowCursorFlag;
		ShowCursor(ShowCursorFlag);
	}

	if (key == VK_TAB)
	{
		changeCameraFlag = !changeCameraFlag;

		if (changeCameraFlag)
		{
			SceneContainer[0]->Activate_Camera(1);
		}
		else
		{
			SceneContainer[0]->Activate_Camera(0);
		}
	}
}

void TestGame::onMouseMove(const Point& mouse_pos)
{
}

void TestGame::onLeftMouseDown(const Point& mouse_pos)
{
}

void TestGame::onLeftMouseUp(const Point& mouse_pos)
{
}

void TestGame::onRightMouseDown(const Point& mouse_pos)
{
}

void TestGame::onRightMouseUp(const Point& mouse_pos)
{
}

bool TestGame::Create_Scene_And_Entity()
{
	std::vector<Scene_descriptor*> sd_list;
	std::vector<EntityDesc*> ed_list;

	cb = new constant();

	static D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//SEMANTIC NAME - SEMANTIC INDEX - FORMAT - INPUT SLOT - ALIGNED BYTE OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0,  DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA ,0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	Scene_descriptor sd;
	sd.scene_name = L"Scene1";
	sd.scene_id = 0;
	sd.scene_texture_uid = 0;
	sd.connect_backbuffer = true;
	sd.BackBuffer_Index = 0;
	sd.window_client_width = 0;
	sd.window_client_height = 0;
	sd.width_ratio = 1;
	sd.height_ratio = 1;
	sd.clearRenderTargetView = true;
	sd.useDepthStencil = true;
	sd.clearDepthStencil = true;
	
	CameraInitData cd;
	cd.get_Input = true;
	cd.isTPC = false;
	cd.isprojecting = true;
	cd.u_id = 0;

	CameraInitData cd1;
	cd1.get_Input = false;
	cd1.isTPC = false;
	cd1.isprojecting = false;
	cd1.u_id = 1;

	sd.camData.push_back(&cd);
	sd.camData.push_back(&cd1);
	sd.NumberofCams = sd.camData.size();

	sd_list.push_back(&sd);
	//----------------------------------------------------//

	EntityDesc ed;
	ed.mesh_uid = 0;
	ed.getMeshfromFile = true;
	ed.texture_uids.push_back(1);
	ed.numOfTextures = 1;
	ed.primitive_texture_type = Primitive_texture_type::oneTexMap_perDrawCall;
	ed.Entity_type = ENTITY_TYPE::ENUM_NORMAL_ENTITY;
	ed.model_initialPosition = { 0,0,20.0f };

	ed.vertex_Shader_uid = 0;
	ed.inLayout = layout;
	ed.sizeLayout = ARRAYSIZE(layout);

	ed.pixel_Shader_uid = 0;

	ed.constant_buffer = cb;
	ed.size_constant_buffer = sizeof(constant);
	ed.constant_buffer_uid = 0;

	ed.primitive_name = L"asteroid";
	ed.primitive_uid = 0;
	ed.Scene_Id = 0;

	ed_list.push_back(&ed);

	if (!CreateSceneAndEntity(sd_list, ed_list))
	{
		return false;
	}

	return true;
}

bool TestGame::Update()
{
	if (SceneContainer.empty()) return false;

	for (auto& [uid, currentScene] : SceneContainer)
	{
		//update all of them
		for (auto& [uid, camera] : currentScene->Get_Camera_Container())
		{
			camera->updateCamera();
		}

		for (auto& [TypeIndex, EntityContainer] : currentScene->GetEntityContainer())
		{
			std::vector<Entity*> CurrentEntContainer = EntityContainer;

			if (TypeIndex == typeid(NormalEntity))
			{
				if (!Update_NormalEntity(CurrentEntContainer, currentScene)) return false;
			}
			else if (TypeIndex == typeid(LocalPlayer))
			{
				if (!Update_LocalPlayer(CurrentEntContainer, currentScene)) return false;
			}
			
		}

	}

	return true;
}

bool TestGame::Update_NormalEntity(std::vector<Entity*>& EntityContainer, Scene* currScene)
{
	for (int i = 0; i < EntityContainer.size(); ++i)
	{
		if (NormalEntity* currentEntity = dynamic_cast<NormalEntity*>(EntityContainer[i]))
		{
			if (currentEntity->Get_Entity_uID() == 0)
			{
				ModelPositionData mp;
				Vector3D currRotation = (currentEntity)->Get_Rotation();
				mp.delta_rotation_x = currRotation.m_x;
				mp.delta_rotation_y = currRotation.m_y;
				mp.delta_rotation_z = currRotation.m_z;

				mp.SmoothRotation = false;

				currRotation.m_x += 0.01f * 0.5f;
				currRotation.m_y += 0.01f * 0.5f;
				currRotation.m_z += 0.01f * 0.5f;

				currentEntity->Set_Rotaion(currRotation.m_x, currRotation.m_y, currRotation.m_z);

				//----------------------------------//

				/*if (val_translation < 300)
				{
					mp.delta_translation_z = 1;
				}
				else
				{
					mp.delta_translation_z = -1;
				}

				if (val_translation == 600)
					val_translation = 0;

				val_translation++;

				mp.SmoothMovement = true;*/

				currentEntity->UpdatePosition(&mp);
				//Matrix4x4 camWorldMatrix = currScene->getActiveCamera()->getWorldMatrix();
				//currentEntity->UpdatepositionRelative(&mp, &camWorldMatrix);
			}
		}
		else
		{
			printf("dynamic_casting in TestGame::UpdateNORMALENTITY failed\n ");
			return false;
		}
	}

	return true;
}

bool TestGame::Update_LocalPlayer(std::vector<Entity*>& EntityContainer, Scene* currScene)
{
	for (int i = 0; i < EntityContainer.size(); ++i)
	{
		if (LocalPlayer* ent = dynamic_cast<LocalPlayer*>(EntityContainer[i]))
		{

		}
		else
		{
			printf("dynamic_casting in TestGame::Update_LocalPlayer failed\n ");
			return false;
		}
	}

	return true;
}






bool TestGame::UpdateEntitiesOnInit(std::unordered_map<unsigned short, Scene*>& SceneContainer)
{
	for (auto& [uid, scene] : SceneContainer)
	{
		for (auto& [uid, camera] : scene->Get_Camera_Container())
		{
			if (uid == 1)
			{
				CameraTranslationData cd;
				cd.delta_rotation_y = 3.14159f;
				camera->updateCamera(&cd);
				camera->setTranslation({ 0, 0, 40.0f });
			}
		}
	}

	return true;
}

