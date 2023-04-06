#include <iostream>

#include "STLmesh.h"

int main()
{
    packSTL("cube.stl", "output.dat");

    /*STLmesh mesh = readSTL("cube.stl");

    std::cout << "Finished reading!" << std::endl;
    std::cout << "Length of vertice array is: " << mesh.vertices.size() << std::endl;
    std::cout << "Length of indice array is: " << mesh.indices.size() << std::endl;

    // Printing out the output

    
    for (std::pair<glm::vec3, glm::vec3> p: mesh.vertices)
    {
        std::cout << "Position: vec3(" << p.first.x << ", " << p.first.y << ", " << p.first.z << ")" << std::endl;
        std::cout << "Normal: vec3(" << p.second.x << ", " << p.second.y << ", " << p.second.z << ")" << std::endl;
    }

    for (int i = 0; i < mesh.indices.size(); i+=3) {
        std::cout << mesh.indices[i] << " " << mesh.indices[i+1] << " " << mesh.indices[i+2] << std::endl;
    }*/

    return 0;
}