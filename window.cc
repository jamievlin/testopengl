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

Window::Window() : win1(glfwCreateWindow(width, height, "Hello OpenGL", nullptr, nullptr)), totalTime(0.f),flipped(false), 
                   transfMat(1.f), projMat(glm::perspective(glm::radians(60.f), width / (float)height, 0.1f, 100.f)), 
                   modelMat(1.f)
{
    glfwMakeContextCurrent(win1);
    glfwSetWindowUserPointer(win1, this);

    glewExperimental = GL_TRUE;
    glewInit();

    glGenVertexArrays(1, &vao);
    glGenVertexArrays(1, &finalvao);

    loadShaders();
    loadTextures();

    createCubeArray(vao, shaderProg);

    transfMat = glm::rotate(transfMat, glm::radians(80.f), glm::vec3(0.f, 0.f, 1.f));

    viewMat = glm::lookAt(
        glm::vec3(2.f, -2.f, 2.f),
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0.f, 0.f, 1.f)
    ); 

    fbo = createFrameBuffer(&texFboTarget, &rboDepthStencil);

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

    GLuint finVertShader = createShaderFile("shaders/outbuffer/framebuffer.vs.glsl", GL_VERTEX_SHADER);
    GLuint finFragShader = createShaderFile("shaders/outbuffer/framebuffer.fs.glsl", GL_FRAGMENT_SHADER);

    fbshaderProg = glCreateProgram();
    glAttachShader(fbshaderProg, finVertShader);
    glAttachShader(fbshaderProg, finFragShader);
    glLinkProgram(fbshaderProg);
}

void Window::loadTextures()
{
    glGenTextures(1, &tex1);
    glGenTextures(1, &tex2);

    glpng::PNGArray im("res/texture.png");
    glActiveTexture(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, tex1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, im.width, im.height, 0, GL_RGB, GL_FLOAT, im.data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glpng::PNGArray im2("res/texture2.png");
    glActiveTexture(GL_TEXTURE1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, tex2);
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
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

void Window::createCubeArray(GLint _vao, GLint shaderProg)
{
    glBindVertexArray(_vao);
    // glLinkProgram(shaderProg);
    // glUseProgram(shaderProg);

    // [x, y, z] [i] [u, v]
    std::vector<float> vertices = {
        -0.5f, 0.5f, -0.5f, 1.f, 0.f, 0.f,  // vert 0
        0.5f, 0.5f, -0.5f, 0.5f, 1.f, 0.f,  // vert 1
        0.5f, -0.5f, -0.5f, 0.1f, 1.f, 1.f, // vert 2
        -0.5f, -0.5f, -0.5f, 0.5f, 0.f, 1.f, // vert 3
        
        - 0.5f, 0.5f, .5f, 1.f, 0.f, 1.f,  // vert 4
        0.5f, 0.5f, .5f, 1.f, 1.f, 1.f,  // vert 5
        0.5f, -0.5f, .5f, 1.f, 1.f, 0.f, // vert 6
        -0.5f, -0.5f, .5f, 1.f, 0.f, 0.f, // vert 7

        //floors 
        -1.f, -1.f, -.5f, 0.f, 0.f, 0.f,
        1.f, 1.f, -.5f, 0.f, 0.f, 0.f,
        1.f, -1.f, -.5f, 0.f, 0.f, 0.f,
        -1.f, 1.f, -.5f, 0.f, 0.f, 0.f,
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

        //floors
        8, 9, 10, 
        8, 9, 11
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


    cubeUniforms.emplace("multInten", glGetUniformLocation(shaderProg, "multIntensity"));
    cubeUniforms.emplace("color", glGetUniformLocation(shaderProg, "triangleCol"));
    cubeUniforms.emplace("weight", glGetUniformLocation(shaderProg, "mixWeight"));
    cubeUniforms.emplace("time", glGetUniformLocation(shaderProg, "_time"));
    cubeUniforms.emplace("scaleFactor", glGetUniformLocation(shaderProg, "scaleFactor"));
    cubeUniforms.emplace("transfMat", glGetUniformLocation(shaderProg, "transfMat"));
    cubeUniforms.emplace("modelMat", glGetUniformLocation(shaderProg, "modelMat"));
    cubeUniforms.emplace("viewMat", glGetUniformLocation(shaderProg, "viewMat"));
    cubeUniforms.emplace("projMat", glGetUniformLocation(shaderProg, "projMat"));

    cubeUniforms.emplace("texture1", glGetUniformLocation(shaderProg, "texture1"));
    cubeUniforms.emplace("texture2", glGetUniformLocation(shaderProg, "texture2"));

    // std::cout << deltaTime << std::endl;
    GLint posAttrib = glGetAttribLocation(shaderProg, "position");
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(posAttrib);

    GLint intAttrib = glGetAttribLocation(shaderProg, "intensity");
    glVertexAttribPointer(intAttrib, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(intAttrib);

    GLint texAttrib = glGetAttribLocation(shaderProg, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float)));    
}

void Window::eventTick(float deltaTime)
{
    
}

GLuint Window::createFrameBuffer(GLuint* texColBuffer, GLuint* rboDepthStencil)
{
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, texColBuffer);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, *texColBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *texColBuffer, 0);

    glGenRenderbuffers(1, rboDepthStencil);
    glBindRenderbuffer(GL_RENDERBUFFER, *rboDepthStencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *rboDepthStencil);

    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // glClearColor(1, 1, 1, 1);
    return fbo;
}

