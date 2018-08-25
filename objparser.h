#ifndef __TOGL_OBJ_PARSER
#define __TOGL_OBJ_PARSER

#include <vector>
#include <string>

namespace objparser
{
    typedef std::vector<float> vfloat; 
    int loadobj(std::string const fileLoc, vfloat& vertexBuffer, std::vector<uint32_t>& elementBuffer);
}

#endif