/*********************************************************************************************************************
 * File : main.cpp                                                                                                   *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>

#define GlCall(x)   \
    GlClearError(); \
    x;              \
    assert(GlLogCall())

static void GlClearError()
{
    while (glGetError())
    {
    };
}

static bool GlLogCall()
{
    while (GLenum e = glGetError())
    {
        std::stringstream ss;
        ss << std::hex << e; // Error in hexadecimal
        std::cout << "[OpenGL error] " << ss.str() << std::endl;
        return false;
    }
    return true;
}

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

    unsigned int vao;
    GlCall(glGenVertexArrays(1, &vao));
    GlCall(glBindVertexArray(vao));

    unsigned int buffer;
    GlCall(glGenBuffers(1, &buffer));
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GlCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

    GlCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
    GlCall(glEnableVertexAttribArray(0));

    unsigned int ibo; // Index buffer object
    GlCall(glGenBuffers(1, &ibo));
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    ShaderProgramSource source = ParseShader("/home/trouch/Dev/opengl_sandbox/res/shaders/basic.shader");
    std::cout << "Vertex" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "Fragment" << std::endl;
    std::cout << source.FragmentSource << std::endl;

    GlCall(unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource));
    GlCall(glUseProgram(shader));

    int location = glGetUniformLocation(shader, "u_Color");
    assert(location != -1);
    GlCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

    float r = 0.0f;
    float increment = 0.05f;
    // Loop util the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        GlCall(glClear(GL_COLOR_BUFFER_BIT));

        GlCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
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
    glfwTerminate();
    return 0;
}