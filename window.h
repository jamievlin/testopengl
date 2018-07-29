#include <GLFW/glfw3.h>
#include <string>

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

    float totalTime;
};

