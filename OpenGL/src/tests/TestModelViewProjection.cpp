#include "TestModelViewProjection.h"

namespace test {

	test::TestModelViewProjection::TestModelViewProjection()
		: shader("res/shaders/vertex.shader", "res/shaders/fragment.shader"),
		vao(), vb(), ib(),
		translation(0.0f,0.0f,0.0f),
		rotation(0.0f),
		scale(1.0f),
		modelMatrix(glm::mat4(1.0f))
	{
		vb.Bind();
		vao.Bind();
		ib.Bind();
		float vertices[16] = {
			//top left position   UV Coordinates
			-1.0f,1.0f,			  -1.0f, 1.0f,//0
			//top right position
			1.0f,1.0f,			  1.0f,1.0f,//1     
			//bottom left position
			-1.0f,-1.0f,		  -1.0f,-1.0f,//2
			//bottom right position
			1.0f,-1.0f,			  1.0f,-1.0f//3
		};

		vb.AddData(vertices, 16 * sizeof(float));

		vao.AddVertexAttribPointer(2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		vao.AddVertexAttribPointer(2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)(2 * sizeof(float)));

		unsigned int indices[] = {
			0,1,3,
			0,3,2
		};

		ib.AddData(indices, 6);


		projectionMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);
		viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 MVPMatrix = modelMatrix * viewMatrix * projectionMatrix;

	}

	test::TestModelViewProjection::~TestModelViewProjection()
	{

	}

	void test::TestModelViewProjection::OnUpdate(float deltaTime)
	{
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, translation);
		float angle = glm::radians(rotation);
		modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec3 xyzScale(1.0f * scale, 1.0f * scale, 1.0f);
		modelMatrix = glm::scale(modelMatrix, xyzScale);
		glm::mat4 MVPMatrix = projectionMatrix * viewMatrix * modelMatrix;
		shader.Bind();
		shader.SetUniformMat4f("u_MVP", MVPMatrix);

	}

	void test::TestModelViewProjection::OnRender()
	{
		shader.Bind();
		vao.Bind();
		ib.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void test::TestModelViewProjection::OnImGuiRender()
	{

		ImGui::SliderFloat("X Translation", &translation.x, -4.0f, 4.0f);
		ImGui::SliderFloat("Y Translation", &translation.y, -3.0f, 3.0f);
		ImGui::SliderFloat("Z Rotate", &rotation, 0.0f, 360.0f);
		ImGui::SliderFloat("Scale", &scale, 0.1f, 2.0f);

	}

}
