#ifndef __TOGL_WINDOW
#define __TOGL_WINDOW

#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>
#include <string>

#include "callback.h"
#include "glpng.h"

inline glm::mat4 invdual(glm::mat4 in);
class Window
{
public:
    Window();
    ~Window();

    bool isActive();
    void mainLoop();

    friend void callback::processKeys(GLFWwindow *window, int key, int scancode, int action, int mods);

private: 
    GLFWwindow *win1;
    GLuint shaderProg, fbshaderProg;
    GLuint vao, finalvao, fbo;
    GLuint texFboTarget, rboDepthStencil;
    GLuint tex1, tex2;

    float totalTime;

    // void createCubeArray(GLint _vao, GLint shaderProg);
    void loadObj(GLint _vao, GLint shaderProg, std::string const fileLoc, std::unordered_map<std::string, GLint>& uniforms);
    GLuint createFrameBuffer(GLuint* texColBuffer, GLuint* rboDepthStencil); 

    int totalElements;

    std::unordered_map<std::string, GLint> cubeUniforms;
    void loadShaders();
    void loadTextures();

    /** Main event tick
     * 
     * @param deltaTime[float] Time duration since last event call. 
     *  In milliseconds.
     */
    void eventTick(float deltaTime);
    void drawTick(float deltaTime);

    glm::mat4 transfMat, viewMat, projMat, modelMat;
};

#endif