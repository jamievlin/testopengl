#ifndef __TOGL_CALLBACK
#define __TOGL_CALLBACK

#include <GLFW/glfw3.h>

namespace callback 
{ 
    void processKeys(GLFWwindow* window, int key, int scancode, int action, int mods); 
}

#endif