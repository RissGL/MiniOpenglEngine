#pragma once
#include "tests/Test.h"
#include <vector>
#include <memory>
#include "Base/VertexArray.h"
#include "Base/VertexBuffer.h"
#include "Base/IndexBuffer.h"
#include "Base/Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "GLVideoRecorder.h"
#include "Camera/Camera.h"
#include "Material/LightAndMaterial.h"
#include "Material/Texture.h"
#include "Camera/CameraController.h"

#include "Light/DirLight.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"

#include "Core/GameObject.h"
#include "Core/MeshRenderer.h"
#include "UI/SceneHierarchyPanel.h"
#include "Base/Framebuffer.h"
#include "Model/Model.h"

#include "Base/VertexArray.h"
#include "Base/VertexBuffer.h"
#include "Base/IndexBuffer.h"

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
		void BeginScene();
		void EndScene();

	private:
		std::unique_ptr<Shader> m_LightingShader;
		std::unique_ptr<Shader> m_LightCubeShader;

		float m_ClearColor[4];
		glm::vec3 m_LightPos;

		GLVideoRecorder m_Recorder;
		Camera m_Camera;
		float cameraSpeed;
		Material m_Material;

		std::unique_ptr<Texture> m_DiffuseMap;
		std::unique_ptr<Texture> m_SpecularMap;
		std::unique_ptr<Texture> m_EmissionMap;

		CameraController m_CameraController;

		std::shared_ptr<DirLight> m_SunLight;
		std::shared_ptr<PointLight> m_PointLight;
		std::shared_ptr<SpotLight> m_SpotLight;

		std::vector<std::shared_ptr<GameObject>> m_GameObjects;
		std::unique_ptr<SceneHierarchyPanel> m_HierarchyPanel;

		std::unique_ptr<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		std::shared_ptr<Model> m_SharedModel;

		std::unique_ptr<VertexArray> m_BoxVAO;
		std::unique_ptr<VertexBuffer> m_BoxVBO;
		std::unique_ptr<IndexBuffer> m_BoxIBO;
	};
}