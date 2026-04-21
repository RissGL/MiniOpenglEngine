#pragma once
#include "Test.h"
#include "Core/GameObject.h"
#include "Model/Model.h"
#include "Base/Shader.h"
#include "Camera/CameraController.h" 
#include <vector>
#include <memory>
#include "Base/Framebuffer.h"

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

		void BeginScene();
		void EndScene();
	private:
		std::vector<std::shared_ptr<GameObject>> m_GameObjects;

		std::shared_ptr<Model> m_MonsterModel;
		std::unique_ptr<Shader> m_LightingShader;

		Camera m_Camera;
		CameraController m_CameraController;

		//Ö¡»º³åÇø
		std::unique_ptr<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	};
}