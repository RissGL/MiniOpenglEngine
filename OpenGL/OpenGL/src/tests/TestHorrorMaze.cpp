#include "TestHorrorMaze.h"
#include "Core/MeshRenderer.h"
#include "Window/MyWindow.h"
#include "imgui/imgui.h"
#include "MyTime.h"

test::TestHorrorMaze::TestHorrorMaze()
	:m_Camera(glm::vec3(0.0,1.7,0.0)),
	m_CameraController(m_Camera,CameraType::Game)
{
	m_Framebuffer = std::make_unique<Framebuffer>(MyWindow::GetWidth(), MyWindow::GetHeight());

	m_LightingShader = std::make_unique<Shader>("src/res/shaders/LightingShader.vert",
		"src/res/shaders/LightingShader.frag");

	m_MonsterModel = std::make_shared<Model>("src/res/models/Characters_psx/Models/Killers/Character_Killer.fbx");

	auto monster = std::make_shared<GameObject>("Monster_SlenderMan");
	monster->transform->localPosition = glm::vec3(0.0f, 0.0f, -5.0f); // 放在摄像机正前方 5 米处
	monster->transform->localScale = glm::vec3(0.005f);

	monster->AddComponent<MeshRenderer>(m_MonsterModel);

	// 塞进场景
	m_GameObjects.push_back(monster);

	//唤醒所有物体
	for (auto& go : m_GameObjects) {
		go->Awake();
	}
}

void test::TestHorrorMaze::OnUpdate(float deltaTime)
{
	m_CameraController.OnUpdate(deltaTime);

	for (auto& go : m_GameObjects) {
		go->Update(deltaTime);
	}
}

void test::TestHorrorMaze::OnRender()
{
	BeginScene();
	m_LightingShader->Bind();

	m_LightingShader->SetUniformMat4f("u_View", m_CameraController.GetCamera().GetViewMatrix());
	m_LightingShader->SetUniformMat4f("u_Projection", m_CameraController.GetCamera().GetProjectionMatrix());

	m_CameraController.OnUpdate(MyTime::GetDeltaTime());

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
		m_Camera.SetAspectRatio(m_ViewportSize.x / m_ViewportSize.y);
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