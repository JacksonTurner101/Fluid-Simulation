#pragma once
#include "../IndexBuffer.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../Shader.h"
#include "../Renderer.h"

class Shape {
public:
	virtual void Draw() = 0;
	IndexBuffer GetIndexBuffer();
	VertexArray GetVertexArray();
	Shader GetShader();
	Shape();
protected:
	VertexBuffer m_Vb;
	VertexArray m_Vao;
	IndexBuffer m_Ib;
	Shader m_Shader;
};