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
		layout.Push<float>(3); // 改为法线，不再是纹理坐标了
		layout.Push<float>(2);
		m_Vao->AddBuffer(*m_Vbo, layout);

		m_LightVao = std::make_unique<VertexArray>();
		m_LightVao->AddBuffer(*m_Vbo, layout);

		m_LightingShader = std::make_unique<Shader>("src/res/shaders/LightingShader.shader");
		m_LightCubeShader = std::make_unique<Shader>("src/res/shaders/LightCube.shader");

		m_DiffuseMap = std::make_unique<Texture>("src/res/texture/container2.png");
		m_SpecularMap=std::make_unique<Texture>("src/res/texture/container2_specular.png");
		m_EmissionMap = std::make_unique<Texture>("src/res/texture/matrix.jpg");


		m_Material =
		{
			m_DiffuseMap.get(),
			m_SpecularMap.get(),         // 镜面反射 
			m_EmissionMap.get(),

			32.0f                     // 反光度
		};

		m_Light = {
			m_LightPos,                            // 位置
			glm::vec3(1.0f, 1.0f, 1.0f),
			0.5f,
			glm::vec3(1.0f, 1.0f, 1.0f) * 0.2f,       // 环境光
			glm::vec3(1.0f, 1.0f, 1.0f) * 0.5f,       // 漫反射
			glm::vec3(1.0f, 1.0f, 1.0f) * 0.5f        // 镜面反射
		};
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
		m_Light.Update();


		GLCALL(glViewport(0, 0, MyWindow::GetWidth(), MyWindow::GetHeight()));
		GLCALL(glEnable(GL_DEPTH_TEST));
		GLCALL(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_CameraController.OnUpdate();
		Renderer renderer;

		// 渲染被光照的物体 ---
		m_LightingShader->Bind();
		m_LightingShader->SetUniformMat4f("u_View", m_Camera.GetViewMatrix());
		m_LightingShader->SetUniformMat4f("u_Projection", m_Camera.GetProjectionMatrix());

		// 传光照参数给 Shader
		m_LightingShader->SetUniform3f("u_ViewPos", m_Camera.GetCameraPos().x, m_Camera.GetCameraPos().y, m_Camera.GetCameraPos().z);
		m_LightingShader->SetUniformLight("u_Light",m_Light);
		//传材质
		m_LightingShader->SetUniformMaterial("u_Material",m_Material);

		if (m_Material.mapDiffuse != nullptr)
		{
			m_Material.mapDiffuse->Bind(0);
		}

		if (m_Material.specularMap!=nullptr)
		{
			m_Material.specularMap->Bind(1);
		}

		glm::mat4 model= glm::translate(glm::mat4(1.0f), m_Translation);
		model = glm::scale(model, m_Scare);
		m_LightingShader->SetUniformMat4f("u_Model", model);

		renderer.Draw(*m_Vao, *m_Ibo, *m_LightingShader);

		m_LightCubeShader->Bind();
		m_LightCubeShader->SetUniformMat4f("u_View", m_Camera.GetViewMatrix());
		m_LightCubeShader->SetUniformMat4f("u_Projection", m_Camera.GetProjectionMatrix());
		m_LightCubeShader->SetUniform3f("u_LightColor", m_Light.baseColor.x, m_Light.baseColor.y, m_Light.baseColor.z);
		m_LightCubeShader->SetUniform1f("u_LightIntensity", m_Light.intensity);

		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, m_Light.lightPos); // 移动到光源位置
		lightModel = glm::scale(lightModel, glm::vec3(0.2f)); // 缩小一点
		m_LightCubeShader->SetUniformMat4f("u_Model", lightModel);

		renderer.Draw(*m_LightVao, *m_Ibo, *m_LightCubeShader);

		m_Recorder.CaptureFrame(MyWindow::GetWidth(), MyWindow::GetHeight());
	}

	void TestIllumination::OnImGuiRender()
	{
		ImGui::Text("Light Settings");
		ImGui::SliderFloat3("Light Position", &m_Light.lightPos.x, -5.0f, 5.0f);
		ImGui::ColorEdit3("Light Color", &m_Light.baseColor.x);
		ImGui::SliderFloat("Light Intensity", &m_Light.intensity, 0.0f, 2.0f);

		ImGui::Separator();
		ImGui::Text("Object Settings");
		ImGui::SliderFloat3("Object Translation", &m_Translation.x, -5.0f, 5.0f);
		ImGui::SliderFloat3("Object Scale", &m_Scare.x, 0.1f, 5.0f);

		ImGui::Text("Material Settings");

		ImGui::SliderFloat("Shininess", &m_Material.shininess, 2.0f, 256.0f);

		ImGui::Separator();
		ImGui::ColorEdit4("Ambient Environment Color", m_ClearColor);
	}
}