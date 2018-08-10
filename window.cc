#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include <cassert>

#include <iostream>

#include <chrono>
#include <cmath>
#include <ratio>
#include <memory>

#include "window.h"
#include "glpng.h" 
#include "shadersproc.h"
#include "color.h"
#include "callback.h"

int const width = 800;
int const height = 600;

Window::Window() : win1(glfwCreateWindow(width, height, "Hello OpenGL", nullptr, nullptr)), flipped(false), totalTime(0.f),
                   transfMat(1.f), projMat(glm::perspective(glm::radians(60.f), width / (float)height, 0.1f, 100.f))
{
    glfwMakeContextCurrent(win1);
    glfwSetWindowUserPointer(win1, this);

    glewExperimental = GL_TRUE;
    glewInit();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    loadShaders();
    loadTextures();

    transfMat = glm::rotate(transfMat, glm::radians(80.f), glm::vec3(0.f, 0.f, 1.f));

    viewMat = glm::lookAt(
        glm::vec3(2.f, -2.f, 2.f),
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0.f, 0.f, 1.f)
    ); 

    glEnable(GL_DEPTH_TEST);
    glfwSetKeyCallback(win1, callback::processKeys);
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

    glpng::PNGArray im("res/texture.png");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, im.width, im.height, 0, GL_RGB, GL_FLOAT, im.data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glpng::PNGArray im2("res/texture2.png");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, im2.width, im2.height, 0, GL_RGB, GL_FLOAT, im2.data.data());
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
        // eventTick(duration.count());
        drawTick(duration.count());
        lastTime = newTime;
        glfwSwapBuffers(win1);        
    }
}

void Window::eventTick(float deltaTime)
{

}

void Window::drawTick(float deltaTime)
{
    totalTime += deltaTime;
    // std::cout << totalTime << std::endl;

    // [x, y, z] [i] [u, v]
    std::vector<float> vertices = {
        -0.5f, 0.5f, -0.5f, 1.f, 0.f, 0.f,  // vert 0
        0.5f, 0.5f, -0.5f, 0.5f, 1.f, 0.f,  // vert 1
        0.5f, -0.5f, -0.5f, 0.1f, 1.f, 1.f, // vert 2
        -0.5f, -0.5f, -0.5f, 0.5f, 0.f, 1.f, // vert 3
        
        - 0.5f, 0.5f, .5f, 1.f, 0.f, 1.f,  // vert 4
        0.5f, 0.5f, .5f, 1.f, 1.f, 1.f,  // vert 5
        0.5f, -0.5f, .5f, 1.f, 1.f, 0.f, // vert 6
        -0.5f, -0.5f, .5f, 1.f, 0.f, 0.f // vert 7

    };

    std::vector<GLuint> elements = {

        
        0, 3, 4,
        4, 3, 7, 

        2, 3, 7,
        2, 7, 6, 

        0, 1, 4,
        1, 4, 5, 

        1, 2, 5,
        2, 5, 6, 

        4, 5, 7,
        5, 6, 7, 

        0, 1, 2, // V1-V2-V3
        0, 2, 3, // V2-V3-V4
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

    GLint tbo[2];
    tbo[0] = glGetUniformLocation(shaderProg, "texture1");
    tbo[1] = glGetUniformLocation(shaderProg, "texture2");

    glUniform1i(tbo[0], 0);
    glUniform1i(tbo[1], 1);

    GLint uniColor = glGetUniformLocation(shaderProg, "triangleCol");
    auto rgb = color::hsv2rgb(glm::vec3(totalTime/1000, 1, 1), false);
    glUniform3f(uniColor, rgb.r, rgb.g, rgb.b);

    GLint uniWeight = glGetUniformLocation(shaderProg, "mixWeight");
    glUniform1f(uniWeight, (float)glm::sin(totalTime/1000) * 0.5 + 0.5);

    GLint uniTime = glGetUniformLocation(shaderProg, "_time");
    glUniform1f(uniTime, totalTime/1000);

    GLint uniScaleFactor = glGetUniformLocation(shaderProg, "scaleFactor");

    glUniform1f(uniScaleFactor, 1.f + (1.f * glm::sin(totalTime/1000)));

    transfMat = glm::rotate(transfMat, glm::radians(deltaTime/500), glm::vec3(0.f, 0.f, 1.f));

    glm::mat4 flipMat(1.f);
    flipMat = glm::scale(flipMat, glm::vec3(-1.f, 1.f, 1.f));

    GLint uniTransfMat = glGetUniformLocation(shaderProg, "transfMat");
    glUniformMatrix4fv(uniTransfMat, 1, GL_FALSE, glm::value_ptr(flipped ? flipMat *  transfMat : transfMat));

    GLint uniViewMat = glGetUniformLocation(shaderProg, "viewMat");
    glUniformMatrix4fv(uniViewMat, 1, GL_FALSE, glm::value_ptr(viewMat));

    GLint uniProjMat = glGetUniformLocation(shaderProg, "projMat");
    glUniformMatrix4fv(uniProjMat, 1, GL_FALSE, glm::value_ptr(projMat));

    // std::cout << deltaTime << std::endl;
    GLint posAttrib = glGetAttribLocation(shaderProg, "position");
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(posAttrib);

    GLint intAttrib = glGetAttribLocation(shaderProg, "intensity");
    glVertexAttribPointer(intAttrib, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(intAttrib);

    GLint texAttrib = glGetAttribLocation(shaderProg, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float)));

    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));;

}
