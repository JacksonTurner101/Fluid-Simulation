#pragma once

#include <map>
#include <string>
#include "OBJParser.h"  // Changed from OBJLoader.h

class MeshManager {
public:
    // Singleton access
    static MeshManager& Instance();

    // Prevent copying
    MeshManager(const MeshManager&) = delete;
    MeshManager& operator=(const MeshManager&) = delete;

    // Mesh operations
    MeshData* LoadMesh(const std::string& name, const std::string& filePath,
        bool flipUVs = false, bool generateNormals = false);
    MeshData* GetMesh(const std::string& name);
    bool HasMesh(const std::string& name) const;
    void UnloadMesh(const std::string& name);
    void Clear();

    // Get list of all loaded meshes
    std::vector<std::string> GetLoadedMeshNames() const;

    // Get count of loaded meshes
    size_t GetMeshCount() const { return meshes.size(); }

private:
    MeshManager() = default;
    ~MeshManager() = default;

    std::map<std::string, MeshData> meshes;
};