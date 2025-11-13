#include "OBJLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

OBJLoader::OBJLoader() {}
OBJLoader::~OBJLoader() {}

MeshData OBJLoader::ReadOBJFile(const std::string& filePath)
{
    MeshData meshData;

    // Temporary storage (OBJ indices are separate for v/vt/vn)
    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec2> tempUVs;
    std::vector<glm::vec3> tempNormals;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filePath << std::endl;
        return meshData;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            // Vertex position
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            tempVertices.push_back(vertex);
        }
        else if (prefix == "vt") {
            // Texture coordinate
            glm::vec2 uv;
            iss >> uv.x >> uv.y;
            tempUVs.push_back(uv);
        }
        else if (prefix == "vn") {
            // Normal
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        }
        else if (prefix == "f") {
            // Face - process each vertex of the face
            std::string vertexData;
            std::vector<std::string> faceVertices;

            while (iss >> vertexData) {
                faceVertices.push_back(vertexData);
            }

            // Triangulate faces (handles quads and n-gons)
            for (size_t i = 1; i < faceVertices.size() - 1; i++) {
                ProcessVertex(faceVertices[0], tempVertices, tempUVs, tempNormals, meshData);
                ProcessVertex(faceVertices[i], tempVertices, tempUVs, tempNormals, meshData);
                ProcessVertex(faceVertices[i + 1], tempVertices, tempUVs, tempNormals, meshData);
            }
        }
    }

    file.close();

    // Generate simple indices (since we've duplicated vertices)
    for (int i = 0; i < meshData.vertices.size(); i++) {
        meshData.indices.push_back(i);
    }

    std::cout << "Loaded OBJ: " << meshData.vertices.size() << " vertices, "
        << meshData.indices.size() / 3 << " triangles" << std::endl;

    return meshData;
}

void OBJLoader::ProcessVertex(
    const std::string& vertexData,
    const std::vector<glm::vec3>& tempVertices,
    const std::vector<glm::vec2>& tempUVs,
    const std::vector<glm::vec3>& tempNormals,
    MeshData& meshData)
{
    std::vector<int> indices;
    std::stringstream ss(vertexData);
    std::string index;

    // Parse format: v/vt/vn or v//vn or v/vt or v
    while (std::getline(ss, index, '/')) {
        if (index.empty()) {
            indices.push_back(-1);
        }
        else {
            indices.push_back(std::stoi(index) - 1); // OBJ indices are 1-based
        }
    }

    // Add vertex position (required)
    if (indices.size() > 0 && indices[0] >= 0) {
        meshData.vertices.push_back(tempVertices[indices[0]]);
    }

    // Add texture coordinate (optional)
    if (indices.size() > 1 && indices[1] >= 0) {
        meshData.textureCoord.push_back(tempUVs[indices[1]]);
    }
    else {
        meshData.textureCoord.push_back(glm::vec2(0.0f));
    }

    // Add normal (optional)
    if (indices.size() > 2 && indices[2] >= 0) {
        meshData.normals.push_back(tempNormals[indices[2]]);
    }
    else {
        meshData.normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    }
}