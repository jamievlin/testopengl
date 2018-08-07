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
#include <memory>

#include "window.h"
#include "glpng.h" 
#include "shadersproc.h"
#include "color.h"

Window::Window() : win1(glfwCreateWindow(800, 800, "Hello OpenGL", nullptr, nullptr))
{
    glfwMakeContextCurrent(win1);

    glewExperimental = GL_TRUE;
    glewInit();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    loadShaders();
    loadTextures();
}

void Window::loadShaders()
{
    GLuint vertShader = createShaderFile("shaders/vs.glsl", GL_VERTEX_SHADER);
    GLuint fragShader = createShaderFile("shaders/fs.glsl", GL_FRAGMENT_SHADER);

    shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertShader);
    glAttachShader(shaderProg, fragShader);
    glLinkProgram(shaderProg);
}

void Window::loadTextures()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLuint tex[2];
    glGenTextures(2, tex);

    this->im = std::make_unique<glpng::PNGArray>("res/texture.png");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, im->width, im->height, 0, GL_RGB, GL_FLOAT, im->data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    this->im2 = std::make_unique<glpng::PNGArray>("res/texture2.png");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, im2->width, im2->height, 0, GL_RGB, GL_FLOAT, im2->data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE0);
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

    // [x, y] [v] [u, v]
    std::vector<float> vertices = {
        -0.5f, 0.5f, 1.f, 0.f, 0.f,   // vert 1
        0.5f, 0.5f, 0.5f, 1.f, 0.f,   // vert 2
        0.5f, -0.5f, 0.1f, 1.f, 1.f,  // vert 3
        -0.5f, -0.5f, 0.5f, 0.f, 1.f  // vert 4

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

    glUseProgram(shaderProg);

    GLuint tbo[2];
    tbo[0] = glGetUniformLocation(shaderProg, "texture1");
    tbo[1] = glGetUniformLocation(shaderProg, "texture2");

    // assert(glGetUniformLocation(shaderProg, "texture1") != glGetUniformLocation(shaderProg, "texture2"));

    glUniform1i(tbo[0], 0);
    glUniform1i(tbo[1], 1);

    GLint uniColor = glGetUniformLocation(shaderProg, "triangleCol");
    auto rgb = color::hsv2rgb(glm::vec3(totalTime/1000, 1, 1), false);
    glUniform3f(uniColor, rgb.r, rgb.g, rgb.b);

    GLint uniWeight = glGetUniformLocation(shaderProg, "mixWeight");
    glUniform1f(uniWeight, (float)glm::sin(totalTime/1000) * 0.5 + 0.5);

    GLint uniTime = glGetUniformLocation(shaderProg, "_time");
    glUniform1f(uniTime, totalTime/1000);

    GLint posAttrib = glGetAttribLocation(shaderProg, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(posAttrib);

    GLint intAttrib = glGetAttribLocation(shaderProg, "intensity");
    glVertexAttribPointer(intAttrib, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(intAttrib);

    GLint texAttrib = glGetAttribLocation(shaderProg, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));

    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);;

}
