#include "TestHorrorMaze.h"
#include "Core/MeshRenderer.h"
#include "Window/MyWindow.h"
#include "imgui/imgui.h"
#include "MyTime.h"
#include "Demo/Flashlight.h"
#include "Core/BoxCollider.h"
#include "Demo/MazePrefabFactory.h"


test::TestHorrorMaze::TestHorrorMaze()
	:m_Camera(std::make_shared<Camera>( glm::vec3(0.0f,1.5f,0.0f)))
{
	m_Framebuffer = std::make_unique<Framebuffer>(MyWindow::GetWidth(), MyWindow::GetHeight());

	m_LightingShader = std::make_unique<Shader>("src/res/shaders/LightingShader.vert",
		"src/res/shaders/LightingShader.frag");

	//m_MonsterModel = std::make_shared<Model>("src/res/models/Characters_psx/Models/Killers/Character_Killer.fbx");
	//m_MonsterModel = std::make_shared<Model>("src/res/models/Characters.fbx");
	m_SewerModel = std::make_shared<Model>("src/res/models/Sewer/Models/Sewers.fbx","Serwers01_003");
	auto sewer = std::make_shared<GameObject>("Sewer");
	sewer->transform->localPosition = glm::vec3(0.0f, 0.0f, -5.0f); // 放在摄像机正前方 5 米处
	sewer->transform->localEulerAngles = glm::vec3(-90.0f, 0.0f, 0.0f);
	//sewer->transform->localScale = glm::vec3(0.01f);

	sewer->AddComponent<MeshRenderer>(m_SewerModel);
	m_GameObjects.push_back(sewer);

	m_SpotLight = std::make_shared<SpotLight>
		(
			glm::vec3(0.0f,1.5f,0.0f),
			glm::vec3(1.0f, 0.045f, 0.0075f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(1.0f),
			12.5f,
			25.5f,
			1.0f
		);

	GenerateMaze();

	player = std::make_shared<GameObject>("Player");
	player->transform->localPosition = glm::vec3(0.0f, 1.5f, 0.0f);
	player->AddComponent<FirstPersonController>(m_Camera);
	//player->AddComponent<BoxCollider>(glm::vec3(0.6f, 1.8f, 0.6f));
	m_GameObjects.push_back(player);

	flashlight = std::make_shared<GameObject>("Flashlight");
	m_FlashlightModel = std::make_shared<Model>("src/res/models/flashlight/flashlight.fbx");
	flashlight->transform->SetParent(player->transform);
	flashlight->transform->localPosition = glm::vec3(0.3f, -0.4f, -0.4f);
	flashlight->transform->localScale= glm::vec3(0.08f);
	flashlight->AddComponent<Flashlight>(m_SpotLight, *m_LightingShader,"u_SpotLight");
	flashlight->AddComponent<MeshRenderer>(m_FlashlightModel);
	m_GameObjects.push_back(flashlight);
	m_HierarchyPanel.SetContext(&m_GameObjects);

	//唤醒所有物体
	for (auto& go : m_GameObjects) {
		go->Awake();
	}

	for (auto& go : m_GameObjects) {
		go->Start();
	}
}

void test::TestHorrorMaze::OnUpdate(float deltaTime)
{
	if (m_SpotLight && m_Camera) {
		m_SpotLight->direction = m_Camera->GetCameraFront();
		//flashlight->transform->localPosition = m_Camera->GetCameraPos();
	}
	for (auto& go : m_GameObjects) {
		go->Update(MyTime::GetDeltaTime());
	}
}

void test::TestHorrorMaze::OnRender()
{
	BeginScene();
	m_LightingShader->Bind();

	m_LightingShader->SetUniformMat4f("u_View", m_Camera->GetViewMatrix());
	m_LightingShader->SetUniformMat4f("u_Projection", m_Camera->GetProjectionMatrix());

	m_LightingShader->SetUniform3f("u_ViewPos", m_Camera->GetCameraPos().x, m_Camera->GetCameraPos().y, m_Camera->GetCameraPos().z);

	//m_CameraController.OnUpdate(MyTime::GetDeltaTime());

	for (auto& go : m_GameObjects) {
		go->Draw(*m_LightingShader);
	}
	EndScene();
}

void test::TestHorrorMaze::OnImGuiRender()
{
	ImGui::Begin("Game Viewport");


	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = glm::vec2(viewportPanelSize.x, viewportPanelSize.y);


	uint32_t textureID = m_Framebuffer->GetColorAttachment();

	ImGui::Image((void*)(intptr_t)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	ImGui::End();

	m_HierarchyPanel.OnImGuiRender();
}

/// <summary>
	/// 开始场景
	/// </summary>
void test::TestHorrorMaze::BeginScene()
{
	if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // 防止最小化时崩溃
		(m_Framebuffer->GetWidth() != m_ViewportSize.x || m_Framebuffer->GetHeight() != m_ViewportSize.y))
	{
		// 重新生成对应大小的 FBO
		m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

		// 更新摄像机的投影矩阵宽高比，防止画面被拉伸
		m_Camera->SetAspectRatio(m_ViewportSize.x / m_ViewportSize.y);
	}
	m_Framebuffer->Bind();
	GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	//GLCALL(glViewport(0, 0, MyWindow::GetWidth(), MyWindow::GetHeight()));
	GLCALL(glEnable(GL_DEPTH_TEST));
	//GLCALL(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
	GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

/// <summary>
/// 结束场景
/// </summary>
void test::TestHorrorMaze::EndScene()
{
	m_Framebuffer->UnBind();
}

void test::TestHorrorMaze::GenerateMaze()
{
	enum class Tile { Empty, Straight, Corner };

	struct MapData { Tile type; float rot; };

	MapData map[3][3] = {
		{ {Tile::Corner,   90.0f}, {Tile::Straight,  90.0f}, {Tile::Corner,  180.0f} },

		{ {Tile::Straight,  0.0f}, {Tile::Empty,      0.0f}, {Tile::Straight,  0.0f} },

		{ {Tile::Corner,    0.0f}, {Tile::Straight,  90.0f}, {Tile::Corner,  -90.0f} }
	};

	const float TILE_SIZE = 12.0f;

	auto straightModel = std::make_shared<Model>("src/res/models/Sewer/Models/Sewers.fbx", "Serwers01_005");
	auto cornerModel = std::make_shared<Model>("src/res/models/Sewer/Models/Sewers.fbx", "Serwers01_003");

	for (int x = 0; x < 3; x++)
	{
		for (int z = 0; z < 3; z++)
		{
			MapData data = map[x][z];
			if (data.type == Tile::Empty) continue; // 空地跳过

			std::shared_ptr<GameObject> tileGo = nullptr;
			std::string tileName = "Tile_" + std::to_string(x) + "_" + std::to_string(z);
			if (data.type == Tile::Straight) {
				tileGo = MazePrefabFactory::CreateStraight(tileName, straightModel, m_GameObjects);
			}
			else if (data.type == Tile::Corner) {
				tileGo = MazePrefabFactory::CreateCorner(tileName, cornerModel, m_GameObjects);
			}
			if (tileGo)
			{
				tileGo->transform->localPosition = glm::vec3(x * TILE_SIZE, 0.0f, z * -TILE_SIZE);
				tileGo->transform->localEulerAngles = glm::vec3(0.0f, data.rot, 0.0f);
			}
		}
	}
}