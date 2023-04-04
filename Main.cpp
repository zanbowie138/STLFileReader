#include <iostream>

#include "STLmesh.h"

int main()
{
    STLmesh mesh = convertSTL("cube.stl");

    std::cout << "Finished reading!" << std::endl;
    std::cout << "Length of vertice array is: " << mesh.vertices.size() << std::endl;
    std::cout << "Length of indice array is: " << mesh.indices.size() << std::endl;
    std::cout << "Length of normals array is: " << mesh.normals.size() << std::endl;

    // For debugging purposes
    /*
    for (std::pair<glm::vec3, unsigned int> p: mesh.vertices)
    {
        std::cout << "vec3(" << p.first.x << ", " << p.first.y << ", " << p.first.z << ")" << std::endl;
    }

    for (int i = 0; i < mesh.indices.size(); i+=3) {
        std::cout << mesh.indices[i] << " " << mesh.indices[i+1] << " " << mesh.indices[i+2] << std::endl;
    }

    for (glm::vec3 p: mesh.normals)
    {
        std::cout << "vec3(" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;
    }*/

    return 0;
}