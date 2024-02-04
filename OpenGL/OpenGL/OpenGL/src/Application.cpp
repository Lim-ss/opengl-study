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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

    {//make a scope to clean the vartables in stack,otherwise somethings go wrong after call glfwTerminate();
        float positions[] = {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,
            -0.5f,  0.5f
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        //unsigned int vao;//Verrex Array Object
        //glGenVertexArrays(1, &vao);
        //glBindVertexArray(vao);
        VertexArray va;

        //unsigned int vbo;//vbo,aka.vertex buffer object
        //glgenbuffers(1, &vbo);
        //glbindbuffer(gl_array_buffer, vbo);
        //glbufferdata(gl_array_buffer, 8 * sizeof(float), positions, gl_static_draw);
        VertexBuffer vb(positions, 8 * sizeof(float));

        //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);
        //glEnableVertexAttribArray(0);
        VertexBufferLayout layout;
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
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

        //unbind all thing
        shader.Unbind();
        va.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        float r = 0.0f;
        float increment = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            //glClear(GL_COLOR_BUFFER_BIT);
            renderer.Clear();

            //glUseProgram(shader);//step.1
            //glUniform4f(location, r, 0.3f, 0.8f, 1.0f);
            //shader.Bind();//step.1

            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            //glBindVertexArray(vao);//step.2
            //va.Bind();//step.2

            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);//step.3
            //ib.Bind();//step.3

            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);//draw with index(indices)
            //glDrawArrays(GL_TRIANGLES, 0, 3);//draw with position directly
            renderer.Draw(va, ib, shader);

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;
            r += increment;

            /*legacy method*/
            //glBegin(GL_TRIANGLES);
            //glVertex2f(-0.5f, -0.5f);
            //glVertex2f(0.0f, 0.5f);
            //glVertex2f(0.5f, -0.5f);
            //glEnd();


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }
    glfwTerminate();
    return 0;
}