#include "color.h"

#include <glm/glm.hpp>
#include <cmath>
#include <iostream>

namespace color
{
    // implementation from
    // https://en.wikipedia.org/wiki/HSL_and_HSV#Converting_to_RGB
    glm::vec3 hsv2rgb(glm::vec3 const &hsv, bool deg)
    {
        float h = std::fmod(hsv.x, deg? 360.f : 2*M_PI);
        float hprime = h/(deg? 60.f : M_PI / 3);

        float C = hsv.y * hsv.z;
        float X = C * (1 - std::abs(std::fmod(hprime, 2.f) - 1));

        // std::cout << hprime << " ";
        // std::cout << "C" << C << " ";
        // std::cout << "X" << X << " ";

        glm::vec3 rgbraw(0, 0, 0);

        if (hprime >= 0 and hprime < 1)
        {
            rgbraw.r = C;
            rgbraw.g = X;
        }
        else if (hprime >= 1 and hprime < 2)
        {
            rgbraw.r = X;
            rgbraw.g = C;
        }
        else if (hprime >= 2 and hprime < 3)
        {
            rgbraw.g = C;
            rgbraw.b = X;
        }
        else if (hprime >= 3 and hprime < 4)
        {
            rgbraw.g = X;
            rgbraw.b = C;
        }
        else if (hprime >= 4 and hprime < 5)
        {
            rgbraw.r = X;
            rgbraw.b = C;
        }
        else if (hprime >= 5 and hprime < 6)
        {
            rgbraw.r = C;
            rgbraw.b = X;
        }

        // std::cout << "R" << rgbraw.r << " G" << rgbraw.g << " B" << rgbraw.b;
        // std::cout << std::endl;

        float m = hsv.z - C;
        return rgbraw + glm::vec3(m, m, m);
    }

}