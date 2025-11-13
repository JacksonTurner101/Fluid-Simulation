#include "MeshManager.h"

MeshManager& MeshManager::Instance()
{
    static MeshManager instance;
    return instance;
}

MeshData* MeshManager::LoadMesh(const std::string& name, const std::string& filePath)
{
    // Check if already loaded
    if (meshes.find(name) != meshes.end()) {
        return &meshes[name];
    }

    // Load and store
    OBJLoader loader;
    meshes[name] = loader.ReadOBJFile(filePath);
    return &meshes[name];
}

MeshData* MeshManager::GetMesh(const std::string& name)
{
    auto it = meshes.find(name);
    if (it != meshes.end()) {
        return &it->second;
    }
    return nullptr;
}

void MeshManager::UnloadMesh(const std::string& name)
{
    meshes.erase(name);
}

void MeshManager::Clear()
{
    meshes.clear();
}