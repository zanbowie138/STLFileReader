#include "STLmesh.h"

STLmesh convertSTL(const char* filepath)
{
    std::unordered_set<glm::vec3> vertices;
    std::vector<unsigned int> indices;

    std::ifstream is(filepath, std::ios::binary);
    


    return STLmesh{vertices, indices};
}