void Window::drawTick(float deltaTime)
{

    totalTime += deltaTime;
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
    glLinkProgram(shaderProg);
    glUseProgram(shaderProg);
    glUniform1f(cubeUniforms["time"], totalTime/1000);

    glUniform1f(cubeUniforms["multInten"], 1);

    auto rgb = color::hsv2rgb(glm::vec3(totalTime/1000, 1, 1), false);
    glUniform3f(cubeUniforms["color"], rgb.r, rgb.g, rgb.b);

    glUniform1f(cubeUniforms["weight"], (float)glm::sin(totalTime/1000) * 0.5 + 0.5);

    glUniformMatrix4fv(cubeUniforms["projMat"], 1, GL_FALSE, glm::value_ptr(projMat));
    glUniformMatrix4fv(cubeUniforms["viewMat"], 1, GL_FALSE, glm::value_ptr(viewMat));   

    glm::mat4 flipMat(1.f);
    flipMat = glm::scale(flipMat, glm::vec3(-1.f, 1.f, 1.f));
    transfMat = glm::rotate(transfMat, glm::radians(deltaTime/100), glm::vec3(0.f, 0.f, 1.f));

    glUniformMatrix4fv(cubeUniforms["transfMat"], 1, GL_FALSE, glm::value_ptr(flipped ? flipMat *  transfMat : transfMat));
    glUniform1f(cubeUniforms["scaleFactor"], 1.f + (1.f * glm::sin(totalTime/1000)));
    glUniform1f(cubeUniforms["weight"], (float)glm::sin(totalTime/1000) * 0.5 + 0.5);

    modelMat = glm::mat4(1.f);
    glUniformMatrix4fv(cubeUniforms["modelMat"], 1, GL_FALSE, glm::value_ptr(modelMat));
    // begin drawing

    glClearColor(0.392, 0.584, 0.929, 1.f);
    // glClearColor(0, 0, 0, 1);

    glUniform1i(cubeUniforms["texture1"], 0);
    glUniform1i(cubeUniforms["texture2"], 1);

    glEnable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));

    // drawing floor

    glEnable(GL_STENCIL_TEST);
    glStencilMask(0xff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xff);

    glClear(GL_STENCIL_BUFFER_BIT);

    glDepthMask(GL_FALSE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(36 * sizeof(unsigned int)));
    glDepthMask(GL_TRUE);

    modelMat = glm::scale(glm::translate(modelMat, glm::vec3(0, 0, -.5f)), glm::vec3(1, 1, -1));
    modelMat = glm::translate(modelMat, glm::vec3(0, 0, 0.5f));

    glUniformMatrix4fv(cubeUniforms["modelMat"], 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniform1f(cubeUniforms["multInten"], 0.3);

    // reflection
    glStencilFunc(GL_EQUAL, 1, 0xff);
    glStencilMask(0x00);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));

    glDisable(GL_STENCIL_TEST);
    glUniform1f(cubeUniforms["multInten"], 1);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(36 * sizeof(unsigned int)));
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // end drawing 

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(finalvao);

    glUseProgram(fbshaderProg);
    // [x, y] [u, v]
    std::vector<float> scrverts = {
        -1, -1, 0, 0, 
        1, -1, 1 ,0, 
        1, 1, 1 ,1, 

        -1, -1, 0, 0, 
        -1, 1, 0 ,1, 
        1, 1, 1 ,1
    };

    GLuint vbo2;
    glGenBuffers(1, &vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, scrverts.size() * sizeof(float), scrverts.data(), GL_STATIC_DRAW);

    GLint fbTextureUni = glGetUniformLocation(fbshaderProg, "texFrameBuffer");

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texFboTarget);
    glUniform1i(fbTextureUni, 2);

    GLint fbPosAttrib = glGetAttribLocation(fbshaderProg, "position");
    glVertexAttribPointer(fbPosAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(fbPosAttrib);

    GLint fbTexCoordAttrib = glGetAttribLocation(fbshaderProg, "texcoord");
    glVertexAttribPointer(fbTexCoordAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(fbTexCoordAttrib);

    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
