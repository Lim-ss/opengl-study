#pragma once

#include "Test.h"

#include "Shader.h"
#include "VertexBufferLayout.h"

#include "GLFW/glfw3.h"

#include "imgui/imgui.h"

#include <memory>

namespace test {

	class TestPoint : public Test
	{
	public:
		TestPoint();
		~TestPoint();

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImguiRender() override;
	private:

		//use smart pointer to keep the API consistent with before
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<Shader> m_Shader;

		ImGuiIO& m_IO;
	};
}