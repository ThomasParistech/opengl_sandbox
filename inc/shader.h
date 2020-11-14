/*********************************************************************************************************************
 * File : shader.h                                                                                                   *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>

class Shader
{
public:
    Shader(const std::string &filepath);

    ~Shader();

    void bind() const;

    void unbind() const;

    void set_uniform1i(const std::string &name, int value);
    void set_uniform1f(const std::string &name, float value);
    void set_uniform4f(const std::string &name, float v0, float v1, float v2, float v3);

private:
    struct ProgramSource
    {
        std::string VertexSource;
        std::string FragmentSource;
    };

    ProgramSource parse_shader(const std::string &filepath);

    unsigned int compile_shader(unsigned int type, const std::string &source);

    unsigned int create_shader(const std::string &vertexShader, const std::string &fragmentShader);

    int get_uniform_location(const std::string &name);

    std::unordered_map<std::string, int> uniform_location_cache_;
    unsigned int rendered_id_;
};

#endif // SHADER_H
