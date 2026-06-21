#include "tests/TestIllumination.h"
#include "Base/VertexBufferLayout.h"
#include "imgui/imgui.h"
#include "GLFW/glfw3.h"
#include "Input.h"
#include "MyTime.h"
#include "Window/MyWindow.h"
#include "Core/LightComponents.h"

namespace test
{
	TestIllumination::TestIllumination()
		: m_ClearColor{ 0.05f, 0.05f, 0.05f, 1.0f },
		m_Camera(glm::vec3(0.0f, 1.5f, 6.0f)),
		cameraSpeed(2.0f),
		m_LightPos(0.0f, 2.0f, 0.0f),
		m_CameraController(m_Camera)
	{
		m_LightingShader = std::make_unique<Shader>("src/res/shaders/LightingShader.vert",
			"src/res/shaders/LightingShader.frag");
		m_LightCubeShader = std::make_unique<Shader>("src/res/shaders/LightCube.vert",
			"src/res/shaders/LightCube.frag");

		m_DiffuseMap = std::make_shared<Texture>("src/res/texture/container2.png");
		m_SpecularMap = std::make_shared<Texture>("src/res/texture/container2_specular.png");

#pragma region 光照
		m_Material = { m_DiffuseMap, m_SpecularMap, m_EmissionMap, 32.0f };

		m_SunLight = std::make_shared<DirLight>(
			glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.1f, 0.1f, 0.2f), 1.0f);

		m_PointLights.push_back(std::make_shared<PointLight>(
			m_LightPos, glm::vec3(1.0f, 0.0014f, 0.000007f), glm::vec3(1.0f, 0.8f, 0.6f), 1.2f));
		m_PointLights.push_back(std::make_shared<PointLight>(
			glm::vec3(2.0f, 2.0f, 0.0f), glm::vec3(1.0f, 0.045f, 0.0075f), glm::vec3(0.2f, 0.5f, 1.0f), 0.8f));

		m_SpotLight = std::make_shared<SpotLight>(
			glm::vec3(0.0f), glm::vec3(1.0f, 0.0014f, 0.000007f), glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(1.0f), 12.5f, 25.0f, 1.0f);

		auto pl0 = m_Scene.AddGameObject("Point Light 0");
		pl0->transform->localPosition = m_LightPos;
		pl0->AddComponent<PointLightComponent>(m_PointLights[0]);

		auto pl1 = m_Scene.AddGameObject("Point Light 1");
		pl1->transform->localPosition = glm::vec3(2.0f, 2.0f, 0.0f);
		pl1->AddComponent<PointLightComponent>(m_PointLights[1]);

		auto sunObj = m_Scene.AddGameObject("Directional Light");
		sunObj->transform->localEulerAngles = glm::vec3(-45.0f, -30.0f, 0.0f);
		sunObj->AddComponent<DirLightComponent>(m_SunLight);

		auto spotObj = m_Scene.AddGameObject("SpotLight");
		spotObj->AddComponent<SpotLightComponent>(m_SpotLight);
#pragma endregion

#pragma region 地板
		float half = 5.0f;
		struct Vertex {
			glm::vec3 pos;
			glm::vec3 normal;
			glm::vec2 texCoord;
		};
		Vertex floorVertices[] = {
			{{-half, 0.0f, -half}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
			{{ half, 0.0f, -half}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
			{{ half, 0.0f,  half}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
			{{-half, 0.0f, -half}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
			{{ half, 0.0f,  half}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
			{{-half, 0.0f,  half}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}
		};

		m_BoxVAO = std::make_unique<VertexArray>();
		m_BoxVBO = std::make_unique<VertexBuffer>(floorVertices, sizeof(floorVertices));

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_BoxVAO->AddBuffer(*m_BoxVBO, layout);
#pragma endregion

#pragma region 光源小球
	{
		const int latSegs = 12, lonSegs = 16;
		std::vector<float> sphereVerts;
		std::vector<unsigned int> sphereIndices;

		for (int lat = 0; lat <= latSegs; lat++) {
			float theta = lat * glm::pi<float>() / latSegs;
			float sinT = sin(theta), cosT = cos(theta);
			for (int lon = 0; lon <= lonSegs; lon++) {
				float phi = lon * 2.0f * glm::pi<float>() / lonSegs;
				float sinP = sin(phi), cosP = cos(phi);
				sphereVerts.push_back(sinT * cosP);
				sphereVerts.push_back(cosT);
				sphereVerts.push_back(sinT * sinP);
			}
		}
		for (int lat = 0; lat < latSegs; lat++) {
			for (int lon = 0; lon < lonSegs; lon++) {
				unsigned int a = lat * (lonSegs + 1) + lon;
				unsigned int b = a + lonSegs + 1;
				sphereIndices.push_back(a);
				sphereIndices.push_back(b);
				sphereIndices.push_back(a + 1);
				sphereIndices.push_back(a + 1);
				sphereIndices.push_back(b);
				sphereIndices.push_back(b + 1);
			}
		}
		m_SphereIndexCount = sphereIndices.size();

		m_SphereVAO = std::make_unique<VertexArray>();
		m_SphereVBO = std::make_unique<VertexBuffer>(sphereVerts.data(), sphereVerts.size() * sizeof(float));
		m_SphereIBO = std::make_unique<IndexBuffer>(sphereIndices.data(), sphereIndices.size());

		VertexBufferLayout sphereLayout;
		sphereLayout.Push<float>(3);
		m_SphereVAO->AddBuffer(*m_SphereVBO, sphereLayout);
	}
#pragma endregion

		m_SharedModel = std::make_shared<Model>("src/res/models/Characters.fbx", "Character_Dummy_Male_01");

		auto obj1 = m_Scene.AddGameObject("Demonstrator 1");
		obj1->transform->localPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		obj1->transform->localScale = glm::vec3(1.0f);
		obj1->AddComponent<MeshRenderer>(m_SharedModel);

		auto obj2 = m_Scene.AddGameObject("Demonstrator 2");
		obj2->transform->localPosition = glm::vec3(2.0f, 0.0f, 0.0f);
		obj2->transform->localScale = glm::vec3(1.0f);
		obj2->AddComponent<MeshRenderer>(m_SharedModel);

		m_Scene.Awake();
		m_Scene.Start();

		m_HierarchyPanel = std::make_unique<SceneHierarchyPanel>();
		m_HierarchyPanel->SetContext(&m_Scene.GetGameObjects());

		m_Framebuffer = std::make_unique<Framebuffer>(MyWindow::GetWidth(), MyWindow::GetHeight());

		m_Skybox = std::make_unique<Skybox>();
	}

	TestIllumination::~TestIllumination() {}

	void TestIllumination::OnUpdate(float deltaTime)
	{
		m_Scene.Update(deltaTime);
	}

	void TestIllumination::OnRender()
	{
		BeginScene();
		m_CameraController.OnUpdate(MyTime::GetDeltaTime());

		m_LightingShader->Bind();
		m_LightingShader->SetUniformMat4f("u_View", m_Camera.GetViewMatrix());
		m_LightingShader->SetUniformMat4f("u_Projection", m_Camera.GetProjectionMatrix());
		m_LightingShader->SetUniform3f("u_ViewPos", m_Camera.GetCameraPos().x, m_Camera.GetCameraPos().y, m_Camera.GetCameraPos().z);
		m_LightingShader->SetUniformMaterial("u_Material", m_Material);

		m_SunLight->BindToShader(*m_LightingShader, "u_DirLight");

		for (size_t i = 0; i < m_PointLights.size(); i++) {
			std::string uniformName = "u_PointLights[" + std::to_string(i) + "]";
			m_PointLights[i]->BindToShader(*m_LightingShader, uniformName);
		}
		m_LightingShader->SetUniform1i("u_NumPointLights", (int)m_PointLights.size());

		m_SpotLight->BindToShader(*m_LightingShader, "u_SpotLight");

		if (m_Material.mapDiffuse != nullptr) m_Material.mapDiffuse->Bind(0);
		if (m_Material.specularMap != nullptr) m_Material.specularMap->Bind(1);

#pragma region 地板
		glm::mat4 floorModel = glm::mat4(1.0f);
		m_LightingShader->SetUniformMat4f("u_Model", floorModel);
		m_BoxVAO->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
#pragma endregion

		m_Scene.Draw(*m_LightingShader);

		m_Skybox->Draw(m_Camera.GetViewMatrix(), m_Camera.GetProjectionMatrix());


		// 光源小球标记
		m_LightCubeShader->Bind();
		m_LightCubeShader->SetUniformMat4f("u_View", m_Camera.GetViewMatrix());
		m_LightCubeShader->SetUniformMat4f("u_Projection", m_Camera.GetProjectionMatrix());
		m_SphereVAO->Bind();
		for (auto& pl : m_PointLights) {
			m_LightCubeShader->SetUniform3f("u_LightColor", pl->baseColor.x, pl->baseColor.y, pl->baseColor.z);
			m_LightCubeShader->SetUniform1f("u_LightIntensity", pl->intensity);
			glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), pl->position);
			lightModel = glm::scale(lightModel, glm::vec3(0.2f));
			m_LightCubeShader->SetUniformMat4f("u_Model", lightModel);
			GLCALL(glDrawElements(GL_TRIANGLES, m_SphereIndexCount, GL_UNSIGNED_INT, nullptr));
		}

		EndScene();
		m_Recorder.CaptureFrame(MyWindow::GetWidth(), MyWindow::GetHeight());
	}

	void TestIllumination::OnImGuiRender()
	{
		m_HierarchyPanel->OnImGuiRender();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Game Viewport");

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = glm::vec2(viewportPanelSize.x, viewportPanelSize.y);

		uint32_t textureID = m_Framebuffer->GetColorAttachment();
		ImGui::Image((void*)(intptr_t)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("Shader Tools");
		if (ImGui::Button("Reload Lighting Shader")) {
			m_LightingShader->Reload();
		}
		ImGui::End();
	}

	void TestIllumination::BeginScene() {
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(m_Framebuffer->GetWidth() != m_ViewportSize.x || m_Framebuffer->GetHeight() != m_ViewportSize.y)) {
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_Camera.SetAspectRatio(m_ViewportSize.x / m_ViewportSize.y);
		}
		m_Framebuffer->Bind();
		GLCALL(glEnable(GL_DEPTH_TEST));
		GLCALL(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}
	void TestIllumination::EndScene() { m_Framebuffer->UnBind(); }
}
