#pragma once

#include <fstream>
#include <set>
#include <vector>
#include <iostream>
#include <iterator>

#include "include/glm/vec3.hpp"
#include "include/glm/vector_relational.hpp"

struct compareVec3
{
    // Adapted from https://stackoverflow.com/questions/46636721/how-do-i-use-glm-vector-relational-functions
    bool operator() (const std::pair<glm::vec3, unsigned int>& lhs, const std::pair<glm::vec3, unsigned int>& rhs) const
    {
        glm::vec3 nequ = glm::notEqual(lhs.first, rhs.first);
        return glm::lessThan(lhs.first, rhs.first)[nequ[0] ? 0 : (nequ[1] ? 1 : 2)];
    }
};

struct STLmesh
{
    std::set<std::pair<glm::vec3, unsigned int>, compareVec3> vertices;
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> normals;
};

STLmesh convertSTL(const char* filepath);