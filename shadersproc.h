#include <GLFW/glfw3.h>
#include <string>

GLuint createShaders(GLchar const *src, int shaderType);
GLuint createShaderFile(std::string file, int shaderType);