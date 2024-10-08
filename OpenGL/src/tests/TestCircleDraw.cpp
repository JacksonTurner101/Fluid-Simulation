#include "TestCircleDraw.h"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

	test::TestCircleDraw::TestCircleDraw()
		: shader("res/shaders/circleVert.shader", "res/shaders/circleFrag.shader"),
		vao(),vb(),ib(),
		mvpMatrix(1.0f)
	{

		vb.Bind();
		vao.Bind();
		ib.Bind();
		float vertices[16] = {
			//top left position   UV Coordinates
			-0.5f,0.5f,       -1.0f, 1.0f,//0
			//top right position
			0.5f,0.5f,        1.0f,1.0f,//1     
			//bottom left position
			-0.5f,-0.5f,      -1.0f,-1.0f,//2
			//bottom right position
			0.5f,-0.5f,        1.0f,-1.0f//3
		};

		vb.AddData(vertices, 16 * sizeof(float));

		vao.AddVertexAttribPointer(2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		vao.AddVertexAttribPointer(2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)(2 * sizeof(float)));


		unsigned int indices[] = {
			0,1,3,
			0,3,2
		};

		ib.AddData(indices, 6);

		glm::mat4 proj = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);
		mvpMatrix *= proj;

		shader.Bind();
		shader.SetUniformMat4f("u_MVP", mvpMatrix);

	}

	test::TestCircleDraw::~TestCircleDraw()
	{

	}

	void test::TestCircleDraw::OnUpdate(float deltaTime)
	{

	}

	void test::TestCircleDraw::OnRender()
	{
		shader.Bind();
		vao.Bind();
		ib.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void test::TestCircleDraw::OnImGuiRender()
	{
	}

}
