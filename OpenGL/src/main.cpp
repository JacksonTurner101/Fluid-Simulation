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
    Renderer* renderer = new Renderer();

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

    //-----Creating ImGUI Context and Initialising-----//
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
    float vertices[16] = {
        //top left position   UV Coordinates
        -1.0f,1.0f,       -1.0f, 1.0f,//0
        //top right position
        1.0f,1.0f,        1.0f,1.0f,//1     
        //bottom left position
        -1.0f,-1.0f,      -1.0f,-1.0f,//2
        //bottom right position
        1.0f,-1.0f,        1.0f,-1.0f//3
    };

    VertexBuffer* vb = new VertexBuffer(vertices, 16 * sizeof(float));

    //-----Vertex Array Object-----//

    VertexArray* vao = new VertexArray();
    vao->AddVertexAttribPointer(2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    vao->AddVertexAttribPointer(2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)(2 * sizeof(float)));

    //-----INDEX BUFFER STUFF-----//
    unsigned int indices[] = {
       0,1,3,
       0,3,2
    };

    IndexBuffer* ib = new IndexBuffer(indices, 6);

    //-----SHADER STUFF-----//
    Shader* shader = new Shader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
    //shader->Bind();
    //shader->SetUniform3f("u_Color", 1.0f, 1.0f, 1.0f);

    //-----Vertex Shader Matrix Stuff-----//
    glm::mat4 proj = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    //glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f)); <-- Moved into render loop so I can change model matrix with ImGui
    //glm::mat4 MVPMatrix = proj * view * model;

    //shader->SetUniformMat4f("u_MVP", MVPMatrix);
    
    
    //-----Delta Time testing-----//
    float now = (float)glfwGetTime();
    float last = (float)glfwGetTime();
    float deltaTime = 0;
    
    //-----Shape testing-----//
    //Triangle triangle;
    //Square square;
    
    //-----Changing Colours Testing-----//
    float red = 0.0f;
    float green = 0.5f;
    float blue = 1.0f;

    float changeValR = 1;
    float changeValG = 1;
    float changeValB = 1;

    //Clearing everything
    vao->Unbind();
    shader->Unbind();
    vb->Unbind();
    ib->Unbind();

    //This enables alpha value so I can make stuff see through
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Variables
    glm::vec3 translation(0.0f, 0.0f, 0.0f);
    float angle = glm::radians(0.0f);
    float degrees = 0.0f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //----ImGui Stuff----//
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //ImGui::ShowDemoWindow();
        // 
        //Make Sure Gui stuff goes between begin and end
        ImGui::Begin("Stuff");
        ImGui::SliderFloat("X Translation", &translation.x,-4.0f,4.0f);
        ImGui::SliderFloat("Y Translation", &translation.y, -3.0f, 3.0f);
        ImGui::SliderFloat("Z Rotate", &degrees, 0.0f, 360.0f);
        ImGui::End();
        //-----End of ImGui Stuff-----//

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

        red += changeValR * deltaTime;
        green += changeValG * deltaTime;
        blue += changeValB * deltaTime;
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, translation);
        angle = glm::radians(degrees);
        model = glm::rotate(model, angle, glm::vec3(0.0f,0.0f,1.0f));
        glm::mat4 MVPMatrix = proj * view * model;

        shader->Bind();
        shader->SetUniform3f("u_Color", red, green, blue);
        shader->SetUniformMat4f("u_MVP", MVPMatrix);

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