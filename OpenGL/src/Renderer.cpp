#include"Renderer.h"
#include <iostream>

void GLClearError() {
    while (glGetError() != GL_NO_ERROR) {

    }
}

bool GLLogCall(const char* function, const char* file, int line) {

    while (GLenum error = glGetError()) {

        std::cout << "[OpenGL Error] (" << error << "):\n" <<
            "    function: " << function << "\n    file: " << file <<
            "\n    line: " << line << std::endl;
        return false;
    }
    return true;
}


void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ib, const Shader& shader) const {

    shader.Bind();
    vao.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

Renderer::~Renderer()
{

}

Renderer::Renderer()
{

}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}
