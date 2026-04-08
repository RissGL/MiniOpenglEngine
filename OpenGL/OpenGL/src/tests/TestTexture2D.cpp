#include "tests/TestTexture2D.h"
#include "Renderer.h"
#include "imgui/imgui.h"
#include "VertexBufferLayout.h"

#include "Debug/Debug.h"


namespace test 
{
	TestTexture2D::TestTexture2D() 
		:m_Translation(200.0f, 200.0f, 0.0f),
		m_Scare(1.0f, 1.0f, 1.0f),
		m_Rotation(0.0f),
		m_Proj(glm::ortho(0.0f,1000.0f,0.0f,1000.0f,-1.0f,1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		float position[] = {
	-100.0f, -100.0f, 0.0f,0.0f,    //1.0f, 0.0f, 0.0f, 1.0f,
	-100.0f,  100.0f, 0.0f,1.0f,   //0.0f, 1.0f, 0.0f, 1.0f,
	 100.0f,  100.0f, 1.0f,1.0f,    //0.0f, 0.0f, 1.0f, 1.0f,
	 100.0f, -100.0f, 1.0f,0.0f    //0.0f, 0.0f, 1.0f, 1.0f,
		};

		unsigned int indices[] = {
	0, 1, 2,
	2, 3, 0
		};

		m_Vao = std::make_unique<VertexArray>();
		m_Vbo = std::make_unique<VertexBuffer>(position, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		m_Vao->AddBuffer(*m_Vbo, layout);

		m_Ibo = std::make_unique<IndexBuffer>(indices, 6);

		m_Shader = std::make_unique<Shader>("src/res/shaders/Basic.shader");
		m_Shader->Bind();

		m_Texture = std::make_unique<Texture>("src/res/texture/2000x2000bb-100 (4).jpg");
		m_Texture->Bind(0);
		m_Shader->SetUniform1i("u_Texture", 0);

	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
		GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));

		m_Texture->Bind(0);

		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
		model = glm::rotate(model, glm::radians(m_Rotation), glm::vec3(0,0,1));
		model = glm::scale(model, m_Scare);

		glm::mat4 mvp = m_Proj * m_View * model;

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);

		Renderer renderer;
		renderer.Draw(*m_Vao, *m_Ibo, *m_Shader);
	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation", &m_Translation.x, 0, 1000.0f);
		ImGui::SliderFloat("Rotation", &m_Rotation, 0, 1000.0f);
		ImGui::SliderFloat3("Scare", &m_Scare.x, 0, 10.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}