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

    // Vertice will have position and normal information
    std::set<std::tuple<glm::vec3, glm::vec3, unsigned int>, compareVec3> vertices;
    std::vector<unsigned int> indices;

    std::string localDir = "";
    std::ifstream is((localDir + filepath), std::ios::binary);

    // First 80 chars is the header (title)
    // Next is the number of triangles, which is an unsigned integer (4 bytes or 4 chars).  Buffer index is offset by 80 chars
    is.seekg(80);
    unsigned int numTriangles;
    is.read((char*) &numTriangles, sizeof(unsigned int));

    glm::vec3 temp;
    unsigned int indice = 0;
    std::pair<std::set<std::tuple<glm::vec3, glm::vec3, unsigned int>>::iterator,bool> temp_pair;
    glm::vec3 temp_normal;

    for (int t = 0; t < numTriangles; t++)
    {
        is.seekg(84+50*t);

        // Normal vector
        is.read((char*) &temp, sizeof(glm::vec3));
        temp_normal = temp;

        // 3 Vertexes
        for (int i = 0; i < 3; i++)
        {
            is.read((char*) &temp, sizeof(glm::vec3));
            temp_pair = vertices.insert(std::tuple<glm::vec3, glm::vec3, unsigned int>(temp, temp_normal, indice));

            // If inserted successfully, increment indice count and push a new indice value.  Else, find the copied vertex and push it's indice value.
            if (temp_pair.second)
            {
                indices.push_back(indice);
                indice++;
            } else 
            {
                std::tuple<glm::vec3, glm::vec3, unsigned int> temp_vertex = (*(temp_pair.first));
                vertices.erase(temp_vertex);

                indices.push_back(std::get<2>(temp_vertex));

                temp_vertex = std::make_tuple(std::get<0>(temp_vertex), std::get<1>(temp_vertex) + temp_normal, std::get<2>(temp_vertex));
                vertices.insert(temp_vertex);
            }
        }
        
    }
    is.close();
    return STLmesh{vertices, indices};
}