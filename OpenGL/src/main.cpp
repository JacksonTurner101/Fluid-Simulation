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
#include "shapes/Triangle.h"
#include "shapes/Square.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //-----OpenGL version 3.3 - Core-----//
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Creating ImGUI Context
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR! Glew is not GLEW_OK!" << std::endl;
    }
    
    //-----VERTEX BUFFER STUFF-----//
    //2d positions for a square
    float positions[8] = {
        //top left
        -100.0f,100.0f,//0
        //top right
        100.0f,100.0f,//1
        //bottom left
        -100.0f,-100.0f,//2
        //bottom right
        100.0f,-100.0f//3
    };

    VertexBuffer* vb = new VertexBuffer(positions, 8 * sizeof(float));

    //-----Vertex Array Object-----//

    VertexArray* vao = new VertexArray();
    vao->AddVertexAttribPointer(2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    //-----INDEX BUFFER STUFF-----//
    unsigned int indices[] = {
       0,1,3,
       0,3,2
    };

    IndexBuffer* ib = new IndexBuffer(indices, 6);

    //-----SHADER STUFF-----//
    Shader* shader = new Shader("res/shaders/vertex.shader", "res/shaders/fragment.shader");

    glm::mat4 proj = glm::ortho(-400.0f, 400.0f, -300.0f, 300.0f, -1.0f, 1.0f);

    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 0.0f, 0.0f));

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));

    glm::mat4 MVPMatrix = proj * view * model;

    shader->Bind();
    shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
    shader->SetUniformMat4f("u_MVP", MVPMatrix);
    
    
    //-----Delta Time testing-----//
    double now = glfwGetTime();
    double last = glfwGetTime();
    double deltaTime = 0;
    //end testing
    
    //Shape testing
    //Triangle triangle;
    //Square square;
    
    float red = 0.0f;
    float green = 0.5f;
    float blue = 1.0f;
    float alpha = 1.0f;

    float changeValR = 1;
    float changeValG = 1;
    float changeValB = 1;
    float changeValA = 1;

    //Clearing everything
    vao->Unbind();
    shader->Unbind();
    vb->Unbind();
    ib->Unbind();


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    Renderer* renderer = new Renderer();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();

        /* Render here */
        renderer->Clear();

        // Delta Time Testing
        now = glfwGetTime();
        deltaTime = now - last;
        last = now;

        if (red <= 0.0f) {
            changeValR = 1;
        }
        else if (red >= 1.0f) {
            changeValR = -1;
        }

        if (green <= 0.0f) {
            changeValG = 1;
        }
        else if (green >= 1.0f) {
            changeValG = -1;
        }

        if (blue <= 0.0f) {
            changeValB = 1;
        }
        else if (blue >= 1.0f) {
            changeValB = -1;
        }

        if (alpha <= 0.0f) {
            changeValA = 1;
        }
        else if (alpha >= 1.0f) {
            changeValA = -1;
        }

        red += changeValR * deltaTime;
        green += changeValG * deltaTime;
        blue += changeValB * deltaTime;
        alpha += changeValA * deltaTime;
      
        shader->Bind();
        shader->SetUniform4f("u_Color", red, green, blue, alpha);

        renderer->Draw(*vao, *ib, *shader);
        //triangle.Draw();
        //square.Draw();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }


    //---REMEMBER--- this is beacuse I have allocated these objects to the HEAP and need to free the memory
    //This gets rid of the perpetual while loop that glfwTerminate() gets into when trying to clean up when these are on the stack
    delete vb;
    delete ib;
    delete vao;
    delete shader;
    delete renderer;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();;

    glfwTerminate();
    return 0;
}