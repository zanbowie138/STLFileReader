#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <utility>

#include "include/glm/vec3.hpp"
#include "include/glm/geometric.hpp"
#include "include/glm/gtx/hash.hpp"

struct ModelPt
{
    glm::vec3 position;
    glm::vec3 normal;
};

static std::pair<std::vector<ModelPt>, std::vector<unsigned int>> ReadSTL(const char* filepath);
void PackSTL(const char* stl_path, const char* output_path);
static std::pair<std::vector<ModelPt>, std::vector<unsigned int>> ReadPackedSTL(const char* filepath);

static std::pair<std::vector<ModelPt>, std::vector<unsigned int>> ReadSTL(const char* filepath)
{
    // STL File format: https://people.sc.fsu.edu/~jburkardt/data/stlb/stlb.html

    std::vector<ModelPt> points;
    std::vector<unsigned int> indices;

    std::unordered_map<glm::vec3, unsigned int, std::hash<glm::vec3>> pointToIndex;

    std::ifstream is(filepath, std::ios::binary);

    // First 80 chars is the header (title)
    is.seekg(80);

    // Next is the number of triangles, which is an unsigned integer (4 bytes or 4 chars).  Buffer index is offset by 80 chars
    unsigned int numTriangles;
    is.read(reinterpret_cast<char*>(&numTriangles), sizeof(unsigned int));

    unsigned int currentIndex = 0;

    for (unsigned int t = 0; t < numTriangles; t++)
    {
        // Jump to triangle location
        is.seekg(t * 50 + 84);

        // Normal vector for entire triangle
        glm::vec3 tempNormal;
        is.read(reinterpret_cast<char*>(&tempNormal), sizeof(glm::vec3));

        // 3 points per triangle
        for (int p = 0; p < 3; p++)
        {
            glm::vec3 tempPos;
            is.read(reinterpret_cast<char*>(&tempPos), sizeof(glm::vec3));

            auto iterator = pointToIndex.find(tempPos);
            if (iterator == pointToIndex.end())
            {
                // If the current point is new
                pointToIndex.insert(std::make_pair(tempPos, currentIndex));
                points.emplace_back(ModelPt{ tempPos, tempNormal });
                indices.push_back(currentIndex);
                currentIndex++;
            }
            else
            {
                indices.push_back(iterator->second);
                // Add normal to existing point normal
                points[iterator->second].normal = points[iterator->second].normal + tempNormal;
            }
        }
    }
    is.close();

    for (auto& point : points)
    {
        point.normal = glm::normalize(point.normal);
    }

    return std::make_pair(points, indices);
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
void PackSTL(const char* stl_path, const char* output_path)
{
    auto stl = ReadSTL(stl_path);

    std::cout << "Packing..." << std::endl;

    // Packing
    std::ofstream fs(output_path, std::ofstream::in | std::ofstream::binary | std::ofstream::trunc);

    // Write amount of points
    unsigned int sizeTemp = stl.first.size();
    fs.write(reinterpret_cast<char*>(&sizeTemp), sizeof(sizeTemp));

    // Write amount of indices
    sizeTemp = stl.second.size();
    fs.write(reinterpret_cast<char*>(&sizeTemp), sizeof(sizeTemp));

    for (auto& point : stl.first)
    {
        fs.write(reinterpret_cast<char*>(&point.position), sizeof(glm::vec3));

        glm::vec3 normalized = glm::normalize(point.normal);
        fs.write(reinterpret_cast<char*>(&normalized), sizeof(glm::vec3));
    }

    for (auto& index : stl.second)
    {
        fs.write(reinterpret_cast<char*>(&index), sizeof(unsigned int));
    }
    fs.close();
}

static std::pair<std::vector<ModelPt>, std::vector<unsigned int>> ReadPackedSTL(const char* filepath)
{
    // Reading packed file
    std::ifstream is(filepath, std::ios::out | std::ios::binary);
    is.seekg(0);

    // Get vertex amount
    unsigned int vertexAmount;
    is.read(reinterpret_cast<char*>(&vertexAmount), sizeof(unsigned int));

    // Get index amount
    unsigned int indexAmount;
    is.read(reinterpret_cast<char*>(&indexAmount), sizeof(unsigned int));

    // Read position and normal information
    ModelPt tempPt{};
    std::vector<ModelPt> vertices;
    vertices.reserve(vertexAmount);
    for (int i = 0; i < static_cast<int>(vertexAmount); i++)
    {
        is.read(reinterpret_cast<char*>(&tempPt.position), sizeof(glm::vec3));
        is.read(reinterpret_cast<char*>(&tempPt.normal), sizeof(glm::vec3));
        vertices.emplace_back(tempPt);
    }

    // Read index information
    unsigned int tempIndex;
    std::vector<unsigned int> indices;
    indices.reserve(indexAmount);
    for (int i = 0; i < static_cast<int>(indexAmount); i++)
    {
        is.read(reinterpret_cast<char*>(&tempIndex), sizeof(unsigned int));
        indices.push_back(tempIndex);
    }
    is.close();

    return std::make_pair(vertices, indices);
}

int main()
{
    auto mesh = ReadSTL("stl/cube.stl");

    std::cout << "Finished reading!" << std::endl;
    std::cout << "Length of vertice array is: " << mesh.first.size() << std::endl;
    std::cout << "Length of indice array is: " << mesh.second.size() << std::endl;

    PackSTL("stl/cube.stl", "cube.bin");

    auto mesh2 = ReadPackedSTL("cube.bin");

    std::cout << "Finished reading!" << std::endl;
    std::cout << "Length of vertice array is: " << mesh2.first.size() << std::endl;
    std::cout << "Length of indice array is: " << mesh2.second.size() << std::endl;

    return 0;
}