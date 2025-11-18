#include "MeshManager.h"
#include <iostream>

MeshManager& MeshManager::Instance()
{
    static MeshManager instance;
    return instance;
}

MeshData* MeshManager::LoadMesh(const std::string& name, const std::string& filePath,
    bool flipUVs, bool generateNormals)
{
    // Check if already loaded
    if (meshes.find(name) != meshes.end()) {
        std::cout << "Mesh '" << name << "' already loaded. Returning existing mesh." << std::endl;
        return &meshes[name];
    }

    // Create new mesh data
    MeshData newMesh;

    // Load using OBJParser
    if (!OBJParser::LoadOBJ(filePath, newMesh, flipUVs, generateNormals)) {
        std::cerr << "Failed to load mesh: " << filePath << std::endl;
        return nullptr;
    }

    // Store the mesh
    meshes[name] = newMesh;
    std::cout << "Mesh '" << name << "' loaded and stored successfully." << std::endl;

    return &meshes[name];
}

MeshData* MeshManager::GetMesh(const std::string& name)
{
    auto it = meshes.find(name);
    if (it != meshes.end()) {
        return &it->second;
    }

    std::cerr << "Mesh '" << name << "' not found!" << std::endl;
    return nullptr;
}

bool MeshManager::HasMesh(const std::string& name) const
{
    return meshes.find(name) != meshes.end();
}

void MeshManager::UnloadMesh(const std::string& name)
{
    auto it = meshes.find(name);
    if (it != meshes.end()) {
        meshes.erase(it);
        std::cout << "Mesh '" << name << "' unloaded." << std::endl;
    }
    else {
        std::cerr << "Cannot unload mesh '" << name << "' - not found!" << std::endl;
    }
}

void MeshManager::Clear()
{
    size_t count = meshes.size();
    meshes.clear();
    std::cout << "Cleared " << count << " mesh(es) from manager." << std::endl;
}

std::vector<std::string> MeshManager::GetLoadedMeshNames() const
{
    std::vector<std::string> names;
    names.reserve(meshes.size());

    for (const auto& pair : meshes) {
        names.push_back(pair.first);
    }

    return names;
}