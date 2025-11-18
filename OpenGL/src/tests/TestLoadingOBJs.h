#pragma once

#include "Test.h"
#include "MeshManager.h"
#include <string>

namespace test {

    class TestLoadingOBJs : public Test
    {
    public:
        TestLoadingOBJs();
        ~TestLoadingOBJs();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        std::string m_SelectedFilePath;
        std::string m_FileName;
        std::string m_MeshName;

        bool m_FlipUVs;
        bool m_GenerateNormals;

        char m_MeshNameBuffer[128];

        bool OpenFileDialog(std::string& outPath);
    };
}