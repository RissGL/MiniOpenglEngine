#pragma once
#include "tests/Test.h"
#include <vector>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include <memory>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "GLVideoRecorder.h"
#include "Camera.h"

namespace test
{
	class TestTexture3D:public Test
	{
	public:
		TestTexture3D();
		~TestTexture3D();
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> m_Vao;
		std::unique_ptr<VertexBuffer> m_Vbo;
		std::unique_ptr<IndexBuffer> m_Ibo;

		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_Model;
		glm::vec3 m_Translation;
		glm::vec3 m_Scare;
		float m_RotationZ;
		float m_RotationX;
		float m_RotationY;

		GLVideoRecorder m_Recorder;
		bool m_ShowFFmpegTool = false;

		float m_ClearColor[4];
		std::vector<glm::vec3> cubePositions;

		Camera m_Camera;

		float cameraSpeed;
	};
}