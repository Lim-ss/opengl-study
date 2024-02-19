#include "TestPoint.h"

#include "Renderer.h"

namespace test {
	TestPoint::TestPoint()
		:m_IO(ImGui::GetIO())
	{
		VertexBuffer::point points[] = {
			//{{   x,    y,     z }, { r,    g,     b },size},
			  { { 0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, 20.0f },
			  { { 0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, 40.0f },
			  { { 0.0f,-0.5f, 0.0f}, {0.3f, 0.1f, 0.8f}, 40.0f },
		};
		m_VAO = std::make_unique<VertexArray>();
		m_VBO = std::make_unique<VertexBuffer>(points, sizeof(VertexBuffer::vertex) * 3);
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(1);
		m_VAO->AddBuffer(*m_VBO, layout);
		m_Shader = std::make_unique<Shader>("res/shaders/TestPoint.shader");
	}

	TestPoint::~TestPoint()
	{

	}

	void TestPoint::OnUpdate(double deltaTime)
	{

	}

	void TestPoint::OnRender()
	{
		glClearColor(0.2f, 0.8f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_Shader->Bind();
		m_VAO->Bind();
		//glPointSize(10.0f);
		glEnable(GL_PROGRAM_POINT_SIZE);//important,you can not set the point size in shader without this line
		GLCall(glDrawArrays(GL_POINTS, 0, 3));
	}

	void TestPoint::OnImguiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_IO.Framerate, m_IO.Framerate);
	}
}