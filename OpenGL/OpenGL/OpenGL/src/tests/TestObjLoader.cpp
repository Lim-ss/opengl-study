#include "TestObjLoader.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace test {

    TestObjLoader::TestObjLoader()
        :
        m_Proj(glm::mat4(1.0f)),
        m_View(glm::mat4(1.0f)),
        m_Model(glm::mat4(1.0f)),
        m_MVP(glm::mat4(1.0f)),
        m_IO(ImGui::GetIO()),
        m_WireframeMode(false)
    {

        if (m_ObjLoader.LoadFile("res/mesh/yuegui.obj") == false)
        {
            std::cout << "obj loader failed" << std::endl;
        }
        else
        {
            std::cout << m_ObjLoader.LoadedMeshes.size() << " mesh(s) be loaded" << std::endl;
            m_Mesh = m_ObjLoader.LoadedMeshes[0];
            m_Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));//调整模型大小
        }

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        m_VAO = std::make_unique<VertexArray>();
        m_VBO = std::make_unique<VertexBuffer>(m_Mesh.Vertices.data(), sizeof(objl::Vertex) * m_Mesh.Vertices.size());
        VertexBufferLayout layout;
        layout.Push<float>(3);//position
        layout.Push<float>(3);//normal
        layout.Push<float>(2);//texCoord
        m_VAO->AddBuffer(*m_VBO, layout);
        m_IBO = std::make_unique<IndexBuffer>(m_Mesh.Indices.data(), m_Mesh.Indices.size());//ATTENTION! argument.2 is count,not size
        m_Shader = std::make_unique<Shader>("res/shaders/TestObjLoader.shader");
        m_Shader->Bind();
        m_Camera = std::make_unique<Camera>(m_View, glm::vec3(0.0f, 0.0f, 3.0f));

    }

    TestObjLoader::~TestObjLoader()
    {
        glDisable(GL_DEPTH_TEST);
    }

    void TestObjLoader::OnUpdate(double deltaTime)
    {
        m_Camera->CameraUpdate(deltaTime);
    }

    void TestObjLoader::OnRender()
    {
        if (m_WireframeMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        int width, height;
        GLFWwindow* window = glfwGetCurrentContext();
        glfwGetWindowSize(window, &width, &height);
        m_Proj = glm::perspective(glm::radians(m_Camera->fov), (float)width / (float)height, 0.1f, 1000.0f);
        
        m_MVP = m_Proj * m_View * m_Model;
        m_Shader->SetUniformMat4f("u_MVP", m_MVP);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
        Renderer renderer;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader, m_Mesh.Indices.size());
    }

    void TestObjLoader::OnImguiRender()
    {

        ImGui::Text("Press ESC to disable the cursor");

        ImGui::Checkbox("Wireframe Mode", &m_WireframeMode);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_IO.Framerate, m_IO.Framerate);

    }

    void TestObjLoader::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        Camera::CursorPosCallback(window, xpos, ypos);
    }

    void TestObjLoader::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Camera::KeyCallback(window, key, scancode, action, mods);
    }
}