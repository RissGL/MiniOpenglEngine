#pragma once
#include "Test.h"
#include "Core/GameObject.h"
#include "Model/Model.h"
#include "Base/Shader.h"
#include "Camera/CameraController.h" 
#include <vector>
#include <memory>
#include "Base/Framebuffer.h"
#include "Light/SpotLight.h"
#include "Demo/FirstPersonController.h"

namespace test
{
	class TestHorrorMaze :public Test
	{
	public:
		TestHorrorMaze();
		~TestHorrorMaze() = default;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void GenerateMaze();

		void BeginScene();
		void EndScene();
	private:
		std::vector<std::shared_ptr<GameObject>> m_GameObjects;

		std::shared_ptr<Model> m_SewerModel;
		std::shared_ptr<Model> m_FlashlightModel;
		std::unique_ptr<Shader> m_LightingShader;

		std::shared_ptr<Camera> m_Camera;

		std::shared_ptr<SpotLight> m_SpotLight;

		std::shared_ptr<GameObject> player;
		std::shared_ptr<GameObject> flashlight;

		//Ö¡»º³åÇø
		std::unique_ptr<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	};
}