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
#include "objparser.h"

int const width = 800;
int const height = 600;
int const resfactor = 2;


inline glm::mat4 invdual(glm::mat4 in)
{
    return glm::transpose(glm::inverse(in));
}

Window::Window() : win1(glfwCreateWindow(width, height, "Hello OpenGL", nullptr, nullptr)), totalTime(0.f),
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
    // loadTextures();

    loadObj(vao, shaderProg, "res/test_torus.obj", cubeUniforms);
    // createCubeArray(vao, shaderProg);

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
    GLuint vertShader = createShaderFile("shaders/main_shader.vs.glsl", GL_VERTEX_SHADER);
    GLuint fragShader = createShaderFile("shaders/main_shader.fs.glsl", GL_FRAGMENT_SHADER);

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
    glDeleteFramebuffers(1, &fbo);
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

void Window::loadObj(GLint _vao, GLint shaderProg, std::string const fileLoc, std::unordered_map<std::string, GLint>& uniforms)
{
    glBindVertexArray(_vao);

    std::vector<float> verts;
    std::vector<GLuint> elements;
    totalElements = objparser::loadobj(fileLoc, verts, elements);

    GLuint vbo;
    GLuint ebo;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLuint), elements.data(), GL_STATIC_DRAW);

    auto reguniforms = [&](std::string const uniformName) {uniforms.emplace(uniformName, glGetUniformLocation(shaderProg, uniformName.c_str())); };

    reguniforms("viewMat");
    reguniforms("projMat");
    reguniforms("modelMat");

    reguniforms("viewMatInvDual");
    reguniforms("projMatInvDual");
    reguniforms("modelMatInvDual");

    GLint posAttrib = glGetAttribLocation(shaderProg, "position");
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(posAttrib);

    GLint normalAttrib = glGetAttribLocation(shaderProg, "normal");
    glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(normalAttrib);

    glBindVertexArray(0);
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resfactor * width, resfactor * height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *texColBuffer, 0);

    glGenerateMipmap(GL_TEXTURE_2D);

    glGenRenderbuffers(1, rboDepthStencil);
    glBindRenderbuffer(GL_RENDERBUFFER, *rboDepthStencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, resfactor * width, resfactor * height);
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

    glViewport(0, 0, resfactor * width, resfactor *  height);

    glBindVertexArray(vao);
    glLinkProgram(shaderProg);
    glUseProgram(shaderProg);

    glClearColor(0, 0, 0, 1);

    modelMat = glm::rotate(modelMat, 0.5f * (deltaTime/1000), glm::vec3(0, 0, 1));

    glUniformMatrix4fv(cubeUniforms["projMat"], 1, GL_FALSE, glm::value_ptr(projMat));
    glUniformMatrix4fv(cubeUniforms["viewMat"], 1, GL_FALSE, glm::value_ptr(viewMat));   
    glUniformMatrix4fv(cubeUniforms["modelMat"], 1, GL_FALSE, glm::value_ptr(modelMat));   

    glUniformMatrix4fv(cubeUniforms["projMatInvDual"], 1, GL_FALSE, glm::value_ptr(invdual(projMat)));
    glUniformMatrix4fv(cubeUniforms["viewMatInvDual"], 1, GL_FALSE, glm::value_ptr(invdual(viewMat)));   
    glUniformMatrix4fv(cubeUniforms["modelMatInvDual"], 1, GL_FALSE, glm::value_ptr(invdual(modelMat)));   

    

    glEnable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, totalElements, GL_UNSIGNED_INT, nullptr);
    // end drawing 


    /* #region main draw */ 

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(finalvao);


    glViewport(0, 0, 1 * width, 1 *  height);

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
    glGenerateMipmap(GL_TEXTURE_2D);
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

    /* #endregion */
}

