#pragma once

#include <glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();

#ifdef _DEBUG
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
    #define GLCall(x)
#endif

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

struct RenderData {
    VertexArray vao;
    IndexBuffer ib;
    Shader shader;
    GLenum mode;
    int count;
    GLenum type;
};

class Renderer {

public:

    void Draw(const VertexArray& vao, const IndexBuffer& ib, const Shader& shader) const;
    ~Renderer();
    Renderer();
    void Clear() const;

private:

};