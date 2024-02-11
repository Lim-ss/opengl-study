#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace test {
	
	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(double deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImguiRender() {}
		virtual void CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {}
		virtual void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {}
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);

		void OnRender() override;
		void OnImguiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;
			m_Tests.push_back(std::make_pair(name, []() {return new T();}));
		}
	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
	};
}