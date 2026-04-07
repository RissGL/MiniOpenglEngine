#include "tests/TestTexture3D.h"
#include "VertexBufferLayout.h"
#include "imgui/imgui.h"

#include "GLFW/glfw3.h"

namespace test
{
	TestTexture3D::TestTexture3D()
		:m_Translation(0.0f, 0.0f, 0.0f),
		m_Scare(1.0f, 1.0f, 1.0f),
		m_Proj(glm::perspective(glm::radians(45.0f),1920.0f/1080.0f,0.1f,100.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -6))),
		m_Model(glm::rotate(glm::mat4(1.0f),glm::radians(0.0f),glm::vec3(1.0f,0,0))),
		m_ClearColor{ 0.2f,0.3f,0.8f,1.0f },
		m_Camera(glm::vec3(0, 0, -6), glm::vec3(0, 0, -1))
	{
		float position[] = {
	-5.0f, -5.0f,10.0f, 0.0f,0.0f,    //1.0f, 0.0f, 0.0f, 1.0f,
	-5.0f,  5.0f,10.0f, 0.0f,1.0f,   //0.0f, 1.0f, 0.0f, 1.0f,
	 5.0f,  5.0f,10.0f, 1.0f,1.0f,    //0.0f, 0.0f, 1.0f, 1.0f,
	 5.0f, -5.0f,10.0f, 1.0f,0.0f    //0.0f, 0.0f, 1.0f, 1.0f,
		};

		float vertices[] = {
			// 位置 (x, y, z)          // 纹理坐标 (u, v)
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 0: 后下左
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 1: 后下右
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 2: 后上右
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 3: 后上左

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 4: 前下左
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 5: 前下右
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 6: 前上右
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // 7: 前上左

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // 8: 左侧上后 (Pos同7, UV不同)
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 9: 左侧上前 (Pos同3, UV不同)
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 10: 左侧下前 (Pos同0, UV不同)

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // 11: 右侧上后 (Pos同6, UV不同)
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 12: 右侧下前 (Pos同1, UV不同)
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 13: 右侧下后 (Pos同5, UV不同)

			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // 14: 底部后右 (Pos同1, UV不同)
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // 15: 顶部前后 (Pos同7, UV不同)
		};

		unsigned int indices[] = 
		{
	0, 1, 2,   2, 3, 0,       // 背面 (Face 1)
	4, 5, 6,   6, 7, 4,       // 前面 (Face 2)
	8, 9, 10,  10, 4, 8,      // 左面 (Face 3)
	11, 2, 12, 12, 13, 11,    // 右面 (Face 4)
	10, 14, 5, 5, 4, 10,      // 底面 (Face 5)
	3, 2, 11,  11, 15, 3      // 顶面 (Face 6)
		};

		m_Ibo = std::make_unique<IndexBuffer>(indices, 36);
		m_Vao = std::make_unique<VertexArray>();
		m_Vbo = std::make_unique<VertexBuffer>(vertices, 16 * 5 * sizeof(float));


		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);

		m_Shader = std::make_unique<Shader>("src/res/shaders/shader2.shader");
		m_Texture = std::make_unique<Texture>("src/res/texture/2000x2000bb-100 (4).jpg");

		m_Vao->AddBuffer(*m_Vbo, layout);
		m_Shader->Bind();

		m_Texture->Bind(0);
		m_Shader->SetUniform1i("u_Texture", 0);

		m_RotationZ = 0.0f;
		m_RotationX = 0.0f;
		m_RotationY = 0.0f;

	 cubePositions = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
		};
	}

	TestTexture3D::~TestTexture3D()
	{
	}

	void TestTexture3D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture3D::OnRender()
	{
		GLCALL(glViewport(0, 0, 1920, 1080));
		GLCALL(glEnable(GL_DEPTH_TEST));
		GLCALL(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_Texture->Bind(0);

		float radius = 10.0f;

		float cameraX =sin( glfwGetTime()) * radius;
		float cameraZ = cos(glfwGetTime()) * radius;

		m_View = glm::lookAt(glm::vec3(cameraX, 0,cameraZ), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		for (unsigned i = 0; i <cubePositions.size(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);

			model = glm::translate(model, m_Translation);
			model = glm::translate(model, cubePositions[i]);

			float angle = 20.0f * i;

			model = glm::rotate(model, glm::radians(m_RotationX), glm::vec3(1, 0, 0));
			model = glm::rotate(model, glm::radians(m_RotationY), glm::vec3(0, 1, 0));
			model = glm::rotate(model, glm::radians(m_RotationZ), glm::vec3(0, 0, 1));
			model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 0, 1));


			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(33.0f), glm::vec3(0.5f, 0.9f, 0.2f));

			model = glm::scale(model, m_Scare);

			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_Model", model);
			m_Shader->SetUniformMat4f("u_Projection", m_Proj);
			m_Shader->SetUniformMat4f("u_View", m_View);

			Renderer renderer;
			renderer.Draw(*m_Vao, *m_Ibo, *m_Shader);
		}


		m_Recorder.CaptureFrame(1920, 1080);
	}

	void TestTexture3D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation", &m_Translation.x, -10, 30.0f);
		ImGui::SliderFloat("RotationZ", &m_RotationZ, 0, 1000.0f);
		ImGui::SliderFloat("RotationX", &m_RotationX, 0, 1000.0f);
		ImGui::SliderFloat("RotationY", &m_RotationY, 0, 1000.0f);
		ImGui::SliderFloat3("Scare", &m_Scare.x, -5, 5.0f);
		ImGui::ColorEdit4("Clear Color", m_ClearColor);

		ImGui::Separator();
		// 添加一个勾选框，勾选后弹出 FFmpeg 窗口
		ImGui::Checkbox("Open FFmpeg Tool", &m_ShowFFmpegTool);

		if (m_ShowFFmpegTool)
		{
			m_Recorder.OnImGuiRender(1920,1080);
		}
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}