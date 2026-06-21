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

	m_SewerModel = std::make_shared<Model>("src/res/models/Sewer/Models/Sewers.fbx","Serwers01_003");
	auto sewer = m_Scene.AddGameObject("Sewer");
	sewer->transform->localPosition = glm::vec3(0.0f, 0.0f, -5.0f);
	sewer->transform->localEulerAngles = glm::vec3(-90.0f, 0.0f, 0.0f);
	sewer->AddComponent<MeshRenderer>(m_SewerModel);

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

	player = m_Scene.AddGameObject("Player");
	player->transform->localPosition = glm::vec3(0.0f, 1.5f, 0.0f);
	player->AddComponent<FirstPersonController>(m_Camera);

	flashlight = m_Scene.AddGameObject("Flashlight");
	m_FlashlightModel = std::make_shared<Model>("src/res/models/flashlight/flashlight.fbx");
	flashlight->transform->SetParent(player->transform);
	flashlight->transform->localPosition = glm::vec3(0.3f, -0.4f, -0.4f);
	flashlight->transform->localScale= glm::vec3(0.08f);
	flashlight->AddComponent<Flashlight>(m_SpotLight, *m_LightingShader,"u_SpotLight");
	flashlight->AddComponent<MeshRenderer>(m_FlashlightModel);
	m_HierarchyPanel.SetContext(&m_Scene.GetGameObjects());

	m_Scene.Awake();
	m_Scene.Start();
}

void test::TestHorrorMaze::OnUpdate(float deltaTime)
{
	if (m_SpotLight && m_Camera) {
		m_SpotLight->direction = m_Camera->GetCameraFront();
	}
	m_Scene.Update(MyTime::GetDeltaTime());
}

void test::TestHorrorMaze::OnRender()
{
	BeginScene();
	m_LightingShader->Bind();

	m_LightingShader->SetUniformMat4f("u_View", m_Camera->GetViewMatrix());
	m_LightingShader->SetUniformMat4f("u_Projection", m_Camera->GetProjectionMatrix());
	m_LightingShader->SetUniform3f("u_ViewPos", m_Camera->GetCameraPos().x, m_Camera->GetCameraPos().y, m_Camera->GetCameraPos().z);

	m_Scene.Draw(*m_LightingShader);
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

void test::TestHorrorMaze::BeginScene()
{
	if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
		(m_Framebuffer->GetWidth() != m_ViewportSize.x || m_Framebuffer->GetHeight() != m_ViewportSize.y))
	{
		m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_Camera->SetAspectRatio(m_ViewportSize.x / m_ViewportSize.y);
	}
	m_Framebuffer->Bind();
	GLCALL(glEnable(GL_DEPTH_TEST));
	GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

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
			if (data.type == Tile::Empty) continue;

			std::shared_ptr<GameObject> tileGo = nullptr;
			std::string tileName = "Tile_" + std::to_string(x) + "_" + std::to_string(z);
			if (data.type == Tile::Straight) {
				tileGo = MazePrefabFactory::CreateStraight(tileName, straightModel, m_Scene.GetGameObjects());
			}
			else if (data.type == Tile::Corner) {
				tileGo = MazePrefabFactory::CreateCorner(tileName, cornerModel, m_Scene.GetGameObjects());
			}
			if (tileGo)
			{
				tileGo->transform->localPosition = glm::vec3(x * TILE_SIZE, 0.0f, z * -TILE_SIZE);
				tileGo->transform->localEulerAngles = glm::vec3(0.0f, data.rot, 0.0f);
			}
		}
	}
}
