#ifndef __TOGL_SHADERSPROC
#define __TOGL_SHADERSPROC

#include <GLFW/glfw3.h>
#include <string>

GLuint createShaders(GLchar const *src, int shaderType);
GLuint createShaderFile(std::string file, int shaderType);
#endif