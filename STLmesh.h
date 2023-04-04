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
    bool operator() (const glm::vec3& lhs, const glm::vec3& rhs) const
    {
        glm::vec3 nequ = glm::notEqual(lhs, rhs);
        return glm::lessThan(lhs, rhs)[nequ[0] ? 0 : (nequ[1] ? 1 : 2)];
    }
};

struct STLmesh
{
    std::set<glm::vec3, compareVec3> vertices;
    std::vector<unsigned int> indices;
};

STLmesh convertSTL(const char* filepath);