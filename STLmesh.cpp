#include "STLmesh.h"

struct Tri
{
    glm::vec3 n;
    glm::vec3 v1;
    glm::vec3 v2;
    glm::vec3 v3;
};

STLmesh convertSTL(const char* filepath)
{
    // STL File format: https://people.sc.fsu.edu/~jburkardt/data/stlb/stlb.html
    std::set<std::pair<glm::vec3, unsigned int>, compareVec3> vertices;
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> normals;

    std::string localDir = "";
    std::ifstream is((localDir + filepath), std::ios::binary);

    // First 80 chars is the header (title)
    // Next is the number of triangles, which is an unsigned integer (4 bytes or 4 chars).  Buffer index is offset by 80 chars
    is.seekg(80);
    unsigned int numTriangles;
    is.read((char*) &numTriangles, sizeof(unsigned int));

    glm::vec3 temp;
    unsigned int indice = 0;
    std::pair<std::set<std::pair<glm::vec3, unsigned int>>::iterator,bool> temp_pair;

    for (int t = 0; t < numTriangles; t++)
    {
        is.seekg(84+50*t);

        // Normal vector
        is.read((char*) &temp, sizeof(glm::vec3));
        normals.push_back(temp);

        // 3 Vertexes
        for (int i = 0; i < 3; i++)
        {
            is.read((char*) &temp, sizeof(glm::vec3));
            temp_pair = vertices.insert(std::pair<glm::vec3, unsigned int>{temp, indice});
            // If inserted successfully, increment indice count and push a new indice value.  Else, find the copied vertex and push it's indice value.
            temp_pair.second ? (indice++, indices.push_back(indice)) : (indices.push_back((*(temp_pair.first)).second)); 
        }
        
    }
    is.close();
    return STLmesh{vertices, indices, normals};
}