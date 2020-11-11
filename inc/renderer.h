/*********************************************************************************************************************
 * File : renderer.h                                                                                                 *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <assert.h>
#include <iostream>
#include <sstream>

#define GlCall(x)   \
    GlClearError(); \
    x;              \
    assert(GlLogCall())

void GlClearError();

bool GlLogCall();

#endif // RENDERER_H
