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

		m_DiffuseMap = std::make_unique<Texture>("src/res/texture/container2.png");
		m_SpecularMap = std::make_unique<Texture>("src/res/texture/container2_specular.png");
		//m_EmissionMap = std::make_unique<Texture>("src/res/texture/matrix.jpg");

#pragma region 光照申明
		m_Material = { m_DiffuseMap.get(), m_SpecularMap.get(), m_EmissionMap.get(), 32.0f };

		m_SunLight = std::make_shared<DirLight>
			(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.1f, 0.1f, 0.2f), 1.0f);

		m_PointLight = std::make_shared<PointLight>
			(m_LightPos, glm::vec3(1.0f, 0.0014f, 0.000007f), glm::vec3(1.0f, 1.0f, 1.0f), 0.5f);

		m_SpotLight = std::make_shared<SpotLight>
			(glm::vec3(0.0f), glm::vec3(1.0f, 0.0014f, 0.000007f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f), 12.5f, 25.0f, 1.0f);

		auto pointLightObj = std::make_shared<GameObject>("Point Light");
		pointLightObj->transform->localPosition = m_LightPos; 
		pointLightObj->AddComponent<PointLightComponent>(m_PointLight); 
		m_GameObjects.push_back(pointLightObj);

		auto sunObj = std::make_shared<GameObject>("Directional Light");
		sunObj->transform->localEulerAngles = glm::vec3(-45.0f, -30.0f, 0.0f);
		sunObj->AddComponent<DirLightComponent>(m_SunLight);
		m_GameObjects.push_back(sunObj);


		auto spotObj = std::make_shared<GameObject>("SpotLight");
		//spotObj->transform->localEulerAngles = glm::vec3(0.0f);
		spotObj->AddComponent<SpotLightComponent>(m_SpotLight);
		m_GameObjects.push_back(spotObj);
#pragma endregion


#pragma region 地板
		float half = 5.0f;
		struct Vertex {
			glm::vec3 pos;
			glm::vec3 normal;
			glm::vec2 texCoord;
		};
		Vertex floorVertices[] = {
			// 三角形1
			{{-half, 0.0f, -half}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
			{{ half, 0.0f, -half}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
			{{ half, 0.0f,  half}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
			// 三角形2
			{{-half, 0.0f, -half}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
			{{ half, 0.0f,  half}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
			{{-half, 0.0f,  half}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}
		};

		m_BoxVAO = std::make_unique<VertexArray>();
		m_BoxVBO = std::make_unique<VertexBuffer>(floorVertices, sizeof(floorVertices));

		VertexBufferLayout layout;
		layout.Push<float>(3); // pos
		layout.Push<float>(3); // normal
		layout.Push<float>(2); // texCoords
		m_BoxVAO->AddBuffer(*m_BoxVBO, layout);
#pragma endregion

		m_SharedModel = std::make_shared<Model>("src/res/models/Characters.fbx", "Character_Dummy_Male_01");


		auto obj1 = std::make_shared<GameObject>("Demonstrator 1");
		obj1->transform->localPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		obj1->transform->localScale = glm::vec3(1.0f);
		obj1->AddComponent<MeshRenderer>(m_SharedModel);
		m_GameObjects.push_back(obj1);

		auto obj2 = std::make_shared<GameObject>("Demonstrator 2");
		obj2->transform->localPosition = glm::vec3(2.0f, 0.0f, 0.0f);
		obj2->transform->localScale = glm::vec3(1.0f);
		obj2->AddComponent<MeshRenderer>(m_SharedModel);
		m_GameObjects.push_back(obj2);

		for (auto& go : m_GameObjects) { go->Awake(); go->Start(); }

		m_HierarchyPanel = std::make_unique<SceneHierarchyPanel>();
		m_HierarchyPanel->SetContext(&m_GameObjects);

		m_Framebuffer = std::make_unique<Framebuffer>(MyWindow::GetWidth(), MyWindow::GetHeight());
	}

	TestIllumination::~TestIllumination() {}

	void TestIllumination::OnUpdate(float deltaTime)
	{
		for (auto& go : m_GameObjects) { go->Update(deltaTime); }
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
		m_PointLight->BindToShader(*m_LightingShader, "u_PointLight");

		//m_SpotLight->position = m_Camera.GetCameraPos();
		//m_SpotLight->direction = m_Camera.GetCameraFront();
		m_SpotLight->BindToShader(*m_LightingShader, "u_SpotLight");

		if (m_Material.mapDiffuse != nullptr) m_Material.mapDiffuse->Bind(0);
		if (m_Material.specularMap != nullptr) m_Material.specularMap->Bind(1);

#pragma region 地板
		glm::mat4 floorModel = glm::mat4(1.0f);   // 可以根据需要平移
		m_LightingShader->SetUniformMat4f("u_Model", floorModel);
		m_BoxVAO->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
#pragma endregion


		for (auto& go : m_GameObjects)
		{
			go->Draw(*m_LightingShader);
		}

		// 画光源提示块
		m_LightCubeShader->Bind();
		m_LightCubeShader->SetUniformMat4f("u_View", m_Camera.GetViewMatrix());
		m_LightCubeShader->SetUniformMat4f("u_Projection", m_Camera.GetProjectionMatrix());
		m_LightCubeShader->SetUniform3f("u_LightColor", m_PointLight->baseColor.x, m_PointLight->baseColor.y, m_PointLight->baseColor.z);
		m_LightCubeShader->SetUniform1f("u_LightIntensity", m_PointLight->intensity);

		glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), m_PointLight->position);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		m_LightCubeShader->SetUniformMat4f("u_Model", lightModel);

		EndScene();
		m_Recorder.CaptureFrame(MyWindow::GetWidth(), MyWindow::GetHeight());
	}

	void TestIllumination::OnImGuiRender()
	{
		m_HierarchyPanel->OnImGuiRender();

		// 自适应视口
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Game Viewport");

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = glm::vec2(viewportPanelSize.x, viewportPanelSize.y);

		uint32_t textureID = m_Framebuffer->GetColorAttachment();
		ImGui::Image((void*)(intptr_t)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void TestIllumination::BeginScene() {
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (m_Framebuffer->GetWidth() != m_ViewportSize.x || m_Framebuffer->GetHeight() != m_ViewportSize.y)) {
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_Camera.SetAspectRatio(m_ViewportSize.x / m_ViewportSize.y);
		}
		m_Framebuffer->Bind();
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCALL(glEnable(GL_DEPTH_TEST));
		GLCALL(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}
	void TestIllumination::EndScene() { m_Framebuffer->UnBind(); }
}