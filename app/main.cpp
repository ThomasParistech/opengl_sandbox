/*********************************************************************************************************************
 * File : main.cpp                                                                                                   *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include "renderer.h"
#include "index_buffer.h"
#include "shader.h"
#include "vertex_array.h"
#include "vertex_buffer.h"

#include <GLFW/glfw3.h>

int main(int argc, const char *argv[])
{
    GLFWwindow *window;

    // Init the Lib
    if (!glfwInit())
        return 1;

    // Create named window and its opengl context
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return 2;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        // clang-format off
        float positions[] = {
            -0.5f, -0.5f, // 0
            0.5f, -0.5f, // 1
            0.5f,  0.5f, // 2
            -0.5f,  0.5f  // 3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };
        // clang-format on

        VertexArray va;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.push<float>(2);
        va.add_buffer(vb, layout);

        IndexBuffer ib(indices, 6);

        Shader shader("/home/trouch/Dev/opengl_sandbox/res/shaders/basic.shader");
        shader.bind();
        shader.set_uniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        va.unbind();
        vb.unbind();
        ib.unbind();
        shader.unbind();

        float r = 0.0f;
        float increment = 0.05f;
        // Loop util the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            // Render here
            GlCall(glClear(GL_COLOR_BUFFER_BIT));

            shader.bind();
            shader.set_uniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            va.bind();
            ib.bind();
            GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = +0.05f;
            r += increment;

            // Swap front and back buffers
            GlCall(glfwSwapBuffers(window));

            // Poll for and process events
            GlCall(glfwPollEvents());
        }
    }
    glfwTerminate();
    return 0;
}