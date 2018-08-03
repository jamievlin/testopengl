#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include <cassert>

// #include <iostream>
#include <chrono>
#include <cmath>
#include <ratio>

#include "window.h"
#include "shadersproc.h"
#include "color.h"

Window::Window() : win1(glfwCreateWindow(800, 600, "Hello OpenGL", nullptr, nullptr))
{
    glfwMakeContextCurrent(win1);

    glewExperimental = GL_TRUE;
    glewInit();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vertShader = createShaderFile("shaders/vs.glsl", GL_VERTEX_SHADER);
    GLuint fragShader = createShaderFile("shaders/fs.glsl", GL_FRAGMENT_SHADER);

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
    auto lastTime = std::chrono::high_resolution_clock::now();
    while (isActive())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float, std::milli> duration = newTime - lastTime;
        eventTick(duration.count());
        lastTime = newTime;
        glfwSwapBuffers(win1);        
    }
}


void Window::eventTick(float deltaTime)
{
    totalTime += deltaTime;
    // std::cout << totalTime << std::endl;

    // [x, y] [v]
    std::vector<float> vertices = {
        -0.5f, 0.5f, 1.f,    // vert 1
        0.5f, 0.5f, 0.5f,  // vert 2
        0.5f, -0.5f, 0.1f, // vert 3

        -0.5f, -0.5f, 0.5f,  // vert 4

    };

    std::vector<GLuint> elements = {
        0, 1, 2, // V1-V2-V3
        0, 2, 3 // V2-V3-V4
    }; 

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * elements.size(), elements.data(), GL_STATIC_DRAW);


    glLinkProgram(shaderProg);
    glUseProgram(shaderProg);

    GLint uniColor = glGetUniformLocation(shaderProg, "triangleCol");

    auto rgb = color::hsv2rgb(glm::vec3(totalTime/1000, 1, 1), false);
    glUniform3f(uniColor, rgb.r, rgb.g, rgb.b);

    GLint posAttrib = glGetAttribLocation(shaderProg, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(posAttrib);

    GLint intAttrib = glGetAttribLocation(shaderProg, "intensity");
    glVertexAttribPointer(intAttrib, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(intAttrib);

    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);;

}
