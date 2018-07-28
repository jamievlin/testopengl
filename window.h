#include <GLFW/glfw3.h>
#include <string>

class Window
{
public:
    Window();
    ~Window();

    bool isActive();
    void mainLoop();
    void eventTick();

private: 
    GLFWwindow *win1;
    GLuint shaderProg;
    GLuint vao;
};

GLuint createShaders(GLchar const* src, int shaderType);
GLuint createShaderFile(std::string file, int shaderType);