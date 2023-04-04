#include "STLmesh.h"

STLmesh convertSTL(const char* filepath)
{
    // STL File format: https://people.sc.fsu.edu/~jburkardt/data/stlb/stlb.html
    std::set<glm::vec3, compareVec3> vertices;
    std::vector<unsigned int> indices;

    std::string localDir = "";
    std::ifstream is((localDir + filepath), std::ios::binary);

    // Get length of file
    //is.seekg (0, is.end);
    //int length = is.tellg();

    //std::cout << "Creating buffer of: " << length << " length." << std::endl;

    // Create a buffer and input file into buffer
    //char* buffer = new char[length];
    //is.read(buffer, length);

    is.seekg(80);

    // First 80 chars is the header (title)
    // Next is the number of triangles, which is an unsigned integer (4 bytes or 4 chars).  Buffer index is offset by 80 chars
    unsigned int numTriangles = 0;

    int numberOfTriangles;
    is.read((char*) &numberOfTriangles, sizeof(int));

    std::cout << "Number of triangles is: " << numberOfTriangles << std::endl;

    // Now 

    //delete[] buffer;
    

    is.close();
    return STLmesh{vertices, indices};
}