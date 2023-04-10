#include <iostream>

#include <fstream>
#include <set>
#include <vector>
#include <iostream>
#include <iterator>
#include <tuple>

#include "include/glm/vec3.hpp"
#include "include/glm/vector_relational.hpp"
#include "include/glm/geometric.hpp"

struct compareVec3
{
    // Adapted from https://stackoverflow.com/questions/46636721/how-do-i-use-glm-vector-relational-functions
    bool operator() (const std::tuple<glm::vec3, glm::vec3, unsigned int>& lhs, const std::tuple<glm::vec3, glm::vec3, unsigned int>& rhs) const
    {
        glm::vec3 nequ = glm::notEqual(std::get<0>(lhs), std::get<0>(rhs));
        return glm::lessThan(std::get<0>(lhs), std::get<0>(rhs))[nequ[0] ? 0 : (nequ[1] ? 1 : 2)];
    }
};
struct STLmesh
{
    std::vector<std::pair<glm::vec3, glm::vec3>> vertices;
    std::vector<unsigned int> indices;
};

STLmesh readSTL(const char* filepath);
void packSTL(const char* stl_path, const char* output_path);
void readPackedSTL(const char* data_path);

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

    std::cout << "Number of triangles: " << numTriangles << std::endl;

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

    std::cout << "Packing..." << std::endl;
    
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

int main()
{
    packSTL("stl/ChessSTL/rook.STL", "bin/rook.bin");
    packSTL("stl/ChessSTL/pawn.STL", "bin/pawn.bin");
    packSTL("stl/ChessSTL/queen.STL", "bin/queen.bin");
    packSTL("stl/ChessSTL/king.STL", "bin/king.bin");
    packSTL("stl/ChessSTL/horse.STL", "bin/horse.bin");
    packSTL("stl/ChessSTL/bishop.STL", "bin/bishop.bin");
    std::cout << "Finished packing!" << std::endl;

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