#pragma once
#include "tests/Test.h"
#include <vector>
#include "Base/VertexArray.h"
#include "Base/VertexBuffer.h"
#include "Base/IndexBuffer.h"
#include "Base/Shader.h"
#include <memory>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "GLVideoRecorder.h"
#include "Camera/Camera.h"
#include "Material/LightAndMaterial.h"
#include "Material/Texture.h"
#include "Camera/CameraController.h"

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

		GLVideoRecorder m_Recorder;
		bool m_ShowFFmpegTool = false;
		float m_ClearColor[4];

		Camera m_Camera;
		float cameraSpeed;

		std::unique_ptr<VertexArray> m_LightVao;

		Material m_Material;
		Light m_Light;

		std::unique_ptr<Texture> m_DiffuseMap;
		std::unique_ptr<Texture> m_SpecularMap;
		std::unique_ptr<Texture> m_EmissionMap;

		CameraController m_CameraController;
	};
}