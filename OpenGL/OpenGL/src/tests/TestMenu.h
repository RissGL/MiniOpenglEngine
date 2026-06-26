#pragma once

#include "Test.h"
#include <vector>
#include <iostream>
#include <functional>

namespace test
{
	class TestMenu:public Test 
	{
	public :
		TestMenu(Test*& currentTestPointer);
		 ~TestMenu();

		 void OnImGuiRender() override;
		 void OnRender() override;
		 void OnUpdate(float deltaTime) override;

		 template<typename T>
		 void RegisterTest(const std::string& name)
		 {
			 std::cout << "Registering test " << name << std::endl;
			 m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		 }
	private:
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
		Test*& m_CurrentTest;
	};

}

