#include "tests/TestIllumination.h"
#include "Base/VertexBufferLayout.h"
#include "imgui/imgui.h"
#include "GLFW/glfw3.h"
#include "Input.h"
#include "MyTime.h"
#include "Window/MyWindow.h"

namespace test
{
	TestIllumination::TestIllumination()
		: m_Translation(0.0f, 0.0f, 0.0f),
		m_Scare(1.0f, 1.0f, 1.0f),
		m_ClearColor{ 0.1f, 0.1f, 0.1f, 1.0f }, // 光照测试环境光暗一点
		m_Camera(glm::vec3(0.0f, 0.0f, 6.0f)),
		cameraSpeed(2.0f),
		m_LightPos(1.2f, 1.0f, 2.0f),       // 初始光源位置
		m_CameraController(m_Camera)
	{
		/*
		// 包含法线的 24 顶点标准立方体 (位置 x,y,z + 法线 nx,ny,nz)
		float vertices[] = {
			// 前面 (法线 0,0,1)
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, // 左下
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, // 右下
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, // 右上
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, // 左上
			// 后面 (法线 0,0,-1)
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			// 左面 (法线 -1,0,0)
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			// 右面 (法线 1,0,0)
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			 // 上面 (法线 0,1,0)
			 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
			  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			 // 下面 (法线 0,-1,0)
			 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f
		};

		unsigned int indices[] = {
			 0,  1,  2,  2,  3,  0,
			 5,  4,  7,  7,  6,  5,
			 8,  9, 10, 10, 11,  8,
			13, 12, 15, 15, 14, 13,
			16, 17, 18, 18, 19, 16,
			21, 20, 23, 23, 22, 21
		};

		m_Vao = std::make_unique<VertexArray>();
		m_Vbo = std::make_unique<VertexBuffer>(vertices, 24 * 8 * sizeof(float));
		m_Ibo = std::make_unique<IndexBuffer>(indices, 36);

		VertexBufferLayout layout;
		layout.Push<float>(3); // 位置
		layout.Push<float>(3); // 改为法线
		layout.Push<float>(2);
		m_Vao->AddBuffer(*m_Vbo, layout);

		m_LightVao = std::make_unique<VertexArray>();
		m_LightVao->AddBuffer(*m_Vbo, layout);*/

		m_LightingShader = std::make_unique<Shader>("src/res/shaders/LightingShader.vert",
			"src/res/shaders/LightingShader.frag");
		m_LightCubeShader = std::make_unique<Shader>("src/res/shaders/LightCube.vert",
			"src/res/shaders/LightCube.frag");

		m_DiffuseMap = std::make_unique<Texture>("src/res/texture/container2.png");
		m_SpecularMap=std::make_unique<Texture>("src/res/texture/container2_specular.png");
		m_EmissionMap = std::make_unique<Texture>("src/res/texture/matrix.jpg");

#pragma region 光照申明
		m_Material =
		{
			m_DiffuseMap.get(),
			m_SpecularMap.get(),         // 镜面反射 
			m_EmissionMap.get(),

			32.0f                     // 反光度
		};

		m_SunLight = std::make_unique<DirLight>
			(
				glm::vec3(-0.2f, -1.0f, -0.3f), // 方向
				glm::vec3(1.0f, 1.0f, 1.0f),    // 颜色 (白光)
				0.5f                            // 强度
			);

		m_PointLight = std::make_unique<PointLight>
			(
				m_LightPos,
				glm::vec3(1.0f, 0.09f, 0.032f),

				glm::vec3(1.0f, 1.0f, 1.0f),    // 颜色 (白光)
				0.5f                            // 强度
			);

		m_SpotLight = std::make_unique<SpotLight>
			(
				glm::vec3(0.0f), 
				glm::vec3(1.0f, 0.045f, 0.0075f),
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::vec3(1.0f),
				12.5f, 
				25.0f, 
				1.0f
			);
#pragma endregion

		cubePositions = {
glm::vec3(0.0f,  0.0f,  0.0f),
glm::vec3(1.0f,  3.0f, -3.0f),
glm::vec3(-1.5f, -2.2f, -2.5f),
glm::vec3(-3.8f, -2.0f, -12.3f),
glm::vec3(2.4f, -0.4f, -3.5f),
glm::vec3(-1.7f,  3.0f, -7.5f),
glm::vec3(1.3f, -2.0f, -2.5f),
glm::vec3(1.5f,  2.0f, -2.5f),
glm::vec3(1.5f,  0.2f, -1.5f),
glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		std::string rootName = "Scene Root";
		m_RootNode = std::make_unique<BaseNode>(0, rootName);

		// 创建光源节点，并传指针
		std::string lightName = "Torch Light";
		PointLightNode* lightNode = new PointLightNode(1, lightName, m_PointLight.get());
		std::string lightDirName = "Dir Light";
		DirLightNode* lightNodeDir= new DirLightNode(2, lightDirName, m_SunLight.get());
		std::string lightSpotName = "SpotLight";
		SpotLightNode* spotLightNode = new SpotLightNode(3, lightSpotName, m_SpotLight.get());

		// 灯光节点挂载到根节点下
		m_RootNode->AddChild(lightNode);
		m_RootNode->AddChild(lightNodeDir);
		m_RootNode->AddChild(spotLightNode);

		// 创建面板
		m_HierarchyPanel = std::make_unique<SceneHierarchyPanel>(m_RootNode.get());

		m_Framebuffer = std::make_unique<Framebuffer>(MyWindow::GetWidth(), MyWindow::GetHeight());

		m_MyModel = std::make_unique<Mesh>("src/res/models/HatsuneMikuNT.fbx");
	}

	TestIllumination::~TestIllumination() {}
	void TestIllumination::OnUpdate(float deltaTime) {}

	void TestIllumination::OnRender()
	{
		/*float speed = 1.5f;
		float raidus = 1.0f;
		float time = MyTime::GetTime();

		m_Light.lightPos.x = sin(time*speed)*raidus;
		m_Light.lightPos.z= cos(time*speed)*raidus;
		*/
		// FBO
		BeginScene();

		m_CameraController.OnUpdate(MyTime::GetDeltaTime());
		Renderer renderer;

		// 渲染被光照的物体 ---
		m_LightingShader->Bind();
		m_LightingShader->SetUniformMat4f("u_View", m_Camera.GetViewMatrix());
		m_LightingShader->SetUniformMat4f("u_Projection", m_Camera.GetProjectionMatrix());

		// 传光照参数给 Shader
		m_LightingShader->SetUniform3f("u_ViewPos", m_Camera.GetCameraPos().x, m_Camera.GetCameraPos().y, m_Camera.GetCameraPos().z);
		//m_LightingShader->SetUniformLightDirectional("u_Light", m_Light);
		//传材质
		m_LightingShader->SetUniformMaterial("u_Material", m_Material);

		m_SunLight->BindToShader(*m_LightingShader,"u_DirLight");
		m_PointLight->BindToShader(*m_LightingShader, "u_PointLight");
		m_SpotLight->BindToShader(*m_LightingShader, "u_SpotLight");

		if (m_Material.mapDiffuse != nullptr)
		{
			m_Material.mapDiffuse->Bind(0);
		}

		if (m_Material.specularMap != nullptr)
		{
			m_Material.specularMap->Bind(1);
		}


		/*for (unsigned i = 0; i < 10; i++)
		{

			float angle = 20.0f * i;

			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
			model = glm::scale(model, m_Scare);

			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 0, 1));

			m_LightingShader->SetUniformMat4f("u_Model", model);

			renderer.Draw(*m_Vao, *m_Ibo, *m_LightingShader);
		}*/
		
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
		model = glm::scale(model, m_Scare);
		m_LightingShader->SetUniformMat4f("u_Model", model);

		// 见证奇迹的时刻：一句话画出几万个顶点的复杂模型！
		m_MyModel->Draw(*m_LightingShader);

		m_LightCubeShader->Bind();
		m_LightCubeShader->SetUniformMat4f("u_View", m_Camera.GetViewMatrix());
		m_LightCubeShader->SetUniformMat4f("u_Projection", m_Camera.GetProjectionMatrix());
		m_LightCubeShader->SetUniform3f("u_LightColor", m_PointLight->baseColor.x, m_PointLight->baseColor.y, m_PointLight->baseColor.z);
		m_LightCubeShader->SetUniform1f("u_LightIntensity", m_PointLight->intensity);

		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel,m_PointLight->position); // 移动到光源位置
		lightModel = glm::scale(lightModel, glm::vec3(0.2f)); // 缩小一点
		m_LightCubeShader->SetUniformMat4f("u_Model", lightModel);

		//renderer.Draw(*m_LightVao, *m_Ibo, *m_LightCubeShader);

		EndScene();

		m_Recorder.CaptureFrame(MyWindow::GetWidth(), MyWindow::GetHeight());
	}

	void TestIllumination::OnImGuiRender()
	{
		m_HierarchyPanel->OnImGuiRender();

		ImGui::Separator();
		ImGui::Separator();

		// --- 物体和环境控制 ---
		ImGui::Text("Object Settings");
		ImGui::SliderFloat3("Object Translation", &m_Translation.x, -5.0f, 5.0f);
		ImGui::SliderFloat3("Object Scale", &m_Scare.x, 0.1f, 5.0f);

		ImGui::Text("Material Settings");
		ImGui::SliderFloat("Shininess", &m_Material.shininess, 2.0f, 256.0f);

		ImGui::Separator();
		ImGui::ColorEdit4("Ambient Environment Color", m_ClearColor);


		//自适应大小
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Scene");

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = glm::vec2(viewportPanelSize.x, viewportPanelSize.y);

		uint32_t textureID = m_Framebuffer->GetColorAttachment();
		ImGui::Image((void*)(intptr_t)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}

	/// <summary>
	/// 开始场景
	/// </summary>
	void TestIllumination::BeginScene()
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
		GLCALL(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	/// <summary>
	/// 结束场景
	/// </summary>
	void TestIllumination::EndScene()
	{
		m_Framebuffer->UnBind();
	}
}