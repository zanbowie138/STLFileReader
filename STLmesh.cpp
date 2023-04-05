#include "STLmesh.h"

STLmesh readSTL(const char* filepath)
{
    // STL File format: https://people.sc.fsu.edu/~jburkardt/data/stlb/stlb.html

    // Vertice will have position and normal information
    std::set<std::tuple<glm::vec3, glm::vec3, unsigned int>, compareVec3> vertexes;
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
        is.read((char*) &temp_normal, sizeof(glm::vec3));

        // 3 Vertexes
        for (int i = 0; i < 3; i++)
        {
            is.read((char*) &temp, sizeof(glm::vec3));
            temp_pair = vertexes.insert(std::tuple<glm::vec3, glm::vec3, unsigned int>(temp, temp_normal, indice));

            // If inserted successfully, increment indice count and push a new indice value.  Else, find the copied vertex and push it's indice value.
            if (temp_pair.second)
            {
                indices.push_back(indice);
                indice++;
            } else 
            {
                std::tuple<glm::vec3, glm::vec3, unsigned int> temp_vertex = (*(temp_pair.first));
                vertexes.erase(temp_vertex);

                indices.push_back(std::get<2>(temp_vertex));

                temp_vertex = std::make_tuple(std::get<0>(temp_vertex), std::get<1>(temp_vertex) + temp_normal, std::get<2>(temp_vertex));
                vertexes.insert(temp_vertex);
            }
        }
        
    }
    is.close();
    return STLmesh{vertexes, indices};
}

// Basically the same function as above, but also packs the vertex and indice information into a binary file.
/*
Format:
    # of vertexes, unsigned int - 4 bytes
    # of indices, unsigned int - 4 bytes

    For each vertex:
        vec3 position, 3 floats with 4 bytes each
        vec3 normal (normalized), 3 floats with 4 bytes each
    
    For each indice
        unsigned int indice, 4 bytes

*/
void packSTL(const char* stl_path, const char* output_path)
{
    std::set<std::tuple<glm::vec3, glm::vec3, unsigned int>, compareVec3> vertexes;
    std::vector<unsigned int> indices;

    std::ifstream is(stl_path, std::ios::binary);

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

        is.read((char*) &temp_normal, sizeof(glm::vec3));

        for (int i = 0; i < 3; i++)
        {
            is.read((char*) &temp, sizeof(glm::vec3));
            temp_pair = vertexes.insert(std::tuple<glm::vec3, glm::vec3, unsigned int>(temp, temp_normal, indice));

            if (temp_pair.second)
            {
                indices.push_back(indice);
                indice++;
            } else 
            {
                std::tuple<glm::vec3, glm::vec3, unsigned int> temp_vertex = (*(temp_pair.first));
                vertexes.erase(temp_vertex);

                indices.push_back(std::get<2>(temp_vertex));

                temp_vertex = std::make_tuple(std::get<0>(temp_vertex), std::get<1>(temp_vertex) + temp_normal, std::get<2>(temp_vertex));
                vertexes.insert(temp_vertex);
            }
        }
        
    }
    is.close();
    
    // Packing
    std::ofstream fs(output_path, std::ofstream::in | std::ofstream::binary | std::ofstream::trunc);
    unsigned int size_temp = vertexes.size();
    fs.write((char*)&size_temp, sizeof(unsigned int));
    size_temp = indices.size();
    fs.write((char*)&size_temp, sizeof(unsigned int));
    for (std::tuple<glm::vec3, glm::vec3, unsigned int> v : vertexes)
    {
        fs.write((char*)&std::get<0>(v), sizeof(glm::vec3));

        glm::vec3 norm_normal = glm::normalize(std::get<1>(v));
        fs.write((char*)&(norm_normal), sizeof(glm::vec3));
    }
    for (unsigned int i : indices)
    {
        fs.write((char*)&i, sizeof(unsigned int));
    }
    fs.close();
}

void readPackedSTL(const char* data_path)
{
    // Reading packed file
    unsigned int vertex_len;
    unsigned int ind_len;

    std::ifstream is(data_path, std::ios::out | std::ios::binary);
    is.seekg(0);
    is.read((char*)&vertex_len, sizeof(unsigned int));
    is.read((char*)&ind_len, sizeof(unsigned int));

    glm::vec3 vertexes[vertex_len];
    glm::vec3 indices[ind_len];
    glm::vec3 normals[vertex_len];

    glm::vec3 temp;
    for (int i = 0; i < vertex_len; i++) 
    {
        is.read((char*)&temp, sizeof(glm::vec3));
        vertexes[i] = temp;
        is.read((char*)&temp, sizeof(glm::vec3));
        normals[i] = temp;
    }
    
    unsigned int temp_i;
    for (int i = 0; i < vertex_len; i++) 
    {
        is.read((char*)&temp, sizeof(unsigned int));
        indices[i] = temp;
    }
    is.close();
}