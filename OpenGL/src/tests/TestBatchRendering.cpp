#include "TestBatchRendering.h"
#include "glm/gtc/matrix_transform.hpp"
#include "iostream"

test::TestBatchRendering::TestBatchRendering() :
	vb(), ib(), vao(), shader("res/shaders/circleVert.shader", "res/shaders/circleFrag.shader"),
	mvpMatrix(1.0f)
{
	const unsigned int numOfVertices = 20;
	vb.ReserveSpace(sizeof(Vertex) * numOfVertices, GL_DYNAMIC_DRAW);
	vao.Bind();
	ib.Bind();
	
	vao.AddVertexAttribPointer(2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));
	vao.AddVertexAttribPointer(2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uvCoord));

	unsigned int indices[] = {
			0,1,3,
			0,3,2,
			4,5,7,
			4,7,6
	};
	ib.AddData(indices, 12);

	glm::mat4 proj = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);
	mvpMatrix *= proj;

	shader.Bind();
	shader.SetUniformMat4f("u_MVP", mvpMatrix);

}

test::TestBatchRendering::~TestBatchRendering()
{

}

void test::TestBatchRendering::OnRender()
{
	shader.Bind();
	vao.Bind();
	ib.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void test::TestBatchRendering::OnUpdate(float deltaTime)
{
	float vertices[] = {
		//top left position   UV Coordinates
		-1.0f,1.0f,			  -1.0f, 1.0f,//0
		//top right position
		1.0f,1.0f,			  1.0f,1.0f,//1     
		//bottom left position
		-1.0f,-1.0f,		  -1.0f,-1.0f,//2
		//bottom right position
		1.0f,-1.0f,			  1.0f,-1.0f,//3

		//top left position   UV Coordinates
		-3.0f,1.0f,			  -1.0f, 1.0f,//0
		//top right position
		-1.0f,1.0f,			  1.0f,1.0f,//1     
		//bottom left position
		-3.0f,-1.0f,		  -1.0f,-1.0f,//2
		//bottom right position
		-1.0f,-1.0f,		  1.0f,-1.0f//3
	};
	vb.Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

}

void test::TestBatchRendering::OnImGuiRender()
{

}
