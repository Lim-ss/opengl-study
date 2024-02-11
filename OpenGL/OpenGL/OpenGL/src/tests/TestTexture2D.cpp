#include "TestTexture2D.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

    TestTexture2D::TestTexture2D()
        :m_Translation(0, 0, 0),
        m_RotationAngle(0),
        m_Scale(1, 1, 1),
        m_IO(ImGui::GetIO()),
        m_Proj(glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_Model(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_MVP(m_Proj* m_View* m_Model),
        m_IfWireframeMode(false)
	{
        float positions[] = {
            //x   , y    , u   , v
              -100.0f, -100.0f, 0.0f, 0.0f, // 0
               100.0f, -100.0f, 1.0f, 0.0f, // 1
               100.0f,  100.0f, 1.0f, 1.0f, // 2
              -100.0f,  100.0f, 0.0f, 1.0f  // 3
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        //unsigned int vao;//Verrex Array Object
        //glGenVertexArrays(1, &vao);
        //glBindVertexArray(vao);
        //VertexArray va;
        m_VAO = std::make_unique<VertexArray>();

        //unsigned int vbo;//vbo,aka.vertex buffer object
        //glgenbuffers(1, &vbo);
        //glbindbuffer(gl_array_buffer, vbo);
        //glbufferdata(gl_array_buffer, 8 * sizeof(float), positions, gl_static_draw);
        //VertexBuffer vb(positions, 16 * sizeof(float));
        m_VBO = std::make_unique<VertexBuffer>(positions, 16 * sizeof(float));

        //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);
        //glEnableVertexAttribArray(0);
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VBO, layout);

        //unsigned int ibo;//index buffer object
        //glGenBuffers(1, &ibo);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        //IndexBuffer ib(indices, 6);
        m_IBO = std::make_unique<IndexBuffer>(indices, 6);

        //shaderprogramsource source = parseshader("res/shaders/basic.shader");
        //unsigned int shader = createshader(source.vectexsource, source.fragmentsource);
        //gluseprogram(shader);
        //Shader shader("res/shaders/Basic.shader");
        m_Shader = std::make_unique<Shader>("res/shaders/TestTexture2D.shader");
        m_Shader->Bind();

        //int location = glGetUniformLocation(shader, "u_Color");
        //ASSERT(location != -1);
        //glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f);
        //shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

       // Texture texture("res/textures/red.png");
        m_Texture = std::make_unique<Texture>("res/textures/red.jpg");
        m_Texture->Bind();
        m_Shader->SetUniform1i("u_Texture", 0);

        //unbind all thing
        //shader.Unbind();
        //va.Unbind();
        //vb.Unbind();
        //ib.Unbind();
	}

	TestTexture2D::~TestTexture2D()
	{

	}

	void TestTexture2D::OnUpdate(double deltaTime)
	{

	}

	void TestTexture2D::OnRender()
	{
        {
            //change the proj matrix because the change of window size
            int width, height;
            GLFWwindow* window = glfwGetCurrentContext();
            glfwGetWindowSize(window, &width, &height);
            m_Proj = glm::ortho<float>(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);

            // Wireframe Mode
            if (m_IfWireframeMode)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            //processInput
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                m_Translation.x += 5;
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                m_Translation.x -= 5;
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                m_Translation.y += 5;
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                m_Translation.y -= 5;
            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
                m_RotationAngle += 5;
            if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
                m_RotationAngle -= 5;
            if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
                m_Scale.x = m_Scale.y -= 0.05f;
            if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
                m_Scale.x = m_Scale.y += 0.05f;
        }
        m_Model = glm::translate(glm::mat4(1.0f), m_Translation);
        m_Model = glm::rotate(m_Model, glm::radians(m_RotationAngle), glm::vec3(0.0, 0.0, 1.0));
        m_Model = glm::scale(m_Model, m_Scale);
        m_MVP = m_Proj * m_View * m_Model;
        m_Shader->SetUniformMat4f("u_MVP", m_MVP);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);
        Renderer renderer;
        renderer.Clear();

        //glUseProgram(shader);//step.1
        //glUniform4f(location, r, 0.3f, 0.8f, 1.0f);
        //shader.Bind();//step.1

        //glBindVertexArray(vao);//step.2
        //va.Bind();//step.2

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);//step.3
        //ib.Bind();//step.3

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);//draw with index(indices)
        //glDrawArrays(GL_TRIANGLES, 0, 3);//draw with position directly
        
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader, 6);
		
	}

	void TestTexture2D::OnImguiRender()
	{
        //ImGui::ShowDemoWindow();
        

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.

        ImGui::Text("Use diretion key to control the object");               // Display some text (you can use a format strings too)

        ImGui::SliderFloat3("Translation", &m_Translation.x, 0.0f, 600.0f);

        ImGui::Checkbox("Wireframe Mode", &m_IfWireframeMode);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_IO.Framerate, m_IO.Framerate);

	}
}