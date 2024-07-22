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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR! Glew is not GLEW_OK!" << std::endl;
    }
    
    //-----VERTEX BUFFER STUFF-----//
    //2d positions for a square
    float positions[8] = {
        //top left
        -0.5f,0.5f,//0
        //top right
        0.5f,0.5f,//1
        //bottom left
        -0.5f,-0.5f,//2
        //bottom right
        0.5f,-0.5f//3
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

    shader->Bind();
    shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

    
    //-----Delta Time testing-----//
    double now = glfwGetTime();
    double last = glfwGetTime();
    double deltaTime = 0;
    //end testing

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


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

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
        
        shader->Bind();
        shader->SetUniform4f("u_Color", red, green, blue, 1.0f);

        vao->Bind();
        ib->Bind();

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

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

    glfwTerminate();
    return 0;
}