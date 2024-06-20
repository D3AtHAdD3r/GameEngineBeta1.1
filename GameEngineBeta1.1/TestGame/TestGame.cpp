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

	UpdateOnInit(SceneContainer);
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

	if (key == VK_INSERT)
	{
		AttachCameraFlag = !AttachCameraFlag;
		Insert_Key_Pressed = true;
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


	float fov = 0.785398f;
	float zNear = 0.1f;
	float zFar = 5000.0f;
	float aspectRatio = 1024.0f / 768.0f;
	
	CameraInitData cd;
	cd.GetInput = true;
	cd.isProjecting = true;
	cd.uID = 0;
	cd.fov = fov;
	cd.zNear = zNear;
	cd.zFar = zFar;
	cd.aspect_ratio = aspectRatio;
	/*cd.SmoothRotation = true;
	cd.SmoothTranslation = true;*/
	
	CameraInitData cd1;
	cd1.GetInput = false;
	cd1.isProjecting = false;
	cd1.uID = 1;
	cd1.fov = fov;
	cd1.zNear = zNear;
	cd1.zFar = zFar;
	cd1.aspect_ratio = aspectRatio;
	cd1.world_pos = { 0,0,40.0f };

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


	EntityDesc ed2 = ed;
	ed2.primitive_uid = 1;
	ed2.model_initialPosition = { 0,0,80.0f };
	ed_list.push_back(&ed2);

	if (!CreateSceneAndEntity(sd_list, ed_list))
	{
		return false;
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


bool TestGame::UpdateOnInit(std::unordered_map<unsigned short, Scene*>& SceneContainer)
{
	for (auto& [uid, scene] : SceneContainer)
	{
		//Rotate Second Camera 180 degrees
		for (auto& [uid, camera] : scene->Get_Camera_Container())
		{
			if (uid == 1)
			{
			
				Vector3D rot = { 0.0f,3.14159f, 0.0f };
				CameraPositionData cd;
				cd.delta_rotation_y = rot.m_y;

				//if (!camera->Update(&cd)) return false;
				if (!camera->Update_Rotation_Direct(rot)) return false;
			}
		}
	}

	return true;
}



bool TestGame::AttachCamera(int Entiy_uID, int Camera_uID, Scene* pScene)
{
	CameraAttachDetails cd;
	cd.camType = CameraType::fpc;
	cd.delta_offset_model_z = 3.0f;

	if (pScene->Attach_Camera(Entiy_uID, Camera_uID, &cd)) return false;

	return true;
}

bool TestGame::Update()
{
	if (SceneContainer.empty()) return false;

	for (auto& [uid, currentScene] : SceneContainer)
	{
		//update all cameras
		for (auto& [uid, camera] : currentScene->Get_Camera_Container())
		{
			camera->Update();
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

		if (Insert_Key_Pressed)
		{
			if (AttachCameraFlag)
			{
				if (currentScene->Get_Scene_uID() == 0)
				{
					if (!AttachCamera(0, currentScene->getActiveCamera()->Get_uID(), currentScene)) return false;
				}
			}
			else
			{
				if (currentScene->Get_Scene_uID() == 0)
				{
					if (!currentScene->Detach_Camera(currentScene->getActiveCamera()->Get_uID())) return false;
				}
			}

			Insert_Key_Pressed = false;
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
				/*Vector3D currRotation = (currentEntity)->Get_Rotation();
				mp.delta_rotation_x = currRotation.m_x;
				mp.delta_rotation_y = currRotation.m_y;
				mp.delta_rotation_z = currRotation.m_z;

				mp.SmoothRotation = false;

				currRotation.m_x += 0.01f * 0.5f;
				currRotation.m_y += 0.01f * 0.5f;
				currRotation.m_z += 0.01f * 0.5f;

				currentEntity->Set_Rotaion(currRotation.m_x, currRotation.m_y, currRotation.m_z);*/
				//currentEntity->UpdatePosition(&mp);
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

