#include "tests/TestMenu.h"
#include "imgui/imgui.h"
#include "Renderer.h"

namespace test
{
	TestMenu::TestMenu(Test*& currentTestPointer)
		:m_Tests(), m_CurrentTest(currentTestPointer)
	{
	}
	TestMenu::~TestMenu()
	{
	}
	void TestMenu::OnImGuiRender()
	{
		ImGui::Begin("Test Menu");
		for (auto& test:m_Tests)
		{
			if (ImGui::Button(test.first.c_str(), ImVec2(100, 50)))
			{
				m_CurrentTest=test.second();
			}
		}
		ImGui::End();
	}
	void TestMenu::OnRender()
	{
		GLCALL(glClearColor(0.0f,0.0f,0.0f,0.0f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));
	}
	void TestMenu::OnUpdate(float deltaTime)
	{
	}
}	