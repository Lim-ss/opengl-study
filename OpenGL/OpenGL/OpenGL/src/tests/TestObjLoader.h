#pragma once

#include "Test.h"

#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Camera.h"

#include "GLFW/glfw3.h"

#include "imgui/imgui.h"

#include <memory>

#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H
#include "obj-loader/OBJ-Loader.h"
#endif


namespace test {

	class TestObjLoader : public Test
	{
	public:
		TestObjLoader();
		~TestObjLoader();

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImguiRender() override;
		void CursorPosCallback(GLFWwindow* window, double xpos, double ypos) override;
		void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) override;
	private:

		glm::mat4 m_Proj;
		glm::mat4 m_View;
		glm::mat4 m_Model;
		glm::mat4 m_MVP;

		//use smart pointer to keep the API consistent with before
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Camera> m_Camera;

		objl::Loader m_ObjLoader;
		objl::Mesh m_Mesh;

		bool m_WireframeMode;

		ImGuiIO& m_IO;
	};
}