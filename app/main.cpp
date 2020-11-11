/*********************************************************************************************************************
 * File : main.cpp                                                                                                   *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include "renderer.h"
#include "index_buffer.h"
#include "vertex_array.h"
#include "vertex_buffer.h"

#include <GLFW/glfw3.h>

#include <fstream>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string &filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType // Use the type as an index in the array
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
            ss[(int)type] << line << '\n';
    }
    return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string &source)
{
    GlCall(unsigned int id = glCreateShader(type));
    const char *src = source.c_str();
    GlCall(glShaderSource(id, 1, &src, nullptr));
    GlCall(glCompileShader(id));

    int result;
    GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char message[length];
        GlCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Fail to compile shader: "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << std::endl;
        std::cout << message << std::endl;
        GlCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    GlCall(unsigned int program = glCreateProgram());
    GlCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
    GlCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

    GlCall(glAttachShader(program, vs));
    GlCall(glAttachShader(program, fs));
    GlCall(glLinkProgram(program));
    GlCall(glValidateProgram(program));

    GlCall(glDeleteShader(vs));
    GlCall(glDeleteShader(fs));

    return program;
}

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

        ShaderProgramSource source = ParseShader("/home/trouch/Dev/opengl_sandbox/res/shaders/basic.shader");
        std::cout << "Vertex" << std::endl;
        std::cout << source.VertexSource << std::endl;
        std::cout << "Fragment" << std::endl;
        std::cout << source.FragmentSource << std::endl;

        GlCall(unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource));

        int location = glGetUniformLocation(shader, "u_Color");
        assert(location != -1);

        float r = 0.0f;
        float increment = 0.05f;
        // Loop util the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            // Render here
            GlCall(glClear(GL_COLOR_BUFFER_BIT));

            GlCall(glUseProgram(shader));
            GlCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

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

        GlCall(glDeleteProgram(shader));
    }
    glfwTerminate();
    return 0;
}