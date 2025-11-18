#include "TestLoadingOBJs.h"
#include "ImGui/imgui.h"
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#include <commdlg.h>
#endif

namespace test {

    TestLoadingOBJs::TestLoadingOBJs()
        : m_FlipUVs(false), m_GenerateNormals(true)
    {
        memset(m_MeshNameBuffer, 0, sizeof(m_MeshNameBuffer));
        strcpy_s(m_MeshNameBuffer, "MyMesh");
    }

    TestLoadingOBJs::~TestLoadingOBJs()
    {
    }

    bool TestLoadingOBJs::OpenFileDialog(std::string& outPath)
    {
#ifdef _WIN32
        OPENFILENAMEA ofn;
        char szFile[260] = { 0 };

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "OBJ Files (*.obj)\0*.obj\0All Files (*.*)\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&ofn) == TRUE)
        {
            outPath = ofn.lpstrFile;
            return true;
        }
#endif
        return false;
    }

    void TestLoadingOBJs::OnRender()
    {
        // Render your loaded models here
        // You can get them from MeshManager::Instance().GetMesh("name")
    }

    void TestLoadingOBJs::OnUpdate(float deltaTime)
    {
    }

    void TestLoadingOBJs::OnImGuiRender()
    {
        ImGui::Text("OBJ File Loader");
        ImGui::Separator();

        // === FILE SELECTION ===
        if (ImGui::Button("Browse OBJ File...", ImVec2(150, 0)))
        {
            std::string path;
            if (OpenFileDialog(path))
            {
                m_SelectedFilePath = path;
                size_t lastSlash = path.find_last_of("/\\");
                m_FileName = (lastSlash != std::string::npos) ?
                    path.substr(lastSlash + 1) : path;

                // Auto-generate mesh name from filename (without extension)
                size_t lastDot = m_FileName.find_last_of('.');
                std::string baseName = (lastDot != std::string::npos) ?
                    m_FileName.substr(0, lastDot) : m_FileName;
                strcpy_s(m_MeshNameBuffer, baseName.c_str());
            }
        }

        ImGui::SameLine();

        if (!m_SelectedFilePath.empty())
        {
            ImGui::Text("%s", m_FileName.c_str());
        }
        else
        {
            ImGui::TextDisabled("No file selected");
        }

        ImGui::Spacing();
        ImGui::Separator();

        // === LOAD OPTIONS ===
        if (!m_SelectedFilePath.empty())
        {
            ImGui::Text("Load Options:");
            ImGui::InputText("Mesh Name", m_MeshNameBuffer, sizeof(m_MeshNameBuffer));
            ImGui::Checkbox("Flip UVs", &m_FlipUVs);
            ImGui::Checkbox("Generate Normals", &m_GenerateNormals);

            ImGui::Spacing();

            // Check if name already exists
            std::string meshName(m_MeshNameBuffer);
            bool alreadyExists = MeshManager::Instance().HasMesh(meshName);

            if (alreadyExists)
            {
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),
                    "Warning: A mesh with this name already exists!");
            }

            if (ImGui::Button("Load OBJ", ImVec2(150, 0)))
            {
                if (strlen(m_MeshNameBuffer) == 0)
                {
                    std::cerr << "Please enter a mesh name!" << std::endl;
                }
                else
                {
                    MeshData* loadedMesh = MeshManager::Instance().LoadMesh(
                        meshName,
                        m_SelectedFilePath,
                        m_FlipUVs,
                        m_GenerateNormals
                    );

                    if (loadedMesh == nullptr)
                    {
                        std::cerr << "Failed to load mesh!" << std::endl;
                    }
                }
            }
        }

        ImGui::Spacing();
        ImGui::Separator();

        // === LOADED MESHES LIST ===
        ImGui::Text("Loaded Meshes (%zu):", MeshManager::Instance().GetMeshCount());

        if (MeshManager::Instance().GetMeshCount() > 0)
        {
            ImGui::BeginChild("MeshList", ImVec2(0, 200), true);

            auto meshNames = MeshManager::Instance().GetLoadedMeshNames();

            for (const auto& name : meshNames)
            {
                MeshData* mesh = MeshManager::Instance().GetMesh(name);

                if (ImGui::TreeNode(name.c_str()))
                {
                    if (mesh)
                    {
                        ImGui::BulletText("Vertices: %zu", mesh->vertices.size());
                        ImGui::BulletText("Texture Coords: %zu", mesh->textureCoord.size());
                        ImGui::BulletText("Normals: %zu", mesh->normals.size());
                        ImGui::BulletText("Indices: %zu", mesh->indices.size());
                        ImGui::BulletText("Triangles: %zu", mesh->indices.size() / 3);

                        ImGui::Spacing();

                        if (ImGui::Button(("Unload##" + name).c_str()))
                        {
                            MeshManager::Instance().UnloadMesh(name);
                        }
                    }

                    ImGui::TreePop();
                }
            }

            ImGui::EndChild();

            if (ImGui::Button("Clear All Meshes"))
            {
                MeshManager::Instance().Clear();
            }
        }
        else
        {
            ImGui::TextDisabled("No meshes loaded");
        }
    }
}