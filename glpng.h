#ifndef __TOGL_GLPNG && NO_PNG
#define __TOGL_GLPNG

#include <vector>
#include <string>

namespace glpng
{

    typedef std::vector<float> fvec;
    class PNGArray
    {
        public:
        PNGArray(std::string file);
        ~PNGArray();

        int width, height;
        fvec data;
    };
}

#endif