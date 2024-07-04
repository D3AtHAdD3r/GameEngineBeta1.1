#include "TestGame.h"
#include<GraphicEngine/ECS/ECSHeaders/EntityStructs.h>
#include<GraphicEngine/ECS/Scene/Scene.h>
#include<GraphicEngine/ECS/Components/Camera.h>
#include<GraphicEngine/ECS/Entity/Entity.h>
#include<GraphicEngine/ECS/Entity/EntityChilds/NormalEntity.h>
#include<GraphicEngine/ECS/Entity/EntityChilds/LocalPlayer.h>
#include<GraphicEngine/ECS/Components/ModelData.h>
#include<d3d11.h>

TestGame::TestGame()
{
	
}

TestGame::~TestGame()
{
	if (cb) delete cb;
	
}

bool TestGame::onInit()
{
	if (!Create_Scene_And_Entity())
	{
		printf("Create_Scene_And_Entity failed");
		return false;
	}
		

	SceneContainer = GetSceneContainer();
	if (SceneContainer.empty())
	{
		printf("Scene container is empty\n");
		return false;
	}
		

	if (!UpdateOnInit(SceneContainer))
	{
		printf("UpdateOnInit(SceneContainer) failed\n");
		return false;
	}

	return true;
}

bool TestGame::onBeginFrame()
{
	SceneContainer.clear();
	SceneContainer = GetSceneContainer();
	if (SceneContainer.empty())
	{
		printf("Scene container is empty\n");
		return false;
	}
		

	if (!Update())
	{
		printf("TestGame::Update Failed\n");
		return false;
	}
		
	return true;
}

bool TestGame::onEndFrame()
{

	return true;
}

bool TestGame::onShutdown()
{

	return true;
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
				Vector3D rot = { 0.785398f,0, 0.0f };
				if (!camera->Update_Rotation_Direct(rot)) return false;
			}
		}

		for (auto& [typeindex, entityContainer] : scene->GetEntityContainer())
		{
			Entity* Parent = nullptr;
			for (auto& currEnt : entityContainer)
			{
				if (currEnt->Get_Entity_uID() == 1) //skybox
				{
					//skybox
					Vector3D scale = { 4000.0f, 4000.0f, 4000.0f };
					currEnt->Get_ModelData()->Update_Scaling(scale);
				}
				if (currEnt->Get_Entity_uID() == 2) //dragon
				{
					//dragon
					Vector3D scale = { 35.0f, 35.0f, 35.0f };
					currEnt->Get_ModelData()->Update_Scaling(scale);

					Parent = currEnt; //to attach camera to dinosaur
				}

			}

			//DigiCam
			for (auto& currEnt : entityContainer)
			{
				if (currEnt->Get_Entity_uID() == 3)
				{
					EntityAttachDetails ed;
					/*ed.delta_offset_model_y = 0.009;
					ed.delta_offset_model_z = 0.02;*/
					ed.delta_offset_model_y = 0.3;
					ed.delta_offset_model_z = 0.4;

					NormalEntity* currentEntity = dynamic_cast<NormalEntity*>(currEnt);
					if (!currentEntity->Attach(Parent, &ed))
						return false;
				}
			}
		}

	}

	return true;
}



bool TestGame::AttachCamera(int Entiy_uID, int Camera_uID, Scene* pScene)
{
	CameraAttachDetails cd;
	cd.camType = CameraType::fpc;
	cd.delta_offset_model_y = 0.3;
	cd.delta_offset_model_z = 0.45;

	if (!pScene->Attach_Camera(Entiy_uID, Camera_uID, &cd)) return false;

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
					if (!AttachCamera(2, currentScene->getActiveCamera()->Get_uID(), currentScene)) return false;
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
			if (currentEntity->Get_Entity_uID() == 0) //asteroid
			{
				Vector3D current_Rotation = currentEntity->Get_ModelData()->Get_Rotation();
				current_Rotation.m_y = current_Rotation.m_y + 0.01f * 0.5f;
				if (!currentEntity->Get_ModelData()->Update_Rotation(current_Rotation)) return false;
			}

			if (currentEntity->Get_Entity_uID() == 2) //dragon
			{
				Vector3D current_Rotation = currentEntity->Get_ModelData()->Get_Rotation();
				current_Rotation.m_y = current_Rotation.m_y + 0.01f * 0.5f;
				if (!currentEntity->Get_ModelData()->Update_Rotation(current_Rotation)) return false;
			}

			//have to do for now
			if (currentEntity->Get_Entity_uID() == 3)
			{
				currentEntity->Update();
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
	float zFar = 10000.0f;
	float aspectRatio = Get_AspectRatio();

	CameraInitData cd;
	cd.GetInput = true;
	cd.isProjecting = true;
	cd.uID = 0;
	cd.fov = fov;
	cd.zNear = zNear;
	cd.zFar = zFar;
	cd.aspect_ratio = aspectRatio;
	cd.SmoothRotation = true;
	cd.SmoothTranslation = true;

	CameraInitData cd1;
	cd1.GetInput = false;
	cd1.isProjecting = false;
	cd1.uID = 1;
	cd1.fov = fov;
	cd1.zNear = zNear;
	cd1.zFar = zFar;
	cd1.aspect_ratio = aspectRatio;
	cd1.world_pos = { 0,100,-20.0f };
	cd1.SmoothRotation = true;
	cd1.SmoothTranslation = true;

	sd.camData.push_back(&cd);
	sd.camData.push_back(&cd1);
	sd.NumberofCams = sd.camData.size();

	sd_list.push_back(&sd);
	//----------------------------------------------------//

	//Asteroid
	EntityDesc ed;
	ed.mesh_uid = 0;
	ed.getMeshfromFile = true;

	ed.Texture_Concrete_uIDs.push_back({ Entity_Texture_Type::Tex_Material, 1 });
	ed.Texture_Concrete_uIDs.push_back({ Entity_Texture_Type::Tex_Custom, 2 });
	ed.Entity_type = ENTITY_TYPE::ENUM_NORMAL_ENTITY;
	ed.model_initialPosition = { 0,30,100.0f };
	//ed.model_initialScaling = { 7.0f,7.0f,7.0f };

	ed.vertex_Shader_uid = 0;
	ed.inLayout = layout;
	ed.sizeLayout = ARRAYSIZE(layout);
	ed.pixel_Shader_uid = 0;

	ed.constant_buffer = cb;
	ed.size_constant_buffer = sizeof(constant);
	ed.constant_buffer_uid = 0;

	ed.primitive_name = L"Asteroid";
	ed.primitive_uid = 0;
	ed.Scene_Id = 0;
	ed_list.push_back(&ed);


	//Skybox
	EntityDesc ed2;
	ed2.mesh_uid = 1;
	ed2.getMeshfromFile = true;
	ed2.Texture_Concrete_uIDs.push_back({ Entity_Texture_Type::Tex_Material, 2 });
	ed2.Entity_type = ENTITY_TYPE::ENUM_NORMAL_ENTITY;

	ed2.vertex_Shader_uid = 0;
	ed2.inLayout = layout;
	ed2.sizeLayout = ARRAYSIZE(layout);
	ed2.pixel_Shader_uid = 1;
	ed2.constant_buffer = cb;
	ed2.size_constant_buffer = sizeof(constant);
	ed2.constant_buffer_uid = 0;

	ed2.frontFaceCull = true;
	ed2.primitive_uid = 1;
	ed2.Scene_Id = 0;
	ed2.primitive_name = L"skybox";
	ed_list.push_back(&ed2);

	//Dragon
	EntityDesc ed3;
	ed3.mesh_uid = 2;
	ed3.getMeshfromFile = true;
	ed3.Texture_Concrete_uIDs.push_back({ Entity_Texture_Type::Tex_Material , 3 });
	ed3.Entity_type = ENTITY_TYPE::ENUM_NORMAL_ENTITY;

	ed3.vertex_Shader_uid = 0;
	ed3.inLayout = layout;
	ed3.sizeLayout = ARRAYSIZE(layout);
	ed3.pixel_Shader_uid = 0;
	ed3.constant_buffer = cb;
	ed3.size_constant_buffer = sizeof(constant);
	ed3.constant_buffer_uid = 0;

	ed3.frontFaceCull = false;
	ed3.primitive_uid = 2;
	ed3.primitive_name = L"Dragon";
	ed3.Scene_Id = 0;

	ed3.model_initialPosition = { 0,20,40 };
	ed3.SmoothRotation = true;
	ed_list.push_back(&ed3);

	//DigiCam
	EntityDesc ed4;
	ed4.mesh_uid = 3;
	ed4.getMeshfromFile = true;
	ed4.Texture_Concrete_uIDs.push_back({ Entity_Texture_Type::Tex_Material , 4 });
	ed4.Entity_type = ENTITY_TYPE::ENUM_NORMAL_ENTITY;

	ed4.vertex_Shader_uid = 0;
	ed4.inLayout = layout;
	ed4.sizeLayout = ARRAYSIZE(layout);
	ed4.pixel_Shader_uid = 0;
	ed4.constant_buffer = cb;
	ed4.size_constant_buffer = sizeof(constant);
	ed4.constant_buffer_uid = 0;

	ed4.frontFaceCull = false;
	ed4.primitive_uid = 3;
	ed4.primitive_name = L"DigiCam";
	ed4.Scene_Id = 0;

	ed4.model_initialPosition = { 0,0,20 };
	ed4.model_initialScaling = { 0.3,0.3,0.3 };
	ed4.model_initialRotation = { 4.71239 , 0, 0 };
	ed4.SmoothRotation = true;
	ed_list.push_back(&ed4);


	//Terrain
	EntityDesc ed5;
	ed5.getMeshfromFile = false;
	ed5.isTerrainMesh = true;
	ed5.mesh_uid = 111;
	ed5.TerrainSize = { 512.0f, 100.0f, 512.0f, 0.0f }; 
	ed5.Texture_Concrete_uIDs.push_back({ Entity_Texture_Type::Tex_Height_Map, 5 });
	ed5.Texture_Concrete_uIDs.push_back({ Entity_Texture_Type::Tex_Material, 6 });
	ed5.Texture_Concrete_uIDs.push_back({ Entity_Texture_Type::Tex_Custom, 7 });
	ed5.Entity_type = ENTITY_TYPE::ENUM_NORMAL_ENTITY;
	ed5.model_initialPosition = { -500, -300, 0 };
	ed5.model_initialScaling = { 4,4,4 };

	ed5.vertex_Shader_uid = 1;
	ed5.inLayout = layout;
	ed5.sizeLayout = ARRAYSIZE(layout);
	ed5.pixel_Shader_uid = 3;
	
	ed5.constant_buffer = cb;
	ed5.size_constant_buffer = sizeof(constant);
	ed5.constant_buffer_uid = 0;
	
	ed5.primitive_name = L"Terrain";
	ed5.primitive_uid = 77;
	ed5.Scene_Id = 0;
	ed5.frontFaceCull = false;
	ed_list.push_back(&ed5);

	if (!CreateSceneAndEntity(sd_list, ed_list))
	{
		return false;
	}

	return true;
}

