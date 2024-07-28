#include "Shader.h"
#include "glew.h"
#include "iostream"
#include "fstream"
#include "Renderer.h"

Shader::Shader(const std::string& vertexShaderFilePath, const std::string fragmentShaderFilePath)
{
    m_Id = CreateShader(ReadAndReturnFileString(vertexShaderFilePath), ReadAndReturnFileString(fragmentShaderFilePath));

}

Shader::Shader()
{
    //If calling default constructor, assign default shaders
    m_Id = CreateShader(ReadAndReturnFileString("res/shaders/defaultVert.shader"), ReadAndReturnFileString("res/shaders/defaultFrag.shader"));
}

Shader::~Shader()
{
    glDeleteProgram(m_Id);
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_Id));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));

}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_Id, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: Uniform " << name << "doesnt exsist" << std::endl;
    }
    
     m_UniformLocationCache[name] = location;
    
    return location;


}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
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

unsigned int Shader::CompileShader(const std::string& source, unsigned int type)
{
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

std::string Shader::ReadAndReturnFileString(const std::string& filePath)
{
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
