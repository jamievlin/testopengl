#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include <cassert>

#include <iostream>

#include "window.h"

Window::Window() : win1(glfwCreateWindow(800, 600, "Hello OpenGL", nullptr, nullptr))
{
    glfwMakeContextCurrent(win1);

    glewExperimental = GL_TRUE;
    glewInit();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vertShader = createShaderFile("vs.glsl", GL_VERTEX_SHADER);
    GLuint fragShader = createShaderFile("fs.glsl", GL_FRAGMENT_SHADER);

    shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertShader);
    glAttachShader(shaderProg, fragShader);
}

Window::~Window()
{
    glfwDestroyWindow(win1);
}

bool Window::isActive()
{
    return not glfwWindowShouldClose(win1);
}

void Window::mainLoop()
{
    while (isActive())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
        eventTick();
        glfwSwapBuffers(win1);
    }
}

void Window::eventTick()
{
    std::vector<float> vertices = {
        0.0f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    glLinkProgram(shaderProg);
    glUseProgram(shaderProg);

    GLint posAttrib = glGetAttribLocation(shaderProg, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);

    glDrawArrays(GL_TRIANGLES, 0, 3);

}


GLuint createShaders(GLchar const* src, int shaderType)
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
