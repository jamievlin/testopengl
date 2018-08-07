#ifndef __TOGL_WINDOW
#define __TOGL_WINDOW

#include <GLFW/glfw3.h>
#include <string>
#include <memory>

#include "glpng.h"

class Window
{
public:
    Window();
    ~Window();

    bool isActive();
    void mainLoop();

    /** Main event tick
     * 
     * @param deltaTime[float] Time duration since last event call. 
     *  In milliseconds.
     */
    void eventTick(float deltaTime);

private: 
    GLFWwindow *win1;
    GLuint shaderProg;
    GLuint vao;

    std::unique_ptr<glpng::PNGArray> im, im2;

    float totalTime;

    void loadShaders();
    void loadTextures();
};

#endif