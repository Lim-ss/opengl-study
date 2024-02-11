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
            {{-100.0f, -100.0f,  100.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 1.0f},// 0
            {{ 100.0f, -100.0f,  100.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, 1.0f},// 1
            {{ 100.0f,  100.0f,  100.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, 1.0f},// 2
            {{-100.0f,  100.0f,  100.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, 1.0f},// 3

            {{-100.0f, -100.0f, -100.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 0.0f},// 4
            {{ 100.0f, -100.0f, -100.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, 0.0f},// 5
            {{ 100.0f,  100.0f, -100.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, 0.0f},// 6
            {{-100.0f,  100.0f, -100.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, 0.0f},// 7

            {{-100.0f,  100.0f, -100.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 1.0f},// 8
            {{ 100.0f,  100.0f, -100.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, 1.0f},// 9
            {{ 100.0f,  100.0f,  100.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, 1.0f},// 10
            {{-100.0f,  100.0f,  100.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, 1.0f},// 11

            {{-100.0f, -100.0f, -100.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 0.0f},// 12
            {{ 100.0f, -100.0f, -100.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, 0.0f},// 13
            {{ 100.0f, -100.0f,  100.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, 0.0f},// 14
            {{-100.0f, -100.0f,  100.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, 0.0f},// 15

            {{ 100.0f, -100.0f, -100.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 1.0f},// 16
            {{ 100.0f,  100.0f, -100.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, 1.0f},// 17
            {{ 100.0f,  100.0f,  100.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, 1.0f},// 18
            {{ 100.0f, -100.0f,  100.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, 1.0f},// 19

            {{-100.0f, -100.0f, -100.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 0.0f},// 20
            {{-100.0f,  100.0f, -100.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, 0.0f},// 21
            {{-100.0f,  100.0f,  100.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, 0.0f},// 22
            {{-100.0f, -100.0f,  100.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, 0.0f} // 23
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
            
            4, 5, 6,
            6 ,7, 4,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        m_VAO = std::make_unique<VertexArray>();

        m_VBO = std::make_unique<VertexBuffer>(vertexs, sizeof(VertexBuffer::vertex) * 24);

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);
        layout.Push<float>(1);
        m_VAO->AddBuffer(*m_VBO, layout);
        
        m_IBO = std::make_unique<IndexBuffer>(indices, sizeof(indices));//ATTENTION! argument.2 is count,not size

        m_Shader = std::make_unique<Shader>("res/shaders/Test3D.shader");
        m_Shader->Bind();

        m_Texture1 = std::make_unique<Texture>("res/textures/Elaina.jpg");
        m_Texture2 = std::make_unique<Texture>("res/textures/red.jpg");
        m_Texture1->Bind(1);
        m_Texture2->Bind(2);
        int samplers[2] = { 1, 2 };
        m_Shader->SetUniform1iv("u_Textures", 2, samplers);

        m_Camera = std::make_unique<Camera>(m_View);

        glEnable(GL_DEPTH_TEST);
    }

    Test3D::~Test3D()
    {
        glDisable(GL_DEPTH_TEST);
    }

    void Test3D::OnUpdate(double deltaTime)
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
            m_Proj = glm::perspective(glm::radians(m_Camera->fov), (float)width / (float)height, 0.1f, 1000.0f);
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader, 36);
    }

    void Test3D::OnImguiRender()
    {

        ImGui::Text("Press ESC to disable the cursor");               

        ImGui::SliderFloat("fov", &m_Camera->fov, 30.0f, 120.0f);

        ImGui::Checkbox("Wireframe Mode", &m_IfWireframeMode);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_IO.Framerate, m_IO.Framerate);

    }

    void Test3D::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        Camera::CursorPosCallback(window, xpos, ypos);
    }

    void Test3D::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Camera::KeyCallback(window, key, scancode, action, mods);
    }
}