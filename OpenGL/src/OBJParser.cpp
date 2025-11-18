#include "OBJParser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

OBJParser::OBJParser()
{
}

OBJParser::~OBJParser()
{
}

void OBJParser::ParseFaceVertex(const std::string& vertexStr,
    int& posIndex, int& texIndex, int& normIndex)
{
    posIndex = 0;
    texIndex = 0;
    normIndex = 0;

    std::istringstream viss(vertexStr);
    std::string indexStr;

    // Parse position index (v)
    if (std::getline(viss, indexStr, '/'))
    {
        if (!indexStr.empty())
            posIndex = std::stoi(indexStr);
    }

    // Parse texture coordinate index (vt)
    if (std::getline(viss, indexStr, '/'))
    {
        if (!indexStr.empty())
            texIndex = std::stoi(indexStr);
    }

    // Parse normal index (vn)
    if (std::getline(viss, indexStr, '/'))
    {
        if (!indexStr.empty())
            normIndex = std::stoi(indexStr);
    }
}

void OBJParser::GenerateNormals(MeshData& meshData)
{
    meshData.normals.clear();
    meshData.normals.resize(meshData.vertices.size(), glm::vec3(0.0f));

    // Calculate face normals and accumulate
    for (size_t i = 0; i < meshData.indices.size(); i += 3)
    {
        unsigned int idx0 = meshData.indices[i];
        unsigned int idx1 = meshData.indices[i + 1];
        unsigned int idx2 = meshData.indices[i + 2];

        glm::vec3 v0 = meshData.vertices[idx0];
        glm::vec3 v1 = meshData.vertices[idx1];
        glm::vec3 v2 = meshData.vertices[idx2];

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

        meshData.normals[idx0] += normal;
        meshData.normals[idx1] += normal;
        meshData.normals[idx2] += normal;
    }

    // Normalize all normals
    for (auto& normal : meshData.normals)
    {
        normal = glm::normalize(normal);
    }
}

bool OBJParser::LoadOBJ(const std::string& filepath, MeshData& outMeshData)
{
    return LoadOBJ(filepath, outMeshData, false, false);
}

bool OBJParser::LoadOBJ(const std::string& filepath, MeshData& outMeshData,
    bool flipUVs, bool generateNormals)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open OBJ file: " << filepath << std::endl;
        return false;
    }

    // Clear output data
    outMeshData.vertices.clear();
    outMeshData.textureCoord.clear();
    outMeshData.normals.clear();
    outMeshData.indices.clear();

    // Temporary storage for OBJ data (as read from file)
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_texCoords;
    std::vector<glm::vec3> temp_normals;

    // For handling unique vertex combinations (position/texcoord/normal)
    struct VertexKey {
        int posIndex;
        int texIndex;
        int normIndex;

        bool operator==(const VertexKey& other) const {
            return posIndex == other.posIndex &&
                texIndex == other.texIndex &&
                normIndex == other.normIndex;
        }
    };

    struct VertexKeyHash {
        std::size_t operator()(const VertexKey& k) const {
            return std::hash<int>()(k.posIndex) ^
                (std::hash<int>()(k.texIndex) << 1) ^
                (std::hash<int>()(k.normIndex) << 2);
        }
    };

    std::unordered_map<VertexKey, unsigned int, VertexKeyHash> vertexMap;

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line))
    {
        lineNumber++;

        if (line.empty() || line[0] == '#')
            continue; // Skip empty lines and comments

        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v")
        {
            // Vertex position
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        }
        else if (prefix == "vt")
        {
            // Texture coordinate
            glm::vec2 texCoord;
            iss >> texCoord.x >> texCoord.y;

            if (flipUVs)
                texCoord.y = 1.0f - texCoord.y;

            temp_texCoords.push_back(texCoord);
        }
        else if (prefix == "vn")
        {
            // Normal
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        }
        else if (prefix == "f")
        {
            // Face - format: f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
            std::vector<unsigned int> faceIndices;
            std::string vertexStr;

            while (iss >> vertexStr)
            {
                int posIdx = 0, texIdx = 0, normIdx = 0;
                ParseFaceVertex(vertexStr, posIdx, texIdx, normIdx);

                if (posIdx == 0)
                {
                    std::cerr << "Invalid face at line " << lineNumber << std::endl;
                    continue;
                }

                // OBJ indices are 1-based, convert to 0-based
                posIdx--;
                texIdx--;
                normIdx--;

                // Create vertex key
                VertexKey key = { posIdx, texIdx, normIdx };

                // Check if this vertex combination already exists
                auto it = vertexMap.find(key);
                unsigned int index;

                if (it != vertexMap.end())
                {
                    // Reuse existing vertex
                    index = it->second;
                }
                else
                {
                    // Create new vertex
                    index = (unsigned int)outMeshData.vertices.size();
                    vertexMap[key] = index;

                    // Add vertex position
                    if (posIdx >= 0 && posIdx < temp_vertices.size())
                    {
                        outMeshData.vertices.push_back(temp_vertices[posIdx]);
                    }
                    else
                    {
                        std::cerr << "Invalid vertex index at line " << lineNumber << std::endl;
                        outMeshData.vertices.push_back(glm::vec3(0.0f));
                    }

                    // Add texture coordinate
                    if (texIdx >= 0 && texIdx < temp_texCoords.size())
                    {
                        outMeshData.textureCoord.push_back(temp_texCoords[texIdx]);
                    }
                    else if (!temp_texCoords.empty())
                    {
                        outMeshData.textureCoord.push_back(glm::vec2(0.0f));
                    }

                    // Add normal
                    if (normIdx >= 0 && normIdx < temp_normals.size())
                    {
                        outMeshData.normals.push_back(temp_normals[normIdx]);
                    }
                    else if (!temp_normals.empty() && !generateNormals)
                    {
                        outMeshData.normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
                    }
                }

                faceIndices.push_back(index);
            }

            // Triangulate face (for quads and n-gons)
            // Using simple fan triangulation
            if (faceIndices.size() >= 3)
            {
                for (size_t i = 1; i < faceIndices.size() - 1; i++)
                {
                    outMeshData.indices.push_back(faceIndices[0]);
                    outMeshData.indices.push_back(faceIndices[i]);
                    outMeshData.indices.push_back(faceIndices[i + 1]);
                }
            }
        }
    }

    file.close();

    // Generate normals if requested or if none were loaded
    if (generateNormals || (outMeshData.normals.empty() && !outMeshData.vertices.empty()))
    {
        std::cout << "Generating normals..." << std::endl;
        GenerateNormals(outMeshData);
    }

    // Print statistics
    std::cout << "===== OBJ Loaded Successfully =====" << std::endl;
    std::cout << "File: " << filepath << std::endl;
    std::cout << "Vertices: " << outMeshData.vertices.size() << std::endl;
    std::cout << "Texture Coords: " << outMeshData.textureCoord.size() << std::endl;
    std::cout << "Normals: " << outMeshData.normals.size() << std::endl;
    std::cout << "Indices: " << outMeshData.indices.size() << std::endl;
    std::cout << "Triangles: " << outMeshData.indices.size() / 3 << std::endl;
    std::cout << "====================================" << std::endl;

    return !outMeshData.vertices.empty();
}