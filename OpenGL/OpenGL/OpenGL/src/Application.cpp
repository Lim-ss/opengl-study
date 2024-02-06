#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);//vertical synchronization

    if(glewInit() != GLEW_OK)
        std::cout<<"glewinit error"<<std::endl;
    std::cout << glfwGetVersionString() << std::endl;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    {//make a scope to clean the vartables in stack,otherwise somethings go wrong after call glfwTerminate();
        float positions[] = {
           //x   , y    , u   , v
             0.0f,   0.0f,   0.0f, 0.0f, // 0
             200.0f, 0.0f,   1.0f, 0.0f, // 1
             200.0f, 200.0f, 1.0f, 1.0f, // 2
             0.0f,   200.0f, 0.0f, 1.0f  // 3
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(100, 200, 0));
        glm::mat4 mvp = proj * view * model;

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        //unsigned int vao;//Verrex Array Object
        //glGenVertexArrays(1, &vao);
        //glBindVertexArray(vao);
        VertexArray va;

        //unsigned int vbo;//vbo,aka.vertex buffer object
        //glgenbuffers(1, &vbo);
        //glbindbuffer(gl_array_buffer, vbo);
        //glbufferdata(gl_array_buffer, 8 * sizeof(float), positions, gl_static_draw);
        VertexBuffer vb(positions, 16 * sizeof(float));

        //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);
        //glEnableVertexAttribArray(0);
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        //unsigned int ibo;//index buffer object
        //glGenBuffers(1, &ibo);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        IndexBuffer ib(indices, 6);

        //shaderprogramsource source = parseshader("res/shaders/basic.shader");
        //unsigned int shader = createshader(source.vectexsource, source.fragmentsource);
        //gluseprogram(shader);
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        //int location = glGetUniformLocation(shader, "u_Color");
        //ASSERT(location != -1);
        //glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f);
        //shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

        Texture texture("res/textures/red.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        //unbind all thing
        shader.Unbind();
        va.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;


        glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

        glm::vec3 translation(0, 0, 0);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            model = glm::translate(glm::mat4(1.0f), translation);
            mvp = proj * view * model;

            /* Render here */
            //glClear(GL_COLOR_BUFFER_BIT);
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
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            //ImGui::ShowDemoWindow();

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
            {
                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

                ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 600.0f);            

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}