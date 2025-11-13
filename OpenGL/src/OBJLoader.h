#pragma once

#include <string>
#include <vector>
#include "glm/glm.hpp"

struct MeshData {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> textureCoord;
    std::vector<glm::vec3> normals;
    std::vector<int> indices;
};

class OBJLoader
{
public:
    OBJLoader();
    ~OBJLoader();

    MeshData ReadOBJFile(const std::string& filePath);

private:
    void ProcessVertex(
        const std::string& vertexData,
        const std::vector<glm::vec3>& tempVertices,
        const std::vector<glm::vec2>& tempUVs,
        const std::vector<glm::vec3>& tempNormals,
        MeshData& meshData
    );
};