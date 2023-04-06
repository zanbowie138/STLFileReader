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
        is.read((char*)&temp_normal, sizeof(glm::vec3));

        // 3 Vertexes
        for (int i = 0; i < 3; i++)
        {
            is.read((char*)&temp, sizeof(glm::vec3));
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

    std::vector<std::pair<glm::vec3, glm::vec3>> vertexes_sorted(vertexes.size());
    
    for (const std::tuple<glm::vec3, glm::vec3, unsigned int>& t : vertexes)
    {
        vertexes_sorted[std::get<2>(t)] = std::pair<glm::vec3, glm::vec3>{std::get<0>(t),std::get<1>(t)};
    }

    std::vector<std::pair<glm::vec3, glm::vec3>> vertices_vector;

    for (const std::pair<glm::vec3, glm::vec3>& t: vertexes_sorted)
    {
        vertices_vector.push_back(t);
    }

    return STLmesh{vertices_vector, indices};
}

// Packs the vertex and indice information into a binary file.
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
    STLmesh stl = readSTL(stl_path);
    
    // Packing
    std::ofstream fs(output_path, std::ofstream::in | std::ofstream::binary | std::ofstream::trunc);

    unsigned int size_temp = stl.vertices.size();
    fs.write((char*)&size_temp, sizeof(unsigned int));
    size_temp = stl.indices.size();
    fs.write((char*)&size_temp, sizeof(unsigned int));

    for (std::pair<glm::vec3, glm::vec3> v : stl.vertices)
    {
        fs.write((char*)&v.first, sizeof(glm::vec3));

        glm::vec3 norm_normal = glm::normalize(v.second);
        fs.write((char*)&(norm_normal), sizeof(glm::vec3));
    }

    for (unsigned int i : stl.indices)
    {
        fs.write((char*)&i, sizeof(unsigned int));
    }
    fs.close();
}

void readPackedSTL(const char* data_path)
{
    std::ifstream is(data_path, std::ios::out | std::ios::binary);

    is.seekg(0);

    // Reading packed file
    unsigned int vertex_len;
    unsigned int ind_len;
    
    is.read((char*)&vertex_len, sizeof(unsigned int));
    is.read((char*)&ind_len, sizeof(unsigned int));

    glm::vec3 vertexes[vertex_len];
    unsigned int indices[ind_len];
    glm::vec3 normals[vertex_len];

    glm::vec3 temp;
    for (int i = 0; i < (int)vertex_len; i++) 
    {
        is.read((char*)&temp, sizeof(glm::vec3));
        vertexes[i] = temp;
        is.read((char*)&temp, sizeof(glm::vec3));
        normals[i] = temp;
    }
    
    unsigned int temp_i;
    for (int i = 0; i < (int)ind_len; i++) 
    {
        is.read((char*)&temp_i, sizeof(unsigned int));
        indices[i] = temp_i;
    }
    is.close();
}