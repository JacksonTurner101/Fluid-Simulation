#pragma once

#include <map>
#include <string>
#include "OBJLoader.h"

class MeshManager {
public:
    // Singleton access
    static MeshManager& Instance();

    // Prevent copying
    MeshManager(const MeshManager&) = delete;
    MeshManager& operator=(const MeshManager&) = delete;

    // Mesh operations
    MeshData* LoadMesh(const std::string& name, const std::string& filePath);
    MeshData* GetMesh(const std::string& name);
    void UnloadMesh(const std::string& name);
    void Clear();

private:
    MeshManager() = default;
    ~MeshManager() = default;

    std::map<std::string, MeshData> meshes;
};