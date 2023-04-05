#include <iostream>

#include "STLmesh.h"

int main()
{
    STLmesh mesh = convertSTL("cube.stl");

    std::cout << "Finished reading!" << std::endl;
    std::cout << "Length of vertice array is: " << mesh.vertices.size() << std::endl;
    std::cout << "Length of indice array is: " << mesh.indices.size() << std::endl;

    // Printing out the output
    /*
    for (std::tuple<glm::vec3, glm::vec3, unsigned int> p: mesh.vertices)
    {
        std::cout << "Position: vec3(" << std::get<0>(p).x << ", " << std::get<0>(p).y << ", " << std::get<0>(p).z << ")" << std::endl;
        std::cout << "Normal: vec3(" << std::get<1>(p).x << ", " << std::get<1>(p).y << ", " << std::get<1>(p).z << ")" << std::endl;
    }

    for (int i = 0; i < mesh.indices.size(); i+=3) {
        std::cout << mesh.indices[i] << " " << mesh.indices[i+1] << " " << mesh.indices[i+2] << std::endl;
    }*/

    return 0;
}