#pragma once
#include "tests/Test.h"
#include <vector>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include <memory>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "GLVideoRecorder.h"
#include "Camera.h"

namespace test
{
	class TestIllumination : public Test
	{
	public:
		TestIllumination();
		~TestIllumination();
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> m_Vao;
		std::unique_ptr<VertexBuffer> m_Vbo;
		std::unique_ptr<IndexBuffer> m_Ibo;

		std::unique_ptr<Shader> m_LightingShader; // 负责计算受光物体的明暗
		std::unique_ptr<Shader> m_LightCubeShader; // 负责把光源画成一个纯白/纯色的小方块

		glm::vec3 m_Translation;
		glm::vec3 m_Scare;

		glm::vec3 m_LightPos;
		glm::vec3 m_LightColor;
		glm::vec3 m_ObjectColor;

		GLVideoRecorder m_Recorder;
		bool m_ShowFFmpegTool = false;
		float m_ClearColor[4];

		Camera m_Camera;
		float cameraSpeed;

		float m_LightIntensity;

		std::unique_ptr<VertexArray> m_LightVao;
	};
}