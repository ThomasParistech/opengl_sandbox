/*********************************************************************************************************************
 * File : renderer.cpp                                                                                               *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include "renderer.h"

void GlClearError()
{
    while (glGetError())
    {
    };
}

bool GlLogCall()
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