#include "STLmesh.h"

STLmesh convertSTL(const char* filepath)
{
    std::set<glm::vec3> vertices;
    std::vector<unsigned int> indices;

    std::ifstream is(filepath, std::ios::binary);

    // Get length of file
    is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);

    char * buffer = new char [length];

    std::cout << "Reading " << length << " characters... ";

    // read data as a block:
    is.read (buffer,length);

    if (is)
      std::cout << "all characters read successfully.";
    else
      std::cout << "error: only " << is.gcount() << " could be read";
    is.close();

    // ...buffer contains the entire file...

    delete[] buffer;
    

    is.close();
    return STLmesh{vertices, indices};
}