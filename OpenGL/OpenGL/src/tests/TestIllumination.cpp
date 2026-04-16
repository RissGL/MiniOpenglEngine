#include "tests/TestIllumination.h"
#include "VertexBufferLayout.h"
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
		m_LightColor(1.0f, 1.0f, 1.0f),     // 纯白光
		m_ObjectColor(1.0f, 0.5f, 0.31f)    // 珊瑚红物体
	{
		// 包含法线的 24 顶点标准立方体 (位置 x,y,z + 法线 nx,ny,nz)
		float vertices[] = {
			// 前面 (法线 0,0,1)
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // 0
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // 1
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // 2
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // 3
			// 后面 (法线 0,0,-1)
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // 4
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // 5
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // 6
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // 7
			// 左面 (法线 -1,0,0)
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, // 8
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, // 9
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, // 10
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, // 11
			// 右面 (法线 1,0,0)
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, // 12
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // 13
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // 14
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, // 15
			 // 上面 (法线 0,1,0)
			 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, // 16
			  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, // 17
			  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, // 18
			 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, // 19
			 // 下面 (法线 0,-1,0)
			 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, // 20
			  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, // 21
			  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, // 22
			 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f  // 23
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
		m_Vbo = std::make_unique<VertexBuffer>(vertices, 24 * 6 * sizeof(float));
		m_Ibo = std::make_unique<IndexBuffer>(indices, 36);

		VertexBufferLayout layout;
		layout.Push<float>(3); // 位置
		layout.Push<float>(3); // 改为法线，不再是纹理坐标了
		m_Vao->AddBuffer(*m_Vbo, layout);

		m_LightVao = std::make_unique<VertexArray>();
		m_LightVao->AddBuffer(*m_Vbo, layout);

		m_LightingShader = std::make_unique<Shader>("src/res/shaders/LightingShader.shader");
		m_LightCubeShader = std::make_unique<Shader>("src/res/shaders/LightCube.shader");
	}

	TestIllumination::~TestIllumination() {}
	void TestIllumination::OnUpdate(float deltaTime) {}

	void TestIllumination::OnRender()
	{
		GLCALL(glViewport(0, 0, MyWindow::GetWidth(), MyWindow::GetHeight()));
		GLCALL(glEnable(GL_DEPTH_TEST));
		GLCALL(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		// 相机与鼠标控制
		if (Input::IsKeyPressed(GLFW_KEY_W)) m_Camera.MoveCamera(cameraSpeed * m_Camera.GetCameraFront() * MyTime::GetDeltaTime());
		if (Input::IsKeyPressed(GLFW_KEY_S)) m_Camera.MoveCamera(-cameraSpeed * m_Camera.GetCameraFront() * MyTime::GetDeltaTime());
		if (Input::IsKeyPressed(GLFW_KEY_D)) m_Camera.MoveCamera(-glm::normalize(glm::cross(m_Camera.GetCameraUp(), m_Camera.GetCameraFront())) * cameraSpeed * MyTime::GetDeltaTime());
		if (Input::IsKeyPressed(GLFW_KEY_A)) m_Camera.MoveCamera(glm::normalize(glm::cross(m_Camera.GetCameraUp(), m_Camera.GetCameraFront())) * cameraSpeed * MyTime::GetDeltaTime());

		static bool firstMouse = true;
		static float lastX = MyWindow::GetWidth() / 2.0f;
		static float lastY = MyWindow::GetHeight() / 2.0f;

		float currentX = Input::GetMouseX();
		float currentY = Input::GetMouseY();

		if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
			if (firstMouse) { lastX = currentX; lastY = currentY; firstMouse = false; }
			float xoffset = currentX - lastX; float yoffset = lastY - currentY;
			lastX = currentX; lastY = currentY;
			if (xoffset != 0.0f || yoffset != 0.0f) m_Camera.ProcessMouseMovement(xoffset, yoffset);
		}
		else { firstMouse = true; }

		float scrollY = Input::GetScrollY();
		if (scrollY != 0) m_Camera.ProcessMouseScroll(scrollY);

		Renderer renderer;

		// 渲染被光照的物体 ---
		m_LightingShader->Bind();
		m_LightingShader->SetUniformMat4f("u_View", m_Camera.GetViewMatrix());
		m_LightingShader->SetUniformMat4f("u_Projection", m_Camera.GetProjectionMatrix());

		// 传递光照参数给 Shader
		m_LightingShader->SetUniform3f("u_LightPos", m_LightPos.x, m_LightPos.y, m_LightPos.z);
		m_LightingShader->SetUniform3f("u_ViewPos", m_Camera.GetCameraPos().x, m_Camera.GetCameraPos().y, m_Camera.GetCameraPos().z);
		m_LightingShader->SetUniform3f("u_LightColor", m_LightColor.x * m_LightIntensity, m_LightColor.y * m_LightIntensity, m_LightColor.z * m_LightIntensity);
		m_LightingShader->SetUniform3f("u_ObjectColor", m_ObjectColor.x, m_ObjectColor.y, m_ObjectColor.z);

		glm::mat4 model= glm::translate(glm::mat4(1.0f), m_Translation);
		model = glm::scale(model, m_Scare);
		m_LightingShader->SetUniformMat4f("u_Model", model);

		renderer.Draw(*m_Vao, *m_Ibo, *m_LightingShader);

		m_LightCubeShader->Bind();
		m_LightCubeShader->SetUniformMat4f("u_View", m_Camera.GetViewMatrix());
		m_LightCubeShader->SetUniformMat4f("u_Projection", m_Camera.GetProjectionMatrix());
		m_LightCubeShader->SetUniform3f("u_LightColor", m_LightColor.x, m_LightColor.y, m_LightColor.z);
		m_LightCubeShader->SetUniform1f("u_LightIntensity", m_LightIntensity);

		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, m_LightPos); // 移动到光源位置
		lightModel = glm::scale(lightModel, glm::vec3(0.2f)); // 缩小一点
		m_LightCubeShader->SetUniformMat4f("u_Model", lightModel);

		renderer.Draw(*m_LightVao, *m_Ibo, *m_LightCubeShader);

		m_Recorder.CaptureFrame(MyWindow::GetWidth(), MyWindow::GetHeight());
	}

	void TestIllumination::OnImGuiRender()
	{
		ImGui::Text("Light Settings");
		ImGui::SliderFloat3("Light Position", &m_LightPos.x, -5.0f, 5.0f);
		ImGui::ColorEdit3("Light Color", &m_LightColor.x);
		ImGui::SliderFloat("Light Intensity", &m_LightIntensity,0.0f,1.0f);

		ImGui::Separator();
		ImGui::Text("Object Settings");
		ImGui::SliderFloat3("Object Translation", &m_Translation.x, -5.0f, 5.0f);
		ImGui::SliderFloat3("Object Scale", &m_Scare.x, 0.1f, 5.0f);
		ImGui::ColorEdit3("Object Color", &m_ObjectColor.x);

		ImGui::Separator();
		ImGui::ColorEdit4("Ambient Environment Color", m_ClearColor);
	}
}