#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <cassert>
#include <sstream>
#include <string>

#include "shadersproc.h"

GLuint createShaders(GLchar const *src, int shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    assert(status == GL_TRUE);
    return shader;
}

GLuint createShaderFile(std::string file, int shaderType)
{
    std::ifstream shaderFile;
    shaderFile.open(file);
    std::stringstream shaderSrc;

    if (shaderFile)
    {
        shaderSrc << shaderFile.rdbuf();
        shaderFile.close();
    }
    else
    {
        throw 1;
    }

    return createShaders(shaderSrc.str().data(), shaderType);
}
