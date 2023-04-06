#pragma once

#include <fstream>
#include <set>
#include <vector>
#include <iostream>
#include <iterator>
#include <tuple>

#include "include/glm/vec3.hpp"
#include "include/glm/vector_relational.hpp"
#include "include/glm/geometric.hpp"

struct compareVec3
{
    // Adapted from https://stackoverflow.com/questions/46636721/how-do-i-use-glm-vector-relational-functions
    bool operator() (const std::tuple<glm::vec3, glm::vec3, unsigned int>& lhs, const std::tuple<glm::vec3, glm::vec3, unsigned int>& rhs) const
    {
        glm::vec3 nequ = glm::notEqual(std::get<0>(lhs), std::get<0>(rhs));
        return glm::lessThan(std::get<0>(lhs), std::get<0>(rhs))[nequ[0] ? 0 : (nequ[1] ? 1 : 2)];
    }
};

struct STLmesh
{
    std::vector<std::pair<glm::vec3, glm::vec3>> vertices;
    std::vector<unsigned int> indices;
};

STLmesh readSTL(const char* filepath);
void packSTL(const char* stl_path, const char* output_path);
void readPackedSTL(const char* data_path);