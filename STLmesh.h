#pragma once

#include <fstream>
#include <unordered_set>
#include <vector>
#include <iostream>

#include "include/glm/glm.hpp"

struct STLmesh
{
    std::unordered_set<glm::vec3> vertices;
    std::vector<unsigned int> indices;
};

STLmesh convertSTL(const char* filepath);