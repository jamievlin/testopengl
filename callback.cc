#include <GLFW/glfw3.h>
#include "window.h"
#include "callback.h"

#include <iostream>

namespace callback
{
    void processKeys(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Window* mainWin = (Window*)glfwGetWindowUserPointer(window);

        
        if (key == GLFW_KEY_ESCAPE and action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

        
    }
}