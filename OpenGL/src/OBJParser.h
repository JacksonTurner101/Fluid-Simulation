#pragma once

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "MeshData.h"


class OBJParser
{
public:
    OBJParser();
    ~OBJParser();

    // Load an OBJ file and return the mesh data
    static bool LoadOBJ(const std::string& filepath, MeshData& outMeshData);

    // Load with options
    static bool LoadOBJ(const std::string& filepath, MeshData& outMeshData,
        bool flipUVs, bool generateNormals = false);

private:
    // Helper function to parse face indices
    static void ParseFaceVertex(const std::string& vertexStr,
        int& posIndex, int& texIndex, int& normIndex);

    // Helper to generate normals if the OBJ doesn't have them
    static void GenerateNormals(MeshData& meshData);
};