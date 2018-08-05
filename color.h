#ifndef __TOGL_COLOR
#define __TOGL_COLOR

#include <glm/glm.hpp>

namespace color
{
    glm::vec3 hsv2rgb(glm::vec3 const &hsv, bool deg = true);
}

#endif