#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include "window.h"
#include <memory>

int main(int argc, char* argv[])
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    {
        std::unique_ptr<Window> mainWin = std::unique_ptr<Window>(new Window());
        mainWin->mainLoop();
    }
    glfwTerminate();
    
}

