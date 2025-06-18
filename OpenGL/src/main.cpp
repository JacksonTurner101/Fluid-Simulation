#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <fstream>
#include <string>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestChangeColorUniform.h"
#include "tests/TestCircleDraw.h"
#include "tests/TestModelViewProjection.h"
#include "tests/TestPhysics.h"
#include "tests/TestBatchRendering.h"



int main(void)
{
    GLFWwindow* window;
    Renderer* renderer = new Renderer();
    

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary);
    
    //-----OpenGL version 3.3 - Core-----//
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    int windowWidth = 1200;
    int windowHeight = 800;
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowWidth, windowHeight, "Window 1", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwSetWindowPos(window, (mode->width / 2) - (windowWidth / 2), (mode->height/2) - (windowHeight / 2));
    

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //-----Creating ImGUI Context and Initialising-----//
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR! Glew is not GLEW_OK!" << std::endl;
    }

    //This enables alpha value so I can make stuff see through
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    test::TestBatchRendering* test = new test::TestBatchRendering();
    
    //-----Delta Time -----//
    float now = (float)glfwGetTime();
    float last = (float)glfwGetTime();
    float deltaTime = 0;

    bool showGui = false;
    bool lastTKeyState = false;
    while (!glfwWindowShouldClose(window))
    {
        // Delta Time
        now = glfwGetTime();
        deltaTime = now - last;
        last = now;

        bool tKeyState = glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS;
        if (tKeyState && !lastTKeyState)
        {
            showGui = !showGui;
        }
        lastTKeyState = tKeyState;

        renderer->Clear();
        test->OnUpdate(deltaTime);
        test->OnRender();
        if (showGui)
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::Begin("Variables");

            test->OnImGuiRender();

            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete renderer;
    delete test;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();;

    glfwTerminate();
    return 0;
}