/*********************************************************************************************************************
 * File : main.cpp                                                                                                   *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

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

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!"<<std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    // Loop util the user closesthe window
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}