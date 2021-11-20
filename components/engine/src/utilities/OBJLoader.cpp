//
// Created by BlackFlage on 13.11.2021.
//

#include "OBJLoader.h"
#include <fstream>
#include <sstream>
#include "Logger.h"

namespace vis
{
    Mesh *OBJLoader::load_from_file(const std::string &a_file_path)
    {
        std::ifstream file(a_file_path);

        if(!file)
        {
            LOG_ERROR("Failed to open file: {0} while loading .obj file!", a_file_path);
            return nullptr;
        }

        std::vector<glm::vec3> vertices;
        std::vector<unsigned int> indices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> textureCoords;
        std::vector<unsigned int> vertexNormalIndices;
        std::vector<unsigned int> textureCoordsIndices;
        std::string line;
        float x, y, z;
        while (std::getline(file, line))
        {
            std::istringstream iss;
            std::string operationType = line.substr(0, 2);

            if(operationType == "v ")
            {
                const char* data = line.c_str();

                sscanf_s(data, "v %f %f %f", &x, &y, &z);
                vertices.emplace_back(x, y, z);
            }
            else if(operationType == "vn")
            {
                const char* data = line.c_str();

                sscanf_s(data, "vn  %f %f %f", &x, &y, &z);
                normals.emplace_back(x, y, z);
            }
            else if(operationType == "vt")
            {
                const char* data = line.c_str();

                sscanf_s(data, "vt %f %f", &x, &y);
                textureCoords.emplace_back(x, y);
            }
            else if(operationType == "f ")
            {
                std::string delimiter = "/";
                std::string data = line;

                size_t pos = 0;
                while(data.size() > 0)
                {
                    while(data.at(0) == ' ' || data.at(0) == 'f')
                    {
                        data = data.erase(0, 1);
                    }

                    pos = data.find(' ');
                    std::string face = data.substr(0, pos);

                    if(pos == std::string::npos)
                    {
                        pos = data.size() - 1;
                    }

                    data.erase(0, pos + 1);

                    for(int i = 0; i < 3; i++)
                    {
                        pos = face.find(delimiter);
                        if(pos == 0)
                        {
                            face = face.erase(0, 1);
                            continue;
                        }
                        else if(pos == std::string::npos)
                        {
                            pos = face.size();
                        }

                        unsigned int value = std::stoi(face.substr(0, pos)) - 1;
                        face = face.erase(0, pos + delimiter.length());

                        switch(i)
                        {
                            case 0:
                                indices.push_back(value);
                                break;
                            case 1:
                                textureCoordsIndices.push_back(value);
                                break;
                            case 2:
                                vertexNormalIndices.push_back(value);
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }

        std::vector<Vertex> outVertices;

        for(int i = 0; i < indices.size(); i++)
        {
            outVertices.emplace_back(vertices.at(indices.at(i)), normals.at(vertexNormalIndices.at(i)));
        }

        return new Mesh(outVertices, indices, GL_QUADS);
    }
}