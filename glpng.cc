#ifndef NO_PNG

#include <png++/png.hpp>
#include "glpng.h"
#include <vector>
#include <string>


namespace glpng
{

    PNGArray::~PNGArray()
    {

    }

    PNGArray::PNGArray(std::string file)
    {
        png::image<png::rgb_pixel> im(file);
        width = im.get_width();
        height = im.get_height();
        data.reserve(3 * width * height);

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                auto pixel = im[y][x];
                data.push_back((int)pixel.red /(float)255);
                data.push_back((int)pixel.green / (float)255);
                data.push_back((int)pixel.blue / (float)255);
            }
        }
    }
}

#endif