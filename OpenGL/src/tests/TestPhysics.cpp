#pragma once

#include "TestPhysics.h"

namespace test {

	test::TestPhysics::TestPhysics()
		: shader("res/shaders/circleVert.shader", "res/shaders/circleFrag.shader"),
		vao(), vb(), ib(), translation(0.0f, -0.01f, 0.0f), modelMatrix(glm::mat4(1.0f)),
		boundingBox(40.0f,30.0f),
		ball{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.7f, -0.3f, 0.0f),glm::vec3(0.0f, -0.02f, 0.0f), 1 }
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

		projectionMatrix = glm::ortho(-boundingBox.x, boundingBox.x, -boundingBox.y, boundingBox.y, -1.0f, 1.0f);
		viewMatrix = glm::translate(glm::mat4(1.0f), ball.position);
		glm::mat4 MVPMatrix = modelMatrix * viewMatrix * projectionMatrix;

	}

	test::TestPhysics::~TestPhysics()
	{

	}

	void test::TestPhysics::OnUpdate(float deltaTime)
	{

		UpdatePosition(deltaTime);
		CheckCollisions();

	}

	void test::TestPhysics::OnRender()
	{
		shader.Bind();
		vao.Bind();
		ib.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void test::TestPhysics::OnImGuiRender()
	{


	}

	void TestPhysics::CheckCollisions()
	{
		if (ball.position.y < -30 + ball.size) {
			ball.velocity.y *= -1;

		}

		if (ball.position.y > 30 - ball.size) {
			ball.velocity.y *= -1;

		}

		if (ball.position.x < -40 + ball.size) {
			ball.velocity.x *= -1;

		}

		if (ball.position.x > 40 - ball.size) {
			ball.velocity.x *= -1;
		}
	}

	void TestPhysics::UpdatePosition(float deltaTime)
	{
		ball.velocity += ball.acceleration;
		ball.position += ball.velocity;
		modelMatrix = glm::translate(modelMatrix, ball.velocity);
		glm::mat4 MVPMatrix = projectionMatrix * viewMatrix * modelMatrix;

		shader.Bind();
		shader.SetUniformMat4f("u_MVP", MVPMatrix);
	}


}
