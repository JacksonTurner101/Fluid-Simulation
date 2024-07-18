#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <fstream>
#include <string>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

static std::string ReadAndReturnFileString(const std::string& filePath) {
    
    std::ifstream stream(filePath);
    std::string line;
    std::string output;

    if (!stream.is_open())
    {
        std::cout << "Failed to open file at file path: " + filePath;
        return "";
    }
    else {
        std::cout << "successfully opened file at Path:" << std::endl;
        std::cout << filePath << std::endl;
    }

    while (getline(stream, line)) {
        output += line + '\n';
    }

    return output;
}

static unsigned int CompileShader(const std::string& source, unsigned int type) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {

        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl; 
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;

}

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
    
    //-----Vertex Array Object-----//
    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

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

    //Enables vertex buffer then sets the layout
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));


    //-----INDEX BUFFER STUFF-----//
    unsigned int indices[] = {
       0,1,3,
       0,3,2
    };

    IndexBuffer* ib = new IndexBuffer(indices, 6);

    //-----SHADER STUFF-----//
    unsigned int shader = CreateShader(ReadAndReturnFileString("res/shaders/vertex.shader"), ReadAndReturnFileString("res/shaders/fragment.shader"));
    GLCall(glUseProgram(shader));

    int location = glGetUniformLocation(shader, "u_Color");
    GLCall(glUniform4f(location,1.0f,0.5f,0.0f,1.0f));
    
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
    GLCall(glBindVertexArray(0));
    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));


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

        GLCall(glUseProgram(shader));
        glUniform4f(location, red, green, blue, 1.0f);

        GLCall(glBindVertexArray(vao));
        ib->Bind();

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    //---REMEMBER--- this is beacuse I have allocated these objects to the stack and need to free the memory
    //This gets rid of the perpetual while loop that glfwTerminate() gets into when trying to clean up when these are on the stack
    delete vb;
    delete ib;

    glfwTerminate();
    return 0;
}