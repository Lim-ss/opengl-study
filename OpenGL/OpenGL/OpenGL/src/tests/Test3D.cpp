#include "Test3D.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

    Test3D::Test3D()
        :
        m_Proj(glm::mat4(1.0f)),
        m_View(glm::mat4(1.0f)),
        m_Model(glm::mat4(1.0f)),
        m_MVP(glm::mat4(1.0f)),
        m_IO(ImGui::GetIO()),
        m_IfWireframeMode(false)
    {
        VertexBuffer::vertex vertexs[] = {
          //{{ x,       y,       z }, { r,    g,     b }, { u,    v, },  id },
            {{-100.0f, -100.0f, 0.0}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 1.0f},// 0
            {{ 100.0f, -100.0f, 0.0}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, 1.0f},// 1
            {{ 100.0f,  100.0f, 0.0}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, 1.0f},// 2
            {{-100.0f,  100.0f, 0.0}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, 1.0f} // 3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        m_VAO = std::make_unique<VertexArray>();

        m_VBO = std::make_unique<VertexBuffer>(vertexs, sizeof(VertexBuffer::vertex) * 4);

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);
        layout.Push<float>(1);
        m_VAO->AddBuffer(*m_VBO, layout);
        
        m_IBO = std::make_unique<IndexBuffer>(indices, 6);//ATTENTION! argument.2 is count,not size

        m_Shader = std::make_unique<Shader>("res/shaders/Test3D.shader");
        m_Shader->Bind();

        m_Texture1 = std::make_unique<Texture>("res/textures/red.jpg");
        m_Texture1->Bind(0);
        m_Texture2 = std::make_unique<Texture>("res/textures/Elaina.jpg");
        m_Texture2->Bind(1);
        int samplers[2] = { 0, 1 };
        m_Shader->SetUniform1iv("u_Textures", 2, samplers);

        m_Camera = std::make_unique<Camera>(m_View);

        
    }

    Test3D::~Test3D()
    {

    }

    void Test3D::OnUpdate(float deltaTime)
    {
        m_Camera->CameraUpdate(deltaTime);
    }

    void Test3D::OnRender()
    {
        {
            //change the proj matrix because the change of window size
            int width, height;
            GLFWwindow* window = glfwGetCurrentContext();
            glfwGetWindowSize(window, &width, &height);
            //m_Proj = glm::ortho<float>(0.0f, (float)width, 0.0f, (float)height, -1.0f, 100.0f);
            m_Proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
            // Wireframe Mode
            if (m_IfWireframeMode)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);            
        }
        m_MVP = m_Proj * m_View * m_Model;
        m_Shader->SetUniformMat4f("u_MVP", m_MVP);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
        Renderer renderer;
        renderer.Clear();
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
    }

    void Test3D::OnImguiRender()
    {

        ImGui::Text("Use diretion key to control the object");               

        ImGui::Checkbox("Wireframe Mode", &m_IfWireframeMode);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_IO.Framerate, m_IO.Framerate);

    }

    void Test3D::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        Camera::CursorPosCallback(window, xpos, ypos);
    }
}