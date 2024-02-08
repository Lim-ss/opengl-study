#pragma once

#include "Test.h"

#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "imgui/imgui.h"

#include <memory>

namespace test {

	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImguiRender() override;
	private:
		glm::vec3 m_Translation;
		float m_RotationAngle;
		glm::vec3 m_Scale;
		glm::mat4 m_Proj;
		glm::mat4 m_View;
		glm::mat4 m_Model;
		glm::mat4 m_MVP;

		//use smart pointer to keep the API consistent with before
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		ImGuiIO& m_IO;
		bool m_IfWireframeMode;
	};
}