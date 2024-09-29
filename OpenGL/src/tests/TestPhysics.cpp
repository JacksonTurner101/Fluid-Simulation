#pragma once

#include "TestPhysics.h"
#include "iostream"

namespace test {

	test::TestPhysics::TestPhysics()
		: shader("res/shaders/circleVert.shader", "res/shaders/circleFrag.shader"),
		vao(), vb(), ib(), translation(0.0f, -0.01f, 0.0f), modelMatrix(glm::mat4(1.0f)),
		boundingBox(40.0f,30.0f),
		ball{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.7f, -0.3f, 0.0f),glm::vec3(0.0f, -9.8f, 0.0f), 1 },
		speed(1.0f), bounceDampenerVal(1.0f)
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
		viewMatrix = glm::scale(viewMatrix, glm::vec3(ball.size, ball.size, ball.size));
		glm::mat4 MVPMatrix = modelMatrix * viewMatrix * projectionMatrix;

		//top and bottom direction vector = Vec2(1,0)
		//left and right direction vector = Vec2(0,1)

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
		ImGui::SliderFloat("Speed", &speed, 0.0f, 10.0f);
		ImGui::SliderFloat("Ball Size", &ball.size, 1.0f, 10.0f);
		ImGui::SliderFloat("Ball Bounce Dampener Value", &bounceDampenerVal, 0.01f, 1.0f);

	}

	void TestPhysics::CheckCollisions()
	{
		

		CheckBoundingBoxCollision(boundingBox);

	}

	void TestPhysics::CheckBallLineCollision(glm::vec2 linePosition, glm::vec2 lineDirection) {
		float t = ((ball.position.x - linePosition.x) * lineDirection.x +
			(ball.position.y - linePosition.y) * lineDirection.y) /
			((lineDirection.x * lineDirection.x) + (lineDirection.y * lineDirection.y));
		
		float closestCoordX = linePosition.x + t * lineDirection.x;
		float closestCoordY = linePosition.y + t * lineDirection.y;

		float distance = (float)glm::sqrt((ball.position.x - closestCoordX) * (ball.position.x - closestCoordX) + 
		(ball.position.y - closestCoordY) * (ball.position.y - closestCoordY));

		if (distance <= ball.size) {
			float collisionNormalX = ball.position.x - closestCoordX;
			float collisionNormalY = ball.position.y - closestCoordY;

			glm::vec2 normalVector = glm::normalize(glm::vec2(collisionNormalX, collisionNormalY));

			float penetrationDepth = ball.size - distance;


			ResolveCollision(penetrationDepth, normalVector);
		}
	}

	void TestPhysics::CheckBoundingBoxCollision(glm::vec2 boxSize)
	{
		
		if (abs(ball.position.x) > boundingBox.x - ball.size) {
			ball.position.x = (boundingBox.x - 1 * ball.size) * glm::sign(ball.position.x);
			ball.velocity.x *= -1 * bounceDampenerVal;
		}

		if (abs(ball.position.y) > boundingBox.y - ball.size) {
			ball.position.y = (boundingBox.y - 1 * ball.size) * glm::sign(ball.position.y);
			ball.velocity.y *= -1 * bounceDampenerVal;
		}
	}


	//problem here - need a way to reverse the velocity when collision is detected along the correct axis
	 
	void TestPhysics::ResolveCollision(float penetrationDepth, glm::vec2 collisionNormal) {
		modelMatrix = glm::mat4(1.0f); // Reset to identity

		// New ball position
		ball.position += glm::vec3(collisionNormal.x * penetrationDepth, collisionNormal.y * penetrationDepth, 0);

		// Apply scale first, then translation
		modelMatrix = glm::scale(modelMatrix, glm::vec3(ball.size, ball.size, ball.size));
		modelMatrix = glm::translate(modelMatrix, ball.position);

		// Compute MVP matrix
		glm::mat4 MVPMatrix = projectionMatrix * viewMatrix * modelMatrix;

		shader.Bind();
		shader.SetUniformMat4f("u_MVP", MVPMatrix);
	}

	void TestPhysics::UpdatePosition(float deltaTime)
	{
		deltaTime *= speed;

		// Update velocity with acceleration (gravity)
		ball.velocity += ball.acceleration * deltaTime;

		// Update position with velocity
		ball.position += ball.velocity * deltaTime;

		// Reset the model matrix
		modelMatrix = glm::mat4(1.0f); // Reset to identity

		// Apply scale first, then translation
		modelMatrix = glm::translate(modelMatrix, ball.position);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(ball.size, ball.size, ball.size));

		// Compute MVP matrix
		glm::mat4 MVPMatrix = projectionMatrix * viewMatrix * modelMatrix;

		// Bind shader and set the updated MVP matrix
		shader.Bind();
		shader.SetUniformMat4f("u_MVP", MVPMatrix);
	}


}
