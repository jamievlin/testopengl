#include <vector>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>
#include <boost/algorithm/string.hpp>

#include <glm/glm.hpp>

#include "objparser.h"

namespace objparser
{

    int const floatcount = 6;
    // (pos) [x, y, z] (normal) [x, y, z]
    int loadobj(std::string const fileLoc, vfloat& vertexBuffer, std::vector<uint32_t>& elementBuffer)
    {
        vertexBuffer.clear();
        elementBuffer.clear();

        std::vector<glm::vec3> rawVerts;
        std::vector<glm::vec3> rawNormals;

        std::ifstream objfile(fileLoc);

        assert(objfile.is_open());

        std::string buffer;

        while(getline(objfile, buffer))
        {
            if (buffer[0] == '#')
            {
                continue;
            }

            std::istringstream sstr(buffer);
            std::string mode;
            sstr >> mode;

            if (mode == "v")
            {
                std::string x, y, z;
                sstr >> x >> y >> z;
                rawVerts.push_back(glm::vec3(std::stof(x), std::stof(y), std::stof(z)));
            }
            else if (mode == "vn")
            {
                std::string x, y, z;
                sstr >> x >> y >> z;
                rawNormals.push_back(glm::vec3(std::stof(x), std::stof(y), std::stof(z)));
            }
            else if (mode == "f")
            {
                std::string tmpindex;
                uint32_t currIndex = vertexBuffer.size()/floatcount;
                int counter = 0; 

                while (sstr >> tmpindex)
                {
                    std::vector<std::string> contents;
                    boost::split(contents, tmpindex, [](char c) { return c == '/'; }); 

                    glm::vec3 vert = rawVerts[std::stoi(contents[0]) - 1];
                    glm::vec3 norm(0.f);
                    if (contents.size() >= 3)
                    {
                        norm = rawNormals[std::stoi(contents[2]) - 1];
                    }

                    vertexBuffer.push_back(vert.x); 
                    vertexBuffer.push_back(vert.y); 
                    vertexBuffer.push_back(vert.z); 

                    vertexBuffer.push_back(norm.x); 
                    vertexBuffer.push_back(norm.y); 
                    vertexBuffer.push_back(norm.z); 

                    counter++;
                }

                elementBuffer.push_back(currIndex);
                elementBuffer.push_back(currIndex + 1);
                elementBuffer.push_back(currIndex + 2);

                if (counter >= 4)
                {
                    elementBuffer.push_back(currIndex);
                    elementBuffer.push_back(currIndex + 3);
                    elementBuffer.push_back(currIndex + 2);
                }
            }
        }

        objfile.close();
        return elementBuffer.size();
    }
}