#pragma once

#include <fstream>
#include <set>
#include <vector>
#include <iostream>

#include "include/glm/vec3.hpp"

struct STLmesh
{
    std::set<glm::vec3> vertices;
    std::vector<unsigned int> indices;
};

STLmesh convertSTL(const char* filepath